#include "AppViewController.h"

namespace mural
{

AppViewController::~AppViewController()
{
    delete this->view;
}

void AppViewController::initWithScript(const juce::String& path, int width, int height, const juce::String& title)
{
    this->view = new JavaScriptView(width, height, title);
    this->view->loadScriptAtPath(path.toUTF8());
    this->view->startRunning();
}

void AppViewController::initWithScripts(const juce::StringArray& paths, int width, int height, const juce::String& title)
{
    this->view = new JavaScriptView(width, height, title);
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        this->view->loadScriptAtPath((*it).toUTF8());
    }
    this->view->startRunning();
}

}
