#ifndef BERRY_BERRY_LOCAL_STORAGE_H
#define BERRY_BERRY_LOCAL_STORAGE_H

#include "../common/common.h"
#include "../common/StringUtil.h"
#include "../common/BindUtil.h"

namespace berry
{

class LocalStorage
{
public:
    LocalStorage();
    ~LocalStorage();

    String getItem(const String& key);
    void setItem(const String& key, const String& value);
    void removeItem(const String& key);
    void clear();
    void key();
};

}

// Binding
int w_LocalStorage_constructor(duk_context *ctx);
int w_LocalStorage_getItem(duk_context *ctx);
int w_LocalStorage_setItem(duk_context *ctx);
int w_LocalStorage_removeItem(duk_context *ctx);
int w_LocalStorage_clear(duk_context *ctx);
int w_LocalStorage_key(duk_context *ctx);

void js_register_LocalStorage(duk_context *ctx);

#endif
