#ifndef __MODEL_REPLACER_API__
#define __MODEL_REPLACER_API__

#include "global.h"
#include "modding.h"

#define YAZMT_Z64_MODEL_REPLACER_MOD_NAME "yazmt_mm_modelreplacer"

// Replaces the display list (DL) at the passed in segmented pointer in the given object id.
//
// Returns true if model replaced, false otherwise.
//
// Usage Example:
// ZModelReplacer_setCustomDL(OBJECT_LINK_CHILD, gLinkHumanGreatFairysSwordDL, myCoolSwordModel)
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_setCustomDL(ObjectId id, Gfx *vanillaDL, Gfx *customDL));

// Restores the model to its vanilla appearance.
//
// Returns true if reset ocurred, false otherwise.
//
// Usage Example:
// ZModelReplacer_removeCustomDL(OBJECT_LINK_CHILD, gLinkHumanGreatFairysSwordDL)
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_removeCustomDL(ObjectId id, Gfx *vanillaDL));

// If a model has not been replaced during this play session, the object will need to be reloaded before
// its replacement shows up.
//
// This function is useful if you want an object to have its vanilla appearance at the start but want
// any replacements to be immediately visible.
//
// Returns true on sucessful pre-registration, false otherwise.
//
// Usage Example:
// ZModelReplacer_preregisterDL(OBJECT_LINK_CHILD, gLinkHumanGreatFairysSwordDL)
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_preregisterDL(ObjectId id, Gfx *vanillaDL));

#endif
