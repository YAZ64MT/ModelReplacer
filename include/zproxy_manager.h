#ifndef __ZPROXY_MANAGER_H__
#define __ZPROXY_MANAGER_H__

#include "global.h"
#include "zproxy.h"

void ZProxyManager_initManager();

bool ZProxyManager_registerZProxy(ObjectId id);

bool ZProxyManager_unregisterZProxy(ObjectId id);

ModelReplacerHandle ZProxyManager_createDisplayListHandle(ObjectId id, Gfx *vanillaDL);

bool ZProxyManager_setDisplayList(ModelReplacerHandle handle, Gfx *customDL);

bool ZProxyManager_pushDisplayList(ModelReplacerHandle handle);

void ZProxyManager_enableModelInject();

void ZProxyManager_disableModelInject();

#endif
