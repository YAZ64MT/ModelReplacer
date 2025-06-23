
#include "recomputils.h"
#include "recompdata.h"
#include "globalobjects_api.h"
#include "zproxy_manager.h"
#include "zproxy.h"

bool gIsProxyLoaderisEnabled = false;

U32MemoryHashmapHandle gObjIdToProxyMap;

// TODO: Replace this with a MemorySlotapHandle
// MemorySlotmaps seem to crash upon element creation?
U32MemoryHashmapHandle gCustomDisplayListEntries;

ModelReplacerHandle gNextCustomHandle = 1;

#define IS_ZPROXY_EXIST(objId) (recomputil_u32_memory_hashmap_contains(gObjIdToProxyMap, objId))
#define GET_ZPROXY(objId) (recomputil_u32_memory_hashmap_get(gObjIdToProxyMap, objId))
#define CREATE_ZPROXY_ENTRY(objId) (recomputil_u32_memory_hashmap_create(gObjIdToProxyMap, objId))
#define REMOVE_ZPROXY_ENTRY(objId) (recomputil_u32_memory_hashmap_erase(gObjIdToProxyMap, objId))

#define GET_CUSTOM_ENTRY(handle) (recomputil_u32_memory_hashmap_get(gCustomDisplayListEntries, handle))
#define REMOVE_CUSTOM_ENTRY(handle) (recomputil_u32_memory_hashmap_erase(gCustomDisplayListEntries, handle))

bool isSegmentedPtr(void *p) {
    return ((uintptr_t)p >> 24) <= 0xF;
}

void ZProxyManager_initManager() {
    gObjIdToProxyMap = recomputil_create_u32_memory_hashmap(sizeof(ZProxy));
    gCustomDisplayListEntries = recomputil_create_u32_memory_hashmap(sizeof(ZProxy_CustomDisplayListEntry));
}

bool ZProxyManager_registerZProxy(ObjectId id) {
    if (!IS_ZPROXY_EXIST(id) && CREATE_ZPROXY_ENTRY(id)) {
        ZProxy *proxy = GET_ZPROXY(id);

        if (proxy) {
            ZProxy_initZProxy(proxy, id, gCustomDisplayListEntries);

            // get the non-proxy'd vanilla model into rdram
            ZProxyManager_disableModelInject();

            GlobalObjects_getGlobalObject(id);

            ZProxyManager_enableModelInject();

            return true;
        }
    }

    return false;
}

bool ZProxyManager_unregisterZProxy(ObjectId id) {
    ZProxy *proxy = GET_ZPROXY(id);

    if (proxy) {
        ZProxy_destroyZProxy(proxy);
        return true;
    }

    return false;
}

ModelReplacerHandle ZProxyManager_createDisplayListHandle(ObjectId id, Gfx *vanillaDL) {
    if (!isSegmentedPtr(vanillaDL)) {
        return false;
    }

    ZProxyManager_registerZProxy(id);

    ModelReplacerHandle handle = gNextCustomHandle;
    gNextCustomHandle++;

    recomputil_u32_memory_hashmap_create(gCustomDisplayListEntries, handle);

    ZProxy_CustomDisplayListEntry *entry = GET_CUSTOM_ENTRY(handle);

    entry->id = id;

    entry->vanillaDL = vanillaDL;

    entry->customDL = NULL;

    return handle;
}

bool ZProxyManager_setDisplayList(ModelReplacerHandle handle, Gfx *customDL) {
    if (customDL != NULL && isSegmentedPtr(customDL)) {
        return false;
    }

    ZProxy_CustomDisplayListEntry *entry = GET_CUSTOM_ENTRY(handle);

    if (!entry) {
        return false;
    }

    entry->customDL = customDL;

    ZProxyManager_registerZProxy(entry->id);

    ZProxy *proxy = GET_ZPROXY(entry->id);

    ZProxy_refresh(proxy, handle);

    return true;
}

bool ZProxyManager_pushDisplayList(ModelReplacerHandle handle) {
    ZProxy_CustomDisplayListEntry *entry = GET_CUSTOM_ENTRY(handle);

    if (!entry) {
        return false;
    }

    ZProxyManager_registerZProxy(entry->id);

    ZProxy *proxy = GET_ZPROXY(entry->id);

    return ZProxy_addCustomDisplayList(proxy, handle);
}

bool ZProxyManager_removeDisplayList(ModelReplacerHandle handle) {
    ZProxy_CustomDisplayListEntry *entry = GET_CUSTOM_ENTRY(handle);

    if (!entry) {
        return false;
    }

    ZProxyManager_registerZProxy(entry->id);

    ZProxy *proxy = GET_ZPROXY(entry->id);

    return ZProxy_removeCustomDisplayList(proxy, handle);
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

    ObjectId id;
    if (GlobalObjects_getObjectIdFromVrom(gVrom, &id)) {
        ZProxy *proxy = GET_ZPROXY(id);
        if (proxy) {
            LinkedListNode *curr = LinkedList_start(proxy->vanillaDisplayLists);
            while (curr) {
                uintptr_t vanilla = (uintptr_t)LinkedListNode_getData(curr);

                ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(proxy->vanillaDLToCustomDLMap, vanilla);

                if (container) {
                    gSPBranchList(SEGMENTED_TO_GLOBAL_PTR(gRam, vanilla), &container->displayList);
                }

                curr = LinkedListNode_getNext(curr);
            }
        }
    }

    gVrom = 0;
    gRam = NULL;
}
