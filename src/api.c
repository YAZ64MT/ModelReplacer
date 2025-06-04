#include "global.h"
#include "recomputils.h"
#include "recompdata.h"
#include "zproxy_manager.h"
#include "globalobjects_api.h"

RECOMP_EXPORT ZModelReplacerHandle ZModelReplacer_createReplacer(ObjectId id, Gfx *vanillaDL) {
    return ZProxyManager_createDisplayListHandle(id, vanillaDL);

}

RECOMP_EXPORT bool ZModelReplacer_destroyReplacer(ZModelReplacerHandle handle) {
    return ZProxyManager_destroyDisplayListHandle(handle);
}

RECOMP_EXPORT bool ZModelReplacer_setReplacerModel(ZModelReplacerHandle handle, Gfx *customDL) {
    return ZProxyManager_setDisplayList(handle, customDL);
}

RECOMP_EXPORT bool ZModelReplacer_pushReplacer(ZModelReplacerHandle handle) {
    return ZProxyManager_pushDisplayList(handle);
}

RECOMP_EXPORT bool ZModelReplacer_removeReplacer(ZModelReplacerHandle handle) {
    return ZProxyManager_removeDisplayList(handle);
}

RECOMP_DECLARE_EVENT(ZModelReplacer_onReady());

RECOMP_CALLBACK(YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME, ZGlobalObj_onReady)
void initZProxyManagerOnce() {
    ZProxyManager_initManager();

    ZProxyManager_enableModelInject();

    ZModelReplacer_onReady();
}
