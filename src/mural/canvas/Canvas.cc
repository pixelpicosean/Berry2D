#include "Canvas.h"
#include "../JavaScriptView.h"

namespace mural
{

Canvas::Canvas(JavaScriptView *scriptView):
    scriptView(scriptView),
    renderingContext(nullptr),
    contextMode(kMuCanvasContextModeInvalid),
    width(0), height(0),
    isScreenCanvas(false),
    useRetinaResolution(false),
    msaaEnabled(false),
    msaaSamples(2),
    jsObjectRef(0)
{
    if (!this->scriptView->hasScreenCanvas) {
        this->isScreenCanvas = true;
        this->scriptView->hasScreenCanvas = true;
    }

    this->width = this->scriptView->width;
    this->height = this->scriptView->height;
}

Canvas::~Canvas()
{
    if (this->isScreenCanvas) {
        this->scriptView->hasScreenCanvas = false;
    }
    /* Delete style object */
}

float Canvas::getStyleLeft() { return 0.0f; }
float Canvas::getStyleTop() { return 0.0f; }
float Canvas::getStyleWidth() { return 0.0f; }
float Canvas::getStyleHeight() { return 0.0f; }

Texture *Canvas::getTexture() { return nullptr; }

}
