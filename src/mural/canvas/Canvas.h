#pragma once

#include "CanvasContext.h"
#include "Texture.h"

namespace mural
{

enum MuCanvasContextMode {
    kMuCanvasContextModeInvalid = 0,
    kMuCanvasContextMode2D,
    kMuCanvasContextModeWebGL
};

class JavaScriptView;
class Canvas
{
    JavaScriptView *scriptView;

    CanvasContext *renderingContext;
    MuCanvasContextMode contextMode;
    short width, height;

    /* Style object */
    /* Style */

    bool isScreenCanvas;
    bool useRetinaResolution;

    bool msaaEnabled;
    int msaaSamples;
public:
    int jsObjectRef;

    float getStyleLeft();
    float getStyleTop();
    float getStyleWidth();
    float getStyleHeight();

    Texture *getTexture();

    Canvas(JavaScriptView *scriptView);
    ~Canvas();
};

}
