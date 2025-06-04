#ifndef __ZPROXY_H__
#define __ZPROXY_H__

#include "global.h"
#include "recompdata.h"
#include "z64object.h"
#include "linkedlist.h"

typedef collection_key_t ZModelReplacerHandle;

typedef struct {
    ObjectId id;
    Gfx* vanillaDL;
    Gfx *customDL;
} ZProxy_CustomDisplayListEntry;

typedef struct {
    LinkedList* customDisplayListStack;
    Gfx* vanillaDisplayList;
    Gfx displayList;
} ZProxy_ProxyContainer;

typedef struct {
    ObjectId vanillaObjId;
    U32MemoryHashmapHandle vanillaDLToCustomDLMap;
    LinkedList* vanillaDisplayLists;
    U32MemoryHashmapHandle customDisplayLists;
} ZProxy;

void ZProxy_initZProxy(ZProxy *zProxy, ObjectId id, U32MemoryHashmapHandle custocustomDisplayListMap);

void ZProxy_destroyZProxy(ZProxy *zProxy);

bool ZProxy_refresh(ZProxy *this, ZModelReplacerHandle handle);

bool ZProxy_addCustomDisplayList(ZProxy *this, ZModelReplacerHandle handle);

bool ZProxy_removeCustomDisplayList(ZProxy *this, ZModelReplacerHandle handle);

bool ZProxy_reserveContainer(ZProxy *zProxy, Gfx *vanillaDisplayList);

#endif
