#include "AppViewController.h"

namespace berry {

AppViewController::AppViewController() {}
AppViewController::~AppViewController()
{
    delete this->view;
}

void AppViewController::initWithScriptAtPath(const char *path)
{
    this->path = path;

    this->loadView();
    this->view->loadScriptAtPath(path);
}

void AppViewController::loadView()
{
    this->view = new JavaScriptView();
}

}
