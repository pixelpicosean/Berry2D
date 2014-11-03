#pragma once

#include "../common/common.h"
#include "../common/StringUtil.h"
#include "../common/BindUtil.h"

namespace mural
{

class MuLocalStorage
{
    String storageFile;
    String data;
public:
    MuLocalStorage();
    ~MuLocalStorage() {}

    const String getData();
    void setData(const String& value);
    void clear();
};

// Binding
int w_LocalStorage_constructor(duk_context *ctx);
int w_LocalStorage_prototype_getItem(duk_context *ctx);
int w_LocalStorage_prototype_setItem(duk_context *ctx);
int w_LocalStorage_prototype_removeItem(duk_context *ctx);
int w_LocalStorage_prototype_clear(duk_context *ctx);
int w_LocalStorage_prototype_key(duk_context *ctx);

void js_register_LocalStorage(duk_context *ctx);

}
