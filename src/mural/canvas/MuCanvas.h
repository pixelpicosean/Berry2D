#pragma once

#include "MuCanvasContext.h"
#include "MuTexture.h"

namespace mural
{

enum MuCanvasContextMode {
    kMuCanvasContextModeInvalid = 0,
    kMuCanvasContextMode2D,
    kMuCanvasContextModeWebGL
};

static const char * MuLineCapNames[] = {
    "butt",
    "round",
    "square"
};

static const char * MuLineJoinNames[] = {
    "miter",
    "bevel",
    "round"
};

static const char * MuTextBaselineNames[] = {
    "alphabetic",
    "middle",
    "top",
    "hanging",
    "bottom",
    "ideographic"
};

static const char * MuTextAlignNames[] = {
    "start",
    "end",
    "left",
    "center",
    "right"
};

static const char * MuCompositeOperationNames[] = {
    "source-over",
    "lighter",
    "darker",
    "destination-out",
    "destination-over",
    "source-atop",
    "xor"
};

static const char * MuScalingModeNames[] = {
    "none",
    "fit-width",
    "fit-height"
};

class JavaScriptView;
class MuCanvas
{
    JavaScriptView *scriptView;

    MuCanvasContext *renderingContext;
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

    MuTexture *getTexture();

    MuCanvas(JavaScriptView *scriptView);
    ~MuCanvas();
};

}
