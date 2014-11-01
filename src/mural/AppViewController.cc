#include "AppViewController.h"

namespace mural
{

AppViewController::~AppViewController()
{
    delete this->view;
}

void AppViewController::initWithScript(const String& path, int width, int height, const char *title)
{
    this->view = new JavaScriptView(width, height, title);
    this->view->loadScriptAtPath(path.c_str());
    this->view->startRunning();
}

void AppViewController::initWithScripts(const StringList& paths, int width, int height, const char *title)
{
    this->view = new JavaScriptView(width, height, title);
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        this->view->loadScriptAtPath((*it).c_str());
    }
    this->view->startRunning();
}

}
