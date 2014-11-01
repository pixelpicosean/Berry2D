#pragma once

#include "../duktape/duktape.h"

namespace mural
{

#define MURAL_JS_NAMESPACE "__MURAL__"
#define DATA_POINTER_NAME "__nativePointer__"

template <class T>
T *getNativePointer(duk_context *ctx)
{
    duk_push_this(ctx); /* this */
    duk_get_prop_string(ctx, -1, DATA_POINTER_NAME); /* this pointer */
    T *t = static_cast<T*>(duk_to_pointer(ctx, -1));
    duk_pop_n(ctx, 2);

    return t;
}

template <class T>
void setNativePointer(duk_context *ctx, T *inst)
{
    duk_push_this(ctx); /* this */
    duk_push_pointer(ctx, inst); /* this, inst */
    duk_put_prop_string(ctx, -2, DATA_POINTER_NAME); /* this */
    duk_pop(ctx);
}

void jsRefSetup(duk_context *ctx);
int jsRef(duk_context *ctx);
void jsPushRef(duk_context *ctx, int ref);
void jsUnref(duk_context *ctx, int ref);

/**
 * Macros for binding classes
 *
 * #Usage:
 *   // Start
 *   MU_START_BINDING(class)
 *   // Create prototype and bind numbers and methods
 *   MU_BIND_METHODS_AND_NUMBERS(class)
 *   // Bind getter/setter
 *   MU_BIND_SET(class, prop)
 *   ...
 *   // Finish
 *   MU_FINISH_BINDING(class)
 */

/**
 * Stack after this call:
 *   global, __MURAL__
 */
#define MU_START_BINDING(CLASS) \
    duk_push_global_object(ctx); \
    duk_get_prop_string(ctx, -1, MURAL_JS_NAMESPACE);

/**
 * Require stack items:
 *   global, __MURAL__, constructor
 *
 * Stack after this call:
 *   ... UNKNOWN ...
 */
#define MU_FINISH_BINDING(CLASS) \
    duk_put_prop_string(ctx, -2, #CLASS); \
    duk_pop_2(ctx);

/**
 * Require stack items:
 *   global, __MURAL__
 *
 * Stack after this call:
 *   global, __MURAL__, constructor, prototype
 */
#define MU_BIND_METHODS_AND_NUMBERS(CLASS) \
    duk_push_c_function(ctx, w_##CLASS##_constructor, 0); \
    duk_push_object(ctx); \
    duk_put_number_list(ctx, -1, numbers_of_##CLASS); \
    duk_put_function_list(ctx, -1, methods_of_##CLASS); \
    duk_put_prop_string(ctx, -2, "prototype");

#define MU_BIND_SET(CLASS, NAME) \
    duk_eval_string(ctx, "__MURAL__.__defineSetter__"); \
    duk_get_prop_string(ctx, -2, "prototype"); \
    duk_push_string(ctx, #NAME); \
    duk_push_c_function(ctx, w_##CLASS##_prototype_set_##NAME, 0); \
    duk_call(ctx, 3); \
    duk_pop(ctx);

#define MU_BIND_GET(CLASS, NAME) \
    duk_eval_string(ctx, "__MURAL__.__defineGetter__"); \
    duk_get_prop_string(ctx, -2, "prototype"); \
    duk_push_string(ctx, #NAME); \
    duk_push_c_function(ctx, w_##CLASS##_prototype_get_##NAME, 0); \
    duk_call(ctx, 3); \
    duk_pop(ctx);

#define MU_BIND_SET_GET(CLASS, NAME) \
    duk_eval_string(ctx, "__MURAL__.__defineAccessor__"); \
    duk_get_prop_string(ctx, -2, "prototype"); \
    duk_push_string(ctx, #NAME); \
    duk_push_c_function(ctx, w_##CLASS##_prototype_set_##NAME, 1); \
    duk_push_c_function(ctx, w_##CLASS##_prototype_get_##NAME, 0); \
    duk_call(ctx, 4); \
    duk_pop(ctx);
}
