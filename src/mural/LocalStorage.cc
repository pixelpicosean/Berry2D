#include "LocalStorage.h"

#include "../common/FileUtil.h"

namespace mural
{

LocalStorage::LocalStorage():
    storageFile(getStorageDirectory() + "save-data.dat"),
    data("{}")
{
    // Make sure save directory exists
    makeDirectories(getStorageDirectory());
    this->data = readWholeFile(this->storageFile);
}

const String LocalStorage::getData()
{
    return this->data;
}
void LocalStorage::setData(const String& value)
{
    this->data = value;
    StringList data = { this->data };
    writeLinesToFile(this->storageFile, data);
}
static const StringList empty = { "{}" };
void LocalStorage::clear()
{
    writeLinesToFile(this->storageFile, empty);
}

const duk_function_list_entry methods_of_LocalStorage[] = {
    { "getItem",    w_LocalStorage_getItem,     1 },
    { "setItem",    w_LocalStorage_setItem,     2 },
    { "removeItem", w_LocalStorage_removeItem,  1 },
    { "clear",      w_LocalStorage_clear,       0 },
    { "key",        w_LocalStorage_key,         1 },
    { NULL, NULL, 0 }
};

// Binding
int w_LocalStorage_constructor(duk_context *ctx)
{
    LocalStorage *inst = new LocalStorage();
    setNativePointer(ctx, inst);

    // Create an object property to save data
    duk_push_this(ctx); /* this */
    duk_push_object(ctx); /* this, emptyObj */
    duk_put_prop_string(ctx, -2, "__MURAL_DATA__"); /* this */
    duk_pop(ctx);

    return 1;
}
int w_LocalStorage_getItem(duk_context *ctx)
{
    const char *key = duk_require_string(ctx, 0);

    duk_push_this(ctx); /* this */
    duk_get_prop_string(ctx, -1, "__MURAL_DATA__"); /* this, __MURAL_DATA__ */
    duk_get_prop_string(ctx, -1, key); /* this, __MURAL_DATA__, value */
    const char *value = duk_to_string(ctx, -1); /* this, __MURAL_DATA__, string(value) */
    duk_pop_3(ctx);

    duk_push_string(ctx, value);

    return 1;
}
int w_LocalStorage_setItem(duk_context *ctx)
{
    const char *key = duk_require_string(ctx, 0);
    // Make sure value is string
    duk_to_string(ctx, 1);
    const char *value = duk_require_string(ctx, 1);

    duk_push_this(ctx); /* this */
    duk_get_prop_string(ctx, -1, "__MURAL_DATA__"); /* this, __MURAL_DATA__ */
    duk_push_string(ctx, value); /* this, __MURAL_DATA__, value */
    duk_put_prop_string(ctx, -2, key); /* this, __MURAL_DATA__ */

    LocalStorage *inst = getNativePointer<LocalStorage>(ctx);
    duk_json_encode(ctx, -1); /* this, JSON(__MURAL_DATA__) */
    inst->setData(duk_to_string(ctx, -1)); /* this, string(JSON(__MURAL_DATA__)) */

    duk_pop_2(ctx);

    return 0;
}
int w_LocalStorage_removeItem(duk_context *ctx)
{
    const char *key = duk_require_string(ctx, 0);

    duk_push_this(ctx); /* this */
    duk_get_prop_string(ctx, -1, "__MURAL_DATA__"); /* this, __MURAL_DATA__ */
    duk_del_prop_string(ctx, -1, key); /* this, __MURAL_DATA__ */

    LocalStorage *inst = getNativePointer<LocalStorage>(ctx);
    duk_json_encode(ctx, -1); /* this, JSON(__MURAL_DATA__) */
    inst->setData(duk_to_string(ctx, -1)); /* this, string(JSON(__MURAL_DATA__)) */

    duk_pop_2(ctx);

    return 0;
}
int w_LocalStorage_clear(duk_context *ctx)
{
    duk_push_this(ctx); /* this */
    duk_push_object(ctx); /* this, emptyObj */
    duk_put_prop_string(ctx, -2, "__MURAL_DATA__"); /* this */
    duk_pop(ctx);

    LocalStorage *inst = getNativePointer<LocalStorage>(ctx);
    inst->clear();

    return 0;
}
int w_LocalStorage_key(duk_context *ctx)
{
    return 1;
}

void js_register_LocalStorage(duk_context *ctx)
{
    duk_push_global_object(ctx); /* global */
    duk_get_prop_string(ctx, -1, MURAL_JS_NAMESPACE); /* global, __MURAL__ */

    duk_push_c_function(ctx, w_LocalStorage_constructor, 0); /* global, __MURAL__, constructor */
    duk_push_object(ctx); /* global, __MURAL__, constructor, prototype */
    duk_put_function_list(ctx, -1, methods_of_LocalStorage);
    duk_put_prop_string(ctx, -2, "prototype"); /* global, __MURAL__, constructor */

    duk_put_prop_string(ctx, -2, "LocalStorage"); /* global, __MURAL__ */
    duk_pop_2(ctx);
}

}
