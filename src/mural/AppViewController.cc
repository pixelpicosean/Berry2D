#include "AppViewController.h"

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
    this->view->startRunning();
}

}
