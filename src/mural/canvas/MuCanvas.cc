#include "MuCanvas.h"
#include "../JavaScriptView.h"

namespace mural
{

MuCanvas::MuCanvas(JavaScriptView *scriptView):
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

MuCanvas::~MuCanvas()
{
    if (this->isScreenCanvas) {
        this->scriptView->hasScreenCanvas = false;
    }
    /* Delete style object */
}

float MuCanvas::getStyleLeft() { return 0.0f; }
float MuCanvas::getStyleTop() { return 0.0f; }
float MuCanvas::getStyleWidth() { return 0.0f; }
float MuCanvas::getStyleHeight() { return 0.0f; }

MuTexture *MuCanvas::getTexture() { return nullptr; }

}
