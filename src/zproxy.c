#include "global.h"
#include "recomputils.h"
#include "zproxy.h"
#include "globalobjects_api.h"

void ZProxy_initZProxy(ZProxy *this, ObjectId id) {
    this->vanillaObjId = id;

    this->vanillaDLToCustomDLMap = recomputil_create_u32_memory_hashmap(sizeof(ZProxy_ProxyContainer));
    
    this->vanillaDisplayLists = recomp_alloc(sizeof(LinkedList *));

    LinkedList_initList(this->vanillaDisplayLists);
}

void ZProxy_destroyZProxy(ZProxy *this) {
    recomputil_destroy_u32_memory_hashmap(this->vanillaDLToCustomDLMap);
}

bool ZProxy_reserveContainer(ZProxy *this, Gfx *vanillaDisplayList) {
    u32 vanilla = (uintptr_t)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(this->vanillaDLToCustomDLMap, vanilla);

    if (!container) {
        if (recomputil_u32_memory_hashmap_create(this->vanillaDLToCustomDLMap, vanilla)) {

            LinkedList_addBack(this->vanillaDisplayLists, vanillaDisplayList);

            container = recomputil_u32_memory_hashmap_get(this->vanillaDLToCustomDLMap, vanilla);

            container->vanillaDisplayList = vanillaDisplayList;
            
            gSPBranchList(&container->displayList, ZGlobalObj_getGlobalGfxPtr(this->vanillaObjId, vanillaDisplayList));

            return true;
        }
    }

    return false;
}

bool ZProxy_setCustomDisplayList(ZProxy *this, Gfx *vanillaDisplayList, Gfx customDisplayList[]) {
    ZProxy_reserveContainer(this, vanillaDisplayList);

    uintptr_t vanilla = (uintptr_t)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(this->vanillaDLToCustomDLMap, vanilla);

    if (container) {
        gSPBranchList(&container->displayList, customDisplayList);
        return true;
    }

    return false;
}

bool ZProxy_removeCustomDisplayList(ZProxy *this, Gfx *vanillaDisplayList) {
    ZProxy_reserveContainer(this, vanillaDisplayList);

    uintptr_t vanilla = (uintptr_t)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(this->vanillaDLToCustomDLMap, vanilla);

    if (container) {
        gSPBranchList(&container->displayList, ZGlobalObj_getGlobalGfxPtr(this->vanillaObjId, vanillaDisplayList));
        return true;
    }

    return false;
}
