#ifndef BERRY_BERRY_JAVASCRIPTVIEW_H
#define BERRY_BERRY_JAVASCRIPTVIEW_H

#include "../duktape/duktape.h"

#define BERRY_VERSION "0.1"
#define BERRY_BOOT_JS "../scripts/ejecta.js"

namespace berry {

class JavaScriptView
{
    duk_context *jsGlobalContext;

public:
    JavaScriptView();
    ~JavaScriptView();

    void loadScriptAtPath(const char *path);
};

}

#endif
