#include "JavaScriptView.h"

namespace berry {

JavaScriptView::JavaScriptView()
{
    this->jsGlobalContext = duk_create_heap_default();
}
JavaScriptView::~JavaScriptView()
{
    duk_destroy_heap(this->jsGlobalContext);
}

void JavaScriptView::loadScriptAtPath(const char *path)
{
    duk_eval_file(this->jsGlobalContext, path);
}

}
