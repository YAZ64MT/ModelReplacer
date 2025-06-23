#ifndef __ZPROXY_H__
#define __ZPROXY_H__

#include "global.h"
#include "recompdata.h"
#include "z64object.h"
#include "dynu32array.h"

typedef collection_key_t ModelReplacerHandle;

typedef struct {
    ObjectId id;
    Gfx* vanillaDL;
    Gfx *customDL;
} ZProxy_CustomDisplayListEntry;

typedef struct {
    DynamicU32Array customDisplayListStack;
    Gfx* vanillaDisplayList;
    Gfx displayList[2];
} ZProxy_ProxyContainer;

typedef struct {
    ObjectId vanillaObjId;
    U32MemoryHashmapHandle vanillaDLToCustomDLMap;
    DynamicU32Array vanillaDisplayLists;
    U32MemoryHashmapHandle customDisplayLists;
} ZProxy;

void ZProxy_initZProxy(ZProxy *zProxy, ObjectId id, U32MemoryHashmapHandle custocustomDisplayListMap);

void ZProxy_destroyZProxy(ZProxy *zProxy);

bool ZProxy_refresh(ZProxy *this, ModelReplacerHandle handle);

bool ZProxy_addCustomDisplayList(ZProxy *this, ModelReplacerHandle handle);

bool ZProxy_reserveContainer(ZProxy *zProxy, Gfx *vanillaDisplayList);

#endif
