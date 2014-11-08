#include "MuCanvas.h"
#include "../JavaScriptView.h"
#include "MuPresentable.h"

namespace mural
{

int w_CanvasStyle_constructor(duk_context *ctx)
{
    MuCanvasStyle *inst = new MuCanvasStyle();
    setNativePointer(ctx, inst);
    duk_push_this(ctx);
    inst->jsObjectRef = jsRef(ctx);

    return 1;
}
int w_CanvasStyle_prototype_get_width(duk_context *ctx)
{
    MuCanvasStyle *inst = getNativePointer<MuCanvasStyle>(ctx);
    duk_push_string(ctx, intToString(inst->binding->styleWidth).c_str());
    return 1;
}
int w_CanvasStyle_prototype_set_width(duk_context *ctx)
{
    MuCanvasStyle *inst = getNativePointer<MuCanvasStyle>(ctx);
    if (duk_is_number(ctx, 0)) {
        inst->binding->styleWidth = duk_to_number(ctx, 0);
    }
    else if (duk_is_string(ctx, 0)) {
        const char *valueString = duk_to_string(ctx, 0);
        inst->binding->styleWidth = stringToFloat(valueString);
    }
    else {
        inst->binding->styleWidth = 0.0f;
    }
    return 0;
}
int w_CanvasStyle_prototype_get_height(duk_context *ctx)
{
    MuCanvasStyle *inst = getNativePointer<MuCanvasStyle>(ctx);
    duk_push_string(ctx, intToString(inst->binding->styleHeight).c_str());
    return 1;
}
int w_CanvasStyle_prototype_set_height(duk_context *ctx)
{
    MuCanvasStyle *inst = getNativePointer<MuCanvasStyle>(ctx);
    if (duk_is_number(ctx, 0)) {
        inst->binding->styleHeight = duk_to_number(ctx, 0);
    }
    else if (duk_is_string(ctx, 0)) {
        const char *valueString = duk_to_string(ctx, 0);
        inst->binding->styleHeight = stringToFloat(valueString);
    }
    else {
        inst->binding->styleHeight = 0.0f;
    }
    return 0;
}
int w_CanvasStyle_prototype_get_left(duk_context *ctx)
{
    MuCanvasStyle *inst = getNativePointer<MuCanvasStyle>(ctx);
    duk_push_string(ctx, intToString(inst->binding->styleLeft).c_str());
    return 1;
}
int w_CanvasStyle_prototype_set_left(duk_context *ctx)
{
    MuCanvasStyle *inst = getNativePointer<MuCanvasStyle>(ctx);
    if (duk_is_number(ctx, 0)) {
        inst->binding->styleLeft = duk_to_number(ctx, 0);
    }
    else if (duk_is_string(ctx, 0)) {
        const char *valueString = duk_to_string(ctx, 0);
        inst->binding->styleLeft = stringToFloat(valueString);
    }
    else {
        inst->binding->styleLeft = 0.0f;
    }
    return 0;
}
int w_CanvasStyle_prototype_get_top(duk_context *ctx)
{
    MuCanvasStyle *inst = getNativePointer<MuCanvasStyle>(ctx);
    duk_push_string(ctx, intToString(inst->binding->styleTop).c_str());
    return 1;
}
int w_CanvasStyle_prototype_set_top(duk_context *ctx)
{
    MuCanvasStyle *inst = getNativePointer<MuCanvasStyle>(ctx);
    if (duk_is_number(ctx, 0)) {
        inst->binding->styleTop = duk_to_number(ctx, 0);
    }
    else if (duk_is_string(ctx, 0)) {
        const char *valueString = duk_to_string(ctx, 0);
        inst->binding->styleTop = stringToFloat(valueString);
    }
    else {
        inst->binding->styleTop = 0.0f;
    }
    return 0;
}
const duk_number_list_entry numbers_of_CanvasStyle[] = {
    { NULL, 0.0 }
};
const duk_function_list_entry methods_of_CanvasStyle[] = {
    { NULL, NULL, 0 }
};
void js_register_CanvasStyle(duk_context *ctx)
{
    MU_START_BINDING(CanvasStyle);
    MU_BIND_METHODS_AND_NUMBERS(CanvasStyle);

    MU_BIND_SET_GET(CanvasStyle, width);
    MU_BIND_SET_GET(CanvasStyle, height);
    MU_BIND_SET_GET(CanvasStyle, left);
    MU_BIND_SET_GET(CanvasStyle, top);

    MU_FINISH_BINDING(CanvasStyle);
}

MuCanvas::MuCanvas(JavaScriptView *scriptView):
    scriptView(scriptView),
    renderingContext(nullptr),
    contextMode(kMuCanvasContextModeInvalid),
    width(0), height(0),
    isScreenCanvas(false),
    useRetinaResolution(true),
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
    if (this->styleObject) {
        this->styleObject->binding = nullptr;
        delete this->styleObject;
        this->styleObject = nullptr;
    }
}

#define MU_GET_SET_STYLE(GETTER, SETTER, TARGET) \
    float MuCanvas::GETTER() { return TARGET; } \
    void MuCanvas::SETTER(float value) { \
        TARGET = value; \
        if (renderingContext) { \
            MuPresentable * inst = dynamic_cast<MuPresentable*>(renderingContext); \
            if (inst) { \
                scriptView->currentRenderingContext = renderingContext; \
                inst->style = style; \
            } \
        } \
    } \

    MU_GET_SET_STYLE(getStyleWidth, setStyleWidth, style.size.width);
    MU_GET_SET_STYLE(getStyleHeight, setStyleHeight, style.size.height);
    MU_GET_SET_STYLE(getStyleLeft, setStyleLeft, style.origin.x);
    MU_GET_SET_STYLE(getStyleTop, setStyleTop, style.origin.y);

#undef MU_GET_SET_STYLE

MuTexture *MuCanvas::getTexture()
{
    return renderingContext->texture;
}

}
