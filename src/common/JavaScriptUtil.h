#pragma once

#include "BindUtil.h"
#include <cstdio>

int printNoReturn(duk_context *ctx)
{
    printf("%s", duk_to_string(ctx, 0));
    return 0;
}

void js_register_utils(duk_context *ctx)
{
    duk_push_global_object(ctx);
    duk_push_c_function(ctx, printNoReturn, 1);
    duk_put_prop_string(ctx, -2, "printNoReturn");
    duk_pop(ctx);
}
