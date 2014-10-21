#ifndef BERRY_BERRY_APPVIEWCONTROLLER_H
#define BERRY_BERRY_APPVIEWCONTROLLER_H

#include "JavaScriptView.h"

namespace berry{

class AppViewController
{
    const char *path;

    JavaScriptView *view;

    void loadView();

public:
    AppViewController();
    ~AppViewController();

    void initWithScriptAtPath(const char *path);
};

}

#endif
