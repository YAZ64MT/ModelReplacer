#ifndef __MODEL_REPLACER_API__
#define __MODEL_REPLACER_API__

#include "global.h"
#include "modding.h"

typedef unsigned long ZModelReplacerHandle;

#define YAZMT_Z64_MODEL_REPLACER_MOD_NAME "yazmt_mm_modelreplacer"

// Creates a Replacer and returns a handle to it
//
// Replacer handles are how your mod will interface with ModelReplacer, so hold onto it.
//
// Each Replacer is specific to a particular object and display list within that object, so
// if you want to replace multiple display lists within an object, you will need a separate
// Replacer for each display list.
//
// The most recently created Replacer is given priority over previously created Replacers for
// a given vanilla object's display List. Only the Replacer with the highest priority and a
// non-NULL custom display list will be drawn.
//
// Setting a NULL display list is a good way to reserve a slot. If you want to replace a model
// but have it retain its vanilla appearance at first (or via config), consider creating a
// Replacer with a NULL customDL and using ZModelReplacer_setReplacerModel and
// ZModelReplacer_removeReplacerModel as necessary on the handle returned by this function.
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, ZModelReplacerHandle ZModelReplacer_createReplacer(ObjectId id, Gfx *vanillaDL, Gfx* customDL));

// Destroys a Replacer
//
// Returns true if Replacer was destroyed, false if nothing happened.
//
// If you know for a fact you will never need a handle again, destroy it to avoid a memory leak.
//
// Though, if you just want to temporarily remove a model and add it back later, consider using
// ZModelReplacer_removeReplacerModel and ZModelReplacer_setReplacerModel instead.
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_destroyReplacer(ZModelReplacerHandle handle));

// Sets the display list inside the Replacer
//
// Returns true if display list successfully set, false otherwise.
//
// You may call it to switch out the display list at any time, but if your Replacer is not
// at the top of the stack, it will not appear. The change will be reflected if your model
// ever returns to the top of the stack, though.
//
// A NULL custom display list will cause this Replacer to be skipped but will preserve its
// position in the stack if a non-NULL display list is ever set.
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_setReplacerModel(ZModelReplacerHandle handle, Gfx *customDL));

// Unsets a Replacer's model.
//
// Returns true if model removal ocurred, false if nothing happened.
//
// Removes the model of a Replacer but does not destroy it. This allows the model below it
// in the priority list to be drawn.
//
// This has the same effect as calling ZModelReplacer_setReplacerModel with customDL set to
// NULL.
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_removeReplacerModel(ZModelReplacerHandle handle));

#endif
