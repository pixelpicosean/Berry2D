#ifndef BERRY_BERRY_APPVIEWCONTROLLER_H
#define BERRY_BERRY_APPVIEWCONTROLLER_H

#include "JavaScriptView.h"

namespace berry{

class AppViewController
{
public:
    JavaScriptView *view;

public:
    void initWithScriptAtPath(const char *path, int width = 640, int height = 480, const char *title = "Berry2D");

    static AppViewController& getInstance()
    {
        static AppViewController instance;
        return instance;
    }

private:
    AppViewController() {}
    AppViewController(AppViewController const&) {}
    void operator=(AppViewController const&) {}

    ~AppViewController();
};

}

#define theAppController berry::AppViewController::getInstance()

#endif
