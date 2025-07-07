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
} ZProxy;

void ZProxy_initZProxy(ZProxy *zp, ObjectId id);

void ZProxy_destroyZProxy(ZProxy *zp);

bool ZProxy_refresh(ZProxy *zp, ModelReplacerHandle handle);

bool ZProxy_addCustomDisplayList(ZProxy *zp, ModelReplacerHandle handle);

bool ZProxy_reserveContainer(ZProxy *zp, Gfx *vanillaDisplayList);

#endif
