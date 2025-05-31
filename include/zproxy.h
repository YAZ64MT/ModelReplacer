#ifndef __ZPROXY_H__
#define __ZPROXY_H__

#include "global.h"
#include "recompdata.h"
#include "z64object.h"
#include "linkedlist.h"

typedef struct {
    LinkedList* customDisplayListStack;
    Gfx* vanillaDisplayList;
    Gfx displayList;
} ZProxy_ProxyContainer;

typedef struct {
    ObjectId vanillaObjId;
    U32MemoryHashmapHandle vanillaDLToCustomDLMap;
    LinkedList* vanillaDisplayLists;
} ZProxy;

void ZProxy_initZProxy(ZProxy *zProxy, ObjectId id);

void ZProxy_destroyZProxy(ZProxy *zProxy);

bool ZProxy_addCustomDisplayList(ZProxy *zProxy, Gfx *vanillaDisplayList, Gfx *customDisplayList);

bool ZProxy_removeCustomDisplayList(ZProxy *zProxy, Gfx *vanillaDisplayList, Gfx *customDisplayList);

bool ZProxy_reserveContainer(ZProxy *zProxy, Gfx *vanillaDisplayList);

#endif
