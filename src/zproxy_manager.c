
#include "recomputils.h"
#include "recompdata.h"
#include "globalobjects_api.h"
#include "zproxy_manager.h"
#include "zproxy.h"

bool gIsProxyLoaderisEnabled = false;

U32MemoryHashmapHandle gObjIdToProxyMap;

MemorySlotmapHandle gCustomDisplayListEntries;

#define IS_ZPROXY_EXIST(objId) (recomputil_u32_memory_hashmap_contains(gObjIdToProxyMap, objId))
#define GET_ZPROXY(objId) (recomputil_u32_memory_hashmap_get(gObjIdToProxyMap, objId))
#define CREATE_ZPROXY_ENTRY(objId) (recomputil_u32_memory_hashmap_create(gObjIdToProxyMap, objId))
#define REMOVE_ZPROXY_ENTRY(objId) (recomputil_u32_memory_hashmap_erase(gObjIdToProxyMap, objId))

void *slotmapGet(MemorySlotmapHandle slotmap, collection_key_t k) {
    void *result = NULL;

    recomputil_memory_slotmap_get(slotmap, k, &result);

    return result;
}

#define GET_CUSTOM_ENTRY(handle) (slotmapGet(gCustomDisplayListEntries, handle))
#define REMOVE_CUSTOM_ENTRY(handle) (recomputil_memory_slotmap_erase(gCustomDisplayListEntries, handle))

bool isSegmentedPtr(void *p) {
    return ((uintptr_t)p >> 24) <= 0xF;
}

void ZProxyManager_initManager() {
    gObjIdToProxyMap = recomputil_create_u32_memory_hashmap(sizeof(ZProxy));
    gCustomDisplayListEntries = recomputil_create_memory_slotmap(sizeof(ZProxy_CustomDisplayListEntry));
}

ZProxy_CustomDisplayListEntry *ZProxyManager_getCustomEntry(ModelReplacerHandle h) {
    return GET_CUSTOM_ENTRY(h);
}

bool ZProxyManager_registerZProxy(ObjectId id) {
    if (!IS_ZPROXY_EXIST(id) && CREATE_ZPROXY_ENTRY(id)) {
        ZProxy *proxy = GET_ZPROXY(id);

        if (proxy) {
            ZProxy_initZProxy(proxy, id);

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

    ModelReplacerHandle handle = recomputil_memory_slotmap_create(gCustomDisplayListEntries);

    ZProxy_CustomDisplayListEntry *entry = ZProxyManager_getCustomEntry(handle);

    entry->id = id;

    entry->vanillaDL = vanillaDL;

    entry->customDL = NULL;

    return handle;
}

bool ZProxyManager_setDisplayList(ModelReplacerHandle handle, Gfx *customDL) {
    if (customDL != NULL && isSegmentedPtr(customDL)) {
        return false;
    }

    ZProxy_CustomDisplayListEntry *entry = ZProxyManager_getCustomEntry(handle);

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
    ZProxy_CustomDisplayListEntry *entry = ZProxyManager_getCustomEntry(handle);

    if (!entry) {
        return false;
    }

    ZProxyManager_registerZProxy(entry->id);

    ZProxy *proxy = GET_ZPROXY(entry->id);

    return ZProxy_addCustomDisplayList(proxy, handle);
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

extern Gfx object_link_child_DL_017818[]; // bow string

static Gfx sBowStringWrapperDL[] = {
    gsSPNoOp(),                                      // space for replacement DL
    gsSPBranchList(object_link_child_DL_017818 + 2), // continue drawing bowstring
};

RECOMP_HOOK_RETURN("DmaMgr_ProcessRequest")
void post_DmaMgr_RequestSync() {
    if (!gIsProxyLoaderisEnabled) {
        return;
    }

    ObjectId id;
    if (GlobalObjects_getObjectIdFromVrom(gVrom, &id)) {
        ZProxy *proxy = GET_ZPROXY(id);
        if (proxy) {
            size_t size = YAZMTCore_DynamicU32Array_size(proxy->vanillaDisplayLists);
            u32 *data = YAZMTCore_DynamicU32Array_data(proxy->vanillaDisplayLists);

            for (size_t i = 0; i < size; ++i) {
                uintptr_t vanilla = data[i];

                ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(proxy->vanillaDLToCustomDLMap, vanilla);

                if (container) {
                    if (vanilla != (uintptr_t)object_link_child_DL_017818 || id != OBJECT_LINK_CHILD) {
                        gSPBranchList(SEGMENTED_TO_GLOBAL_PTR(gRam, vanilla), &container->displayList);
                    } else {
                        // Allows replaced DL to inherit bow string interpolation from recomp
                        gSPBranchList(sBowStringWrapperDL, &container->displayList);

                        Gfx *bowString = SEGMENTED_TO_GLOBAL_PTR(gRam, vanilla);
                        gSPDisplayList(bowString + 1, sBowStringWrapperDL);
                        gSPEndDisplayList(bowString + 2);
                    }
                }
            }
        }
    }

    gVrom = 0;
    gRam = NULL;
}
