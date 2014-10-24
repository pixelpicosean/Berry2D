#include "LocalStorage.h"

#include "../common/FileUtil.h"

namespace berry
{

LocalStorage::LocalStorage():
    storageFile("save-data.dat")
{
    // this->storageFile += getExeName();
    printf("LS file: %s\n", this->storageFile);
}

const char *LocalStorage::getItem(const char *key)
{
    return "Hello";
}
void LocalStorage::setItem(const char *key, const char *value) {}
void LocalStorage::removeItem(const char *key) {}
void LocalStorage::clear() {}
void LocalStorage::key() {}

const duk_function_list_entry methods_of_LocalStorage[] = {
    { "getItem",    w_LocalStorage_getItem,     1 },
    { "setItem",    w_LocalStorage_setItem,     1 },
    { "removeItem", w_LocalStorage_removeItem,  1 },
    { "clear",      w_LocalStorage_clear,       0 },
    { "key",        w_LocalStorage_key,         1 }
};

// Binding
int w_LocalStorage_constructor(duk_context *ctx)
{
    LocalStorage *inst = new LocalStorage();
    setNativePointer(ctx, inst);

    return 0;
}
int w_LocalStorage_getItem(duk_context *ctx)
{
    const char *key = duk_to_string(ctx, 0); /* arguments[0] */
    LocalStorage *inst = getNativePointer<LocalStorage>(ctx);
    duk_push_string(ctx, inst->getItem(key));

    return 1;
}
int w_LocalStorage_setItem(duk_context *ctx)
{
    return 0;
}
int w_LocalStorage_removeItem(duk_context *ctx)
{
    return 0;
}
int w_LocalStorage_clear(duk_context *ctx)
{
    return 0;
}
int w_LocalStorage_key(duk_context *ctx)
{
    return 1;
}

void js_register_LocalStorage(duk_context *ctx)
{
    duk_push_global_object(ctx); /* global */
    duk_get_prop_string(ctx, -1, BERRY_JS_NAMESPACE); /* global, __BERRY__ */

    duk_push_c_function(ctx, w_LocalStorage_constructor, 0); /* global, __BERRY__, constructor */
    duk_push_object(ctx); /* global, __BERRY__, constructor, prototype */
    duk_put_function_list(ctx, -1, methods_of_LocalStorage);
    duk_put_prop_string(ctx, -2, "prototype"); /* global, __BERRY__, constructor */

    duk_put_prop_string(ctx, -2, "LocalStorage"); /* global, __BERRY__ */
    duk_pop_2(ctx);
}

}
