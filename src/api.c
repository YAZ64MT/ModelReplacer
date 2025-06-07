#include "global.h"
#include "recomputils.h"
#include "recompdata.h"
#include "zproxy_manager.h"
#include "globalobjects_api.h"

RECOMP_EXPORT ZModelReplacerHandle ZModelReplacer_createReplacer(ObjectId id, Gfx *vanillaDL, Gfx *customDL) {
    ZModelReplacerHandle h = ZProxyManager_createDisplayListHandle(id, vanillaDL);
    ZProxyManager_setDisplayList(h, customDL);
    ZProxyManager_pushDisplayList(h);
    return h;
}

RECOMP_EXPORT bool ZModelReplacer_destroyReplacer(ZModelReplacerHandle handle) {
    return ZProxyManager_destroyDisplayListHandle(handle);
}

RECOMP_EXPORT bool ZModelReplacer_setReplacerModel(ZModelReplacerHandle handle, Gfx *customDL) {
    return ZProxyManager_setDisplayList(handle, customDL);
}

RECOMP_EXPORT bool ZModelReplacer_removeReplacerModel(ZModelReplacerHandle handle) {
    return ZProxyManager_setDisplayList(handle, NULL);
}

RECOMP_DECLARE_EVENT(ZModelReplacer_onReady());

RECOMP_CALLBACK(YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME, ZGlobalObj_onReady)
void initZProxyManagerOnce() {
    ZProxyManager_initManager();

    ZProxyManager_enableModelInject();

    ZModelReplacer_onReady();
}
