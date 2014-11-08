#pragma once

#include "2D/MuCanvasContext2DScreen.h"
#include "2D/MuCanvasContext2DTexture.h"
#include "MuTexture.h"
#include "../../common/BindUtil.h"

namespace mural
{

enum MuCanvasContextMode {
    kMuCanvasContextModeInvalid = 0,
    kMuCanvasContextMode2D,
    kMuCanvasContextModeWebGL
};

// Canvas Style
class MuCanvas;
struct MuCanvasStyle
{
    MuCanvas *binding = nullptr;
    int jsObjectRef = 0;
};
int w_CanvasStyle_constructor(duk_context *ctx);
int w_CanvasStyle_prototype_get_width(duk_context *ctx);
int w_CanvasStyle_prototype_get_height(duk_context *ctx);
int w_CanvasStyle_prototype_get_left(duk_context *ctx);
int w_CanvasStyle_prototype_get_top(duk_context *ctx);

void js_register_CanvasStyle(duk_context *ctx);

// Canvas
class JavaScriptView;
class MuCanvas
{
    JavaScriptView *scriptView;

    MuCanvasContext *renderingContext;
    MuCanvasContextMode contextMode;
    short width, height;

    MuCanvasStyle *styleObject;
    MuRect style;

    bool isScreenCanvas;
    bool useRetinaResolution;

    bool msaaEnabled;
    int msaaSamples;
public:
    int jsObjectRef = 0;

    float styleLeft = 0.0f ;
    float styleTop = 0.0f ;
    float styleWidth = 0.0f ;
    float styleHeight = 0.0f ;

    float getStyleWidth();
    void setStyleWidth(float);
    float getStyleHeight();
    void setStyleHeight(float);
    float getStyleLeft();
    void setStyleLeft(float);
    float getStyleTop();
    void setStyleTop(float);

    MuTexture *getTexture();

    MuCanvas(JavaScriptView *scriptView);
    ~MuCanvas();
};

}
