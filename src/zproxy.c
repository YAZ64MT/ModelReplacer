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
    recomp_free(this->vanillaDisplayLists);
}

bool ZProxy_reserveContainer(ZProxy *this, Gfx *vanillaDisplayList) {
    if (!vanillaDisplayList) {
        return false;
    }

    u32 vanilla = (uintptr_t)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(this->vanillaDLToCustomDLMap, vanilla);

    if (!container) {
        if (recomputil_u32_memory_hashmap_create(this->vanillaDLToCustomDLMap, vanilla)) {

            LinkedList_addBack(this->vanillaDisplayLists, vanillaDisplayList);

            container = recomputil_u32_memory_hashmap_get(this->vanillaDLToCustomDLMap, vanilla);

            container->vanillaDisplayList = vanillaDisplayList;

            container->customDisplayListStack = recomp_alloc(sizeof(LinkedList *));

            LinkedList_initList(container->customDisplayListStack);

            gSPBranchList(&container->displayList, ZGlobalObj_getGlobalGfxPtr(this->vanillaObjId, vanillaDisplayList));

            return true;
        }
    }

    return false;
}

void refreshContainerDL(ZProxy *this, ZProxy_ProxyContainer *c) {
    Gfx *dl = LinkedListNode_getData(LinkedList_end(c->customDisplayListStack));
    recomp_printf("refreshContainerDL custom DL: 0x%X\n", dl);
    if (!dl) {
        dl = ZGlobalObj_getGlobalGfxPtr(this->vanillaObjId, c->vanillaDisplayList);
    }

    gSPBranchList(&c->displayList, dl);
}

bool ZProxy_addCustomDisplayList(ZProxy *this, Gfx *vanillaDisplayList, Gfx customDisplayList[]) {
    ZProxy_reserveContainer(this, vanillaDisplayList);

    if (!customDisplayList || !vanillaDisplayList) {
        return false;
    }

    uintptr_t vanilla = (uintptr_t)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(this->vanillaDLToCustomDLMap, vanilla);

    if (container) {
        LinkedList_addBack(container->customDisplayListStack, customDisplayList);
        refreshContainerDL(this, container);
        return true;
    }

    return false;
}

bool ZProxy_removeCustomDisplayList(ZProxy *this, Gfx *vanillaDisplayList, Gfx customDisplayList[]) {
    ZProxy_reserveContainer(this, vanillaDisplayList);

    if (!customDisplayList || !vanillaDisplayList) {
        return false;
    }

    uintptr_t vanilla = (uintptr_t)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(this->vanillaDLToCustomDLMap, vanilla);

    if (container && LinkedList_removeData(container->customDisplayListStack, customDisplayList)) {
        refreshContainerDL(this, container);
        return true;
    }

    return false;
}
