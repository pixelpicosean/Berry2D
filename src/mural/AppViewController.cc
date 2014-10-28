#include "AppViewController.h"

// #include <thread>

namespace mural
{

AppViewController::~AppViewController()
{
    delete this->view;
}

void AppViewController::initWithScriptAtPath(const char *path, int width, int height, const char *title)
{
    this->view = new JavaScriptView(width, height, title);
    this->view->loadScriptAtPath(path);
    // Test MuOperationQueue
    // this->view->backgroundQueue.addOperation([] {
    //     printf("[Test]: Operation 'A' starts running, this should be long (1000ms)\n");
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //     printf("[Test]: Operation 'A' finished\n");
    // });
    // this->view->backgroundQueue.addOperation([] {
    //     printf("[Test]: Operation 'B' just fired\n");
    // });
    this->view->startRunning();
}

}
