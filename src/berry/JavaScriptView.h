#ifndef BERRY_BERRY_JAVASCRIPTVIEW_H
#define BERRY_BERRY_JAVASCRIPTVIEW_H

#include "../duktape/duktape.h"
#include <GLFW/glfw3.h>

#define BERRY_VERSION "0.1"
#define BERRY_BOOT_JS "src/scripts/berry.js"

namespace berry {

class JavaScriptView
{
    duk_context *jsGlobalContext;
    GLFWwindow *window;

    int width;
    int height;

public:
    JavaScriptView(int width, int height, const char *title);
    ~JavaScriptView();

    // Methods
    void loadScriptAtPath(const char *path);
    void startRunning();

    // Event callbacks
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    void run();
};

class Input
{
    JavaScriptView *delegate;

public:
    static Input& getInstance()
    {
        static Input instance;
        return instance;
    }
    void setDelegate(JavaScriptView *delegate)
    {
        this->delegate = delegate;
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Input::getInstance().delegate->keyCallback(window, key, scancode, action, mods);
    }

private:
    Input() {}
    Input(Input const&) {}
    void operator=(Input const&) {}
};

#define theInput berry::Input::getInstance()

}

#endif
