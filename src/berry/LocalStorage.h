#ifndef BERRY_BERRY_LOCAL_STORAGE_H
#define BERRY_BERRY_LOCAL_STORAGE_H

#include "../common/common.h"
#include "../common/StringUtil.h"
#include "../common/BindUtil.h"

namespace berry
{

class LocalStorage
{
    String storageFile;
    String data;
public:
    LocalStorage();
    ~LocalStorage() {}

    const String getData();
    void setData(const String& value);
    void clear();
};

// Binding
int w_LocalStorage_constructor(duk_context *ctx);
int w_LocalStorage_getItem(duk_context *ctx);
int w_LocalStorage_setItem(duk_context *ctx);
int w_LocalStorage_removeItem(duk_context *ctx);
int w_LocalStorage_clear(duk_context *ctx);
int w_LocalStorage_key(duk_context *ctx);

void js_register_LocalStorage(duk_context *ctx);

}

#endif
