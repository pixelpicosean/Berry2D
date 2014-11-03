#pragma once

#include "../MuTexture.h"
#include "MuCanvas2DTypes.h"
#include "../MuCanvasContext.h"

#define MU_CANVAS_STATE_STACK_SIZE 16

namespace mural
{

typedef enum {
    kMuLineCapButt,
    kMuLineCapRound,
    kMuLineCapSquare
} MuLineCap;

typedef enum {
    kMuLineJoinMiter,
    kMuLineJoinBevel,
    kMuLineJoinRound
} MuLineJoin;

typedef enum {
    kMuTextBaselineAlphabetic,
    kMuTextBaselineMiddle,
    kMuTextBaselineTop,
    kMuTextBaselineHanging,
    kMuTextBaselineBottom,
    kMuTextBaselineIdeographic
} MuTextBaseline;

typedef enum {
    kMuTextAlignStart,
    kMuTextAlignEnd,
    kMuTextAlignLeft,
    kMuTextAlignCenter,
    kMuTextAlignRight
} MuTextAlign;

typedef enum {
    kMuCompositeOperationSourceOver,
    kMuCompositeOperationLighter,
    kMuCompositeOperationDarker,
    kMuCompositeOperationDestinationOut,
    kMuCompositeOperationDestinationOver,
    kMuCompositeOperationSourceAtop,
    kMuCompositeOperationXOR
} MuCompositeOperation;

static const struct { GLenum source; GLenum destination; float alphaFactor; } MuCompositeOperationFuncs[] = {
    {GL_ONE, GL_ONE_MINUS_SRC_ALPHA, 1},
    {GL_ONE, GL_ONE_MINUS_SRC_ALPHA, 0},
    {GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA, 1},
    {GL_ZERO, GL_ONE_MINUS_SRC_ALPHA, 1},
    {GL_ONE_MINUS_DST_ALPHA, GL_ONE, 1},
    {GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 1},
    {GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 1}
};

struct MuFillable;

class MuPath;
struct MuCanvasState {
    MuAffineTransform transform;

    MuCompositeOperation globalCompositeOperation;
    MuColorRGBA fillColor;
    MuFillable* fillObject;
    MuColorRGBA strokeColor;
    float globalAlpha;

    float lineWidth;
    MuLineCap lineCap;
    MuLineJoin lineJoin;
    float miterLimit;

    MuTextAlign textAlign;
    MuTextBaseline textBaseline;
    // UIFont* font;

    MuPath * clipPath;
};

static inline MuColorRGBA MuCanvasBlendColor(MuCanvasState *state, MuColorRGBA color) {
    float alpha = state->globalAlpha * (float)color.rgba.a / 255.0f;
    MuColorRGBA blendedColor;
    blendedColor.rgba.r = (unsigned char)(color.rgba.r * alpha);
    blendedColor.rgba.g = (unsigned char)(color.rgba.g * alpha);
    blendedColor.rgba.b = (unsigned char)(color.rgba.b * alpha);
    blendedColor.rgba.a = (unsigned char)(MuCompositeOperationFuncs[state->globalCompositeOperation].alphaFactor * color.rgba.a * state->globalAlpha);
    return blendedColor;
}

static inline MuColorRGBA MuCanvasBlendWhiteColor(MuCanvasState *state) {
    return MuCanvasBlendColor(state, (MuColorRGBA){ 0xffffffff });
}

static inline MuColorRGBA MuCanvasBlendFillColor(MuCanvasState *state) {
    return MuCanvasBlendColor(state, state->fillColor);
}

static inline MuColorRGBA MuCanvasBlendStrokeColor(MuCanvasState *state) {
    return MuCanvasBlendColor(state, state->strokeColor);
}

class MuCanvasContext2D : public MuCanvasContext
{
protected:
    GLuint viewFrameBuffer, viewRenderBuffer;
    GLuint msaaFrameBuffer, msaaRenderBuffer;
    GLuint stencilBuffer;

    short width, height;
    short bufferWidth, bufferHeight;

    MuTexture *currentTexture;

    MuPath *path;

    MuVertex *vertexBuffer;
    int vertexBufferSize;
    int vertexBufferIndex;

    int stateIndex;
    MuCanvasState stateStack[MU_CANVAS_STATE_STACK_SIZE];

    bool upsideDown;

    // MuGLProgram2D *currentProgram;
    // MuSharedOpenGLContext *sharedGLContext;
    MuFillable* fillObject;

    // void setProgram(MuGLProgram2D *program);

public:
    // NSCache * fontCache;

    MuCanvasState *state;
    // UIFont * font;
    float backingStoreRatio;
    bool msaaEnabled;
    int msaaSamples;
    bool imageSmoothingEnabled;

    MuCanvasContext2D();
    MuCanvasContext2D(short widthp, short heightp);
    ~MuCanvasContext2D();
    virtual void create();
    virtual void resizeToWidth(short newWidth, short newHeight);
    void setScreenSize(int widthp, int heightp);
    void createStencilBufferOnce();
    void bindVertexBuffer();
    virtual void prepare();
    void setTexture(MuTexture *newTexture);
    void pushTri(float x1, float y1, float x2, float y2, float x3, float y3, MuColorRGBA color, MuAffineTransform transform);
    void pushQuad(MuVector2 v1, MuVector2 v2, MuVector2 v3, MuVector2 v4, MuVector2 t1, MuVector2 t2, MuVector2 t3, MuVector2 t4, MuColorRGBA color, MuAffineTransform transform);
    void pushRect(float x, float y, float w, float h, float tx, float ty, float tw, float th, MuColorRGBA color, MuAffineTransform transform);
    void pushFilledRect(float x, float y, float w, float h, MuFillable* fillable, MuColorRGBA color, MuAffineTransform transform);
    void pushTexturedRect(float x, float y, float w, float h, float tx, float ty, float tw, float th, MuColorRGBA color, MuAffineTransform transform);
    // void pushPatternedRect(float x, float y, float w, float h, MuCanvasPattern* pattern, MuColorRGBA color, MuAffineTransform transform);
    void flushBuffers();

    void save();
    void restore();
    void rotate(float angle);
    void translate(float x, float y);
    void scale(float x, float y);
    void transform(float m11, float m12, float m21, float m22, float dx, float dy);
    void setTransform(float m11, float m12, float m21, float m22, float dx, float dy);
    void drawImage(MuTexture *texture, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh);
    void fillRect(float x, float y, float w, float h);
    void strokeRect(float x, float y, float w, float h);
    void clearRect(float x, float y, float w, float h);
    // virtual MuImageData* getImageData(float sx, float sy, float sw, float sh);
    // void putImageData(MuImageData* imageData, float dx, float dy);
    void beginPath();
    void closePath();
    void fill();
    void stroke();
    void moveTo(float x, float y);
    void lineTo(float x, float y);
    void rect(float x, float y, float w, float h);
    void bezierCurveTo(float cpx, float cpy, float cpx2, float cpy2, float x, float y);
    void quadraticCurveTo(float cpx, float cpy, float x, float y);
    void arcTo(float x1, float y1, float x2, float y2, float radius);
    void arc(float x, float y, float radius, float startAngle, float endAngle, bool antiClockwise);
    // MuFont* acquireFont(NSString* fontName , float pointSize ,BOOL fill ,float contentScale);
    void fillText(const String& text, float x, float y);
    void strokeText(const String& text, float x, float y);
    float measureText(const String& text);

    void clip();
    void resetClip();

    void setGlobalCompositeOperation(MuCompositeOperation op);
    MuCompositeOperation getGlobalCompositeOperation() const;
    void setWidth(short w);
    short getWidth() const;
    void setHeight(short h);
    short getHeight() const;
    void setFillObject(MuFillable *pFillable);
    MuFillable* getFillObject();
};

}
