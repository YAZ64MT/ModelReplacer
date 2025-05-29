#ifndef __ZPROXY_MANAGER_H__
#define __ZPROXY_MANAGER_H__

#include "global.h"

void ZProxyManager_initManager();

bool ZProxyManager_registerZProxy(ObjectId id);

bool ZProxyManager_unregisterZProxy(ObjectId id);

bool ZProxyManager_setCustomDisplayList(ObjectId id, Gfx *vanillaDL, Gfx *customDL);

bool ZProxyManager_removeCustomDisplayList(ObjectId id, Gfx *vanillaDL);

bool ZProxyManager_reserveVanillaDisplayList(ObjectId id, Gfx *vanillaDL);

void ZProxyManager_enableModelInject();

void ZProxyManager_disableModelInject();

#endif
