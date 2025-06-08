#include "global.h"
#include "recomputils.h"
#include "recompdata.h"
#include "zproxy_manager.h"
#include "globalobjects_api.h"

static bool sIsRegistrationEnabled = false;

RECOMP_EXPORT ZModelReplacerHandle ZModelReplacer_registerReplacer(ObjectId id, Gfx *vanillaDL, Gfx *customDL) {
    if (!sIsRegistrationEnabled) {
        const char *REGISTER_OUTSIDE_OF_CALLBACK_WARNING =
            "ZModelReplacer: Called ZModelReplacer_registerReplacer outside of ZModelReplacer_onRegisterReplacers callback! "
            "Please move your call to ZModelReplacer_registerReplacer into a callback function for this event.";
        recomp_printf(REGISTER_OUTSIDE_OF_CALLBACK_WARNING);
        return 0;
    }

    ZModelReplacerHandle h = ZProxyManager_createDisplayListHandle(id, vanillaDL);
    ZProxyManager_setDisplayList(h, customDL);
    ZProxyManager_pushDisplayList(h);
    return h;
}

RECOMP_EXPORT bool ZModelReplacer_setReplacerModel(ZModelReplacerHandle handle, Gfx *customDL) {
    return ZProxyManager_setDisplayList(handle, customDL);
}

RECOMP_EXPORT bool ZModelReplacer_removeReplacerModel(ZModelReplacerHandle handle) {
    return ZProxyManager_setDisplayList(handle, NULL);
}

RECOMP_DECLARE_EVENT(ZModelReplacer_onRegisterReplacers());
RECOMP_DECLARE_EVENT(ZModelReplacer_onReady());

RECOMP_CALLBACK(YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME, ZGlobalObj_onReady)
void initZProxyManagerOnce() {
    ZProxyManager_initManager();

    ZProxyManager_enableModelInject();

    sIsRegistrationEnabled = true;
    ZModelReplacer_onRegisterReplacers();
    sIsRegistrationEnabled = false;

    ZModelReplacer_onReady();
}
