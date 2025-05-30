
#include "recomputils.h"
#include "recompdata.h"
#include "globalobjects_api.h"
#include "zproxy_manager.h"
#include "zproxy.h"

bool gIsProxyLoaderisEnabled = false;

U32MemoryHashmapHandle gObjIdToProxyMap;

#define IS_ZPROXY_EXIST(objId) (recomputil_u32_memory_hashmap_contains(gObjIdToProxyMap, objId))
#define GET_ZPROXY(objId) (recomputil_u32_memory_hashmap_get(gObjIdToProxyMap, objId))
#define CREATE_ZPROXY_ENTRY(objId) (recomputil_u32_memory_hashmap_create(gObjIdToProxyMap, objId))
#define REMOVE_ZPROXY_ENTRY(objId) (recomputil_u32_memory_hashmap_erase(gObjIdToProxyMap, objId))

bool isSegmentedPtr(void *p) {
    return SEGMENT_NUMBER(p) <= 0xF;
}

void ZProxyManager_initManager() {
    gObjIdToProxyMap = recomputil_create_u32_memory_hashmap(sizeof(ZProxy));
}

bool ZProxyManager_registerZProxy(ObjectId id) {
    if (!IS_ZPROXY_EXIST(id) && CREATE_ZPROXY_ENTRY(id)) {
        ZProxy *zProxy = GET_ZPROXY(id);

        if (zProxy) {
            ZProxy_initZProxy(zProxy, id);

            // get the non-proxy'd vanilla model into rdram
            ZProxyManager_disableModelInject();

            ZGlobalObj_getGlobalObject(id);

            ZProxyManager_enableModelInject();

            return true;
        }
    }

    return false;
}

bool ZProxyManager_unregisterZProxy(ObjectId id) {
    ZProxy *zProxy = GET_ZPROXY(id);

    if (zProxy) {
        ZProxy_destroyZProxy(zProxy);
        return true;
    }

    return false;
}

bool ZProxyManager_setCustomDisplayList(ObjectId id, Gfx *vanillaDL, Gfx *customDL) {
    if (!isSegmentedPtr(vanillaDL)) {
        return false;
    }

    ZProxy *zProxy = GET_ZPROXY(id);

    if (!zProxy) {
        ZProxyManager_registerZProxy(id);
        zProxy = GET_ZPROXY(id);
    }

    return ZProxy_setCustomDisplayList(zProxy, vanillaDL, customDL);
}

bool ZProxyManager_removeCustomDisplayList(ObjectId id, Gfx *vanillaDL) {
    if (!isSegmentedPtr(vanillaDL)) {
        return false;
    }

    ZProxy *zProxy = GET_ZPROXY(id);

    if (!zProxy) {
        ZProxyManager_registerZProxy(id);
        zProxy = GET_ZPROXY(id);
    }

    return ZProxy_removeCustomDisplayList(zProxy, vanillaDL);
}

bool ZProxyManager_reserveVanillaDisplayList(ObjectId id, Gfx *vanillaDL) {
    if (!isSegmentedPtr(vanillaDL)) {
        return false;
    }

    ZProxy *zProxy = GET_ZPROXY(id);

    if (!zProxy) {
        ZProxyManager_registerZProxy(id);
        zProxy = GET_ZPROXY(id);
    }

    return ZProxy_reserveContainer(zProxy, vanillaDL);
}

void ZProxyManager_enableModelInject() {
    gIsProxyLoaderisEnabled = true;
}

void ZProxyManager_disableModelInject() {
    gIsProxyLoaderisEnabled = false;
}

void *gRam;
uintptr_t gVrom;
size_t gSize;

RECOMP_HOOK("DmaMgr_ProcessRequest")
void on_DmaMgr_RequestSync(DmaRequest *req) {
    gRam = req->dramAddr;
    gVrom = req->vromAddr;
    gSize = req->size;
}

RECOMP_HOOK_RETURN("DmaMgr_ProcessRequest")
void post_DmaMgr_RequestSync() {
    if (!gIsProxyLoaderisEnabled) {
        return;
    }

    unsigned long id;
    if (ZGlobalObj_getObjIdFromVrom(gVrom, &id)) {
        ZProxy *zProxy = GET_ZPROXY(id);
        if (zProxy) {
            Node *curr = LinkedList_start(&zProxy->vanillaDisplayLists);
            while (curr) {
                uintptr_t vanilla = (uintptr_t)curr->data;

                ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(zProxy->vanillaDLToCustomDLMap, vanilla);

                if (container) {

                    gSPBranchList((uintptr_t)gRam + SEGMENT_OFFSET(vanilla), &container->displayList);
                }

                curr = curr->next;
            }
        }
    }

    gVrom = 0;
    gRam = NULL;
}
