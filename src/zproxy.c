#include "global.h"
#include "recomputils.h"
#include "zproxy.h"
#include "zproxy_manager.h"
#include "globalobjects_api.h"

static Gfx sSetBilerpDL[] = {
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

void ZProxy_initZProxy(ZProxy *zp, ObjectId id) {
    zp->vanillaObjId = id;

    zp->vanillaDLToCustomDLMap = recomputil_create_u32_memory_hashmap(sizeof(ZProxy_ProxyContainer));

    zp->vanillaDisplayLists = YAZMTCore_DynamicU32Array_new();
}

void ZProxy_destroyZProxy(ZProxy *zp) {
    recomputil_destroy_u32_memory_hashmap(zp->vanillaDLToCustomDLMap);
    YAZMTCore_DynamicU32Array_destroy(zp->vanillaDisplayLists);
}

bool ZProxy_reserveContainer(ZProxy *zp, Gfx *vanillaDisplayList) {
    if (!vanillaDisplayList) {
        return false;
    }

    u32 vanilla = (uintptr_t)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(zp->vanillaDLToCustomDLMap, vanilla);

    if (!container) {
        if (recomputil_u32_memory_hashmap_create(zp->vanillaDLToCustomDLMap, vanilla)) {

            YAZMTCore_DynamicU32Array_push(zp->vanillaDisplayLists, (uintptr_t)vanillaDisplayList);

            container = recomputil_u32_memory_hashmap_get(zp->vanillaDLToCustomDLMap, vanilla);

            container->customDisplayListStack = YAZMTCore_DynamicU32Array_new();

            container->vanillaDisplayList = vanillaDisplayList;

            gSPDisplayList(&container->displayList[0], GlobalObjects_getGlobalGfxPtr(zp->vanillaObjId, vanillaDisplayList));

            // Vanilla 3D models use G_TF_BILERP in all cases, so change this back in case a mod set a different filtering mode
            gSPBranchList(&container->displayList[1], sSetBilerpDL);

            return true;
        }
    }

    return false;
}

RECOMP_DECLARE_EVENT(onModelChange(ObjectId id, Gfx *vanillaDL, Gfx *newDL))

void refreshContainerDL(ZProxy *zp, ZProxy_ProxyContainer *c) {
    Gfx *dl = NULL;

    size_t size = YAZMTCore_DynamicU32Array_size(c->customDisplayListStack);
    ZProxy_CustomDisplayListEntry **data = (ZProxy_CustomDisplayListEntry **)YAZMTCore_DynamicU32Array_data(c->customDisplayListStack);

    for (size_t i = 0; i < size && !dl; ++i) {
        ZProxy_CustomDisplayListEntry *entry = data[i];

        if (entry) {
            dl = entry->customDL;
        }
    }

    if (!dl) {
        dl = GlobalObjects_getGlobalGfxPtr(zp->vanillaObjId, c->vanillaDisplayList);
    }

    if ((uintptr_t)dl != c->displayList[0].words.w1) {
        gSPDisplayList(&c->displayList[0], dl);
        onModelChange(zp->vanillaObjId, c->vanillaDisplayList, dl);
    }
}

bool ZProxy_refresh(ZProxy *zp, ModelReplacerHandle handle) {

    ZProxy_CustomDisplayListEntry *entry = ZProxyManager_getCustomEntry(handle);

    if (!entry) {
        return false;
    }

    Gfx *vanillaDisplayList = entry->vanillaDL;

    ZProxy_reserveContainer(zp, vanillaDisplayList);

    if (!vanillaDisplayList) {
        return false;
    }

    uintptr_t vanilla = (uintptr_t)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(zp->vanillaDLToCustomDLMap, vanilla);

    if (container) {
        refreshContainerDL(zp, container);
        return true;
    }

    return false;
}

bool ZProxy_addCustomDisplayList(ZProxy *zp, ModelReplacerHandle handle) {
    ZProxy_CustomDisplayListEntry *entry = ZProxyManager_getCustomEntry(handle);

    if (!entry) {
        return false;
    }

    Gfx *vanillaDisplayList = entry->vanillaDL;

    Gfx *customDisplayList = entry->customDL;

    ZProxy_reserveContainer(zp, vanillaDisplayList);

    if (!vanillaDisplayList) {
        return false;
    }

    uintptr_t vanilla = (uintptr_t)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(zp->vanillaDLToCustomDLMap, vanilla);

    if (container) {
        YAZMTCore_DynamicU32Array_push(container->customDisplayListStack, (uintptr_t)entry);
        refreshContainerDL(zp, container);
        return true;
    }

    return false;
}
