#include <cstdio>
#include "duktape/duktape.h"

int main(int argc, char const *argv[])
{
    duk_context *ctx = duk_create_heap_default();

    duk_eval_file(ctx, "src/scripts/ejecta.js");
    duk_eval_file(ctx, "index.js");

    duk_destroy_heap(ctx);

    return 0;
}
