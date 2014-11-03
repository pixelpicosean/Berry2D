#include "JavaScriptView.h"

#include "../common/StringUtil.h"
#include "../common/MuOperationQueue.h"

#include "../common/JavaScriptUtil.h"
#include "MuLocalStorage.h"
#include "canvas/MuImage.h"

#if defined(WIN32)
#  define OS_NAME "Windows"
#  define OS_AGENT "(Windows; OS ?)"
#elif defined(__APPLE__)
#  define OS_NAME "MacOS X"
#  define OS_AGENT "(MacOS X; OS 10.10)"
#elif defined(__linux__)
#  define OS_NAME "Linux"
#  define OS_AGENT "(Linux; OS ?)"
#endif

namespace mural
{

JavaScriptView::JavaScriptView(int width, int height, const char *title):
    jsGlobalContext(nullptr),
    window(nullptr),
    lang("en"),
    width(width),
    height(height),
    hasScreenCanvas(false)
{
    // Create the global JS context
    this->jsGlobalContext = duk_create_heap_default();

    // Setup global namespace for Berry built-in
    duk_push_global_object(this->jsGlobalContext); /* global */
    duk_push_object(this->jsGlobalContext);
    duk_put_prop_string(this->jsGlobalContext, -2, MURAL_JS_NAMESPACE);
    duk_pop(this->jsGlobalContext);

    // To make JavaScript objects accessible in C++
    jsRefSetup(this->jsGlobalContext);

    // Load shim for duktape
    this->loadScriptAtPath(MURAL_SHIM_JS);

    this->defineProperties();

    // Create a window
    if (!glfwInit()) {
        printf("[JavaScriptView]: unable to init GLFW\n");
    }
    this->window = glfwCreateWindow(width, height, title, /*glfwGetPrimaryMonitor()*/NULL, NULL);
    if (!this->window) {
        printf("[JavaScriptView]: unable to create window\n");
    }

    // Setup OpenGL context
    glfwMakeContextCurrent(this->window);

    #if !MURAL_MOBILE
    #else
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, -1.0f);
    #endif
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    // Setup event callbacks
    theInput.setDelegate(this);
    glfwSetKeyCallback(this->window, &Input::keyCallback);

    // Register built-in
    js_register_utils(this->jsGlobalContext);
    js_register_LocalStorage(this->jsGlobalContext);
    js_register_Image(this->jsGlobalContext);

    // Load boot script
    this->loadScriptAtPath(MURAL_BOOT_JS);
}
JavaScriptView::~JavaScriptView()
{
    // OpenGL reset
    glDisable(GL_TEXTURE_2D);

    // Cleanup JavaScript engine
    duk_destroy_heap(this->jsGlobalContext);

    // Cleanup GLFW
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void JavaScriptView::loadScriptAtPath(const char *path)
{
    duk_eval_file(this->jsGlobalContext, path);
    duk_pop(this->jsGlobalContext);
}

void JavaScriptView::startRunning()
{
    while (!glfwWindowShouldClose(this->window)) {
        this->run();
    }
}

void JavaScriptView::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void JavaScriptView::run()
{
    float ratio;
    int width, height;

    glfwGetFramebufferSize(this->window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glRotatef((float) glfwGetTime() * 50.0f, 0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-0.6f, -0.4f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.6f, -0.4f, 0.0f);
        glColor3f(0.0f, 0.f, 1.0f);
        glVertex3f(0.f, 0.6f, 0.0f);
    glEnd();

    // Block operations
    MuOperationQueue::defaultQueue().doBlockOperations();

    // RAF
    duk_push_global_object(this->jsGlobalContext);
    duk_get_prop_string(this->jsGlobalContext, -1, MURAL_JS_NAMESPACE); /* __MURAL__ */
    duk_get_prop_string(this->jsGlobalContext, -1, "tickAnimFrame");
    duk_call(this->jsGlobalContext, 0);
    duk_pop_n(this->jsGlobalContext, 3);

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

void JavaScriptView::defineProperties()
{
    // Set properties to __MURAL__
    duk_push_global_object(this->jsGlobalContext);
    duk_get_prop_string(this->jsGlobalContext, -1, MURAL_JS_NAMESPACE); /* __MURAL__ */

    // - screenWidth
    duk_push_int(this->jsGlobalContext, width);
    duk_put_prop_string(this->jsGlobalContext, -2, "screenWidth");
    // - screenHeight
    duk_push_int(this->jsGlobalContext, height);
    duk_put_prop_string(this->jsGlobalContext, -2, "screenHeight");

    // - language
    duk_push_string(this->jsGlobalContext, this->lang);
    duk_put_prop_string(this->jsGlobalContext, -2, "language");
    // - userAgent
    std::string userAgent("Ejecta/");
    userAgent += MURAL_VERSION;
    userAgent += OS_AGENT;
    duk_push_string(this->jsGlobalContext, userAgent.c_str());
    duk_put_prop_string(this->jsGlobalContext, -2, "userAgent");
    // - app version
    duk_push_string(this->jsGlobalContext, "0.1.0");
    duk_put_prop_string(this->jsGlobalContext, -2, "appVersion");
    // - platform
    duk_push_string(this->jsGlobalContext, OS_NAME);
    duk_put_prop_string(this->jsGlobalContext, -2, "platform");

    // Leave global scope
    duk_pop_2(this->jsGlobalContext);
}

}
