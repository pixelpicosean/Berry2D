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

}
