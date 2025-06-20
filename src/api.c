#include "global.h"
#include "recomputils.h"
#include "recompdata.h"
#include "zproxy_manager.h"
#include "globalobjects_api.h"

static bool sIsRegistrationEnabled = false;

RECOMP_EXPORT ModelReplacerHandle ModelReplacer_registerReplacer(ObjectId id, Gfx *vanillaDL, Gfx *customDL) {
    if (!sIsRegistrationEnabled) {
        const char *REGISTER_OUTSIDE_OF_CALLBACK_WARNING =
            "ModelReplacer: Called ModelReplacer_registerReplacer outside of onRegisterReplacers callback! "
            "Please move your call to ModelReplacer_registerReplacer into a callback function for this event.";
        recomp_printf(REGISTER_OUTSIDE_OF_CALLBACK_WARNING);
        return 0;
    }

    ModelReplacerHandle h = ZProxyManager_createDisplayListHandle(id, vanillaDL);
    ZProxyManager_setDisplayList(h, customDL);
    ZProxyManager_pushDisplayList(h);
    return h;
}

RECOMP_EXPORT bool ModelReplacer_setReplacerModel(ModelReplacerHandle handle, Gfx *customDL) {
    return ZProxyManager_setDisplayList(handle, customDL);
}

RECOMP_EXPORT bool ModelReplacer_removeReplacerModel(ModelReplacerHandle handle) {
    return ZProxyManager_setDisplayList(handle, NULL);
}

RECOMP_DECLARE_EVENT(onBeforeRegisterReplacers());
RECOMP_DECLARE_EVENT(onRegisterReplacers());
RECOMP_DECLARE_EVENT(onReady());

GLOBAL_OBJECTS_CALLBACK_ON_READY
void initZProxyManagerOnce() {
    ZProxyManager_initManager();

    ZProxyManager_enableModelInject();

    onBeforeRegisterReplacers();

    sIsRegistrationEnabled = true;
    onRegisterReplacers();
    sIsRegistrationEnabled = false;

    onReady();
}
