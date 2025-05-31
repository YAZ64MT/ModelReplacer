#include "global.h"
#include "recomputils.h"
#include "zproxy_manager.h"
#include "globalobjects_api.h"

RECOMP_EXPORT bool ZModelReplacer_addCustomDL(ObjectId id, Gfx *vanillaDL, Gfx *customDL) {
    return ZProxyManager_addCustomDisplayList(id, vanillaDL, customDL);
}

RECOMP_EXPORT bool ZModelReplacer_removeCustomDL(ObjectId id, Gfx *vanillaDL, Gfx *customDL) {
    return ZProxyManager_removeCustomDisplayList(id, vanillaDL, customDL);
}

RECOMP_EXPORT bool ZModelReplacer_preregisterDL(ObjectId id, Gfx *vanillaDL) {
    return ZProxyManager_reserveVanillaDisplayList(id, vanillaDL);
}

RECOMP_DECLARE_EVENT(ZModelReplacer_onReady());

RECOMP_CALLBACK(YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME, ZGlobalObj_onReady)
void initZProxyManagerOnce() {
    ZProxyManager_initManager();

    ZProxyManager_enableModelInject();

    ZModelReplacer_onReady();
}
