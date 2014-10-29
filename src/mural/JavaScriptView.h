#pragma once

#include <GLFW/glfw3.h>

#include "../common/BindUtil.h"

#define MURAL_VERSION "0.1"
#define MURAL_SHIM_JS "src/scripts/shim.js"
#define MURAL_BOOT_JS "src/scripts/mural.js"

namespace mural
{

class JavaScriptView
{
    duk_context *jsGlobalContext;
    GLFWwindow *window;

    int width;
    int height;

    const char *lang;

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
    void defineProperties();
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

#define theInput mural::Input::getInstance()

}
