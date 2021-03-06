#include "../../../common/common.h"
#include "MuCanvasContext2D.h"
#include "MuPath.h"

namespace mural
{

MuCanvasContext2D::MuCanvasContext2D():
    viewFrameBuffer(0),
    viewRenderBuffer(0),
    msaaFrameBuffer(0),
    msaaRenderBuffer(0),
    stencilBuffer(0),
    vertexBuffer(NULL),
    vertexBufferSize(0),
    vertexBufferIndex(0),
    upsideDown(false),
    currentProgram(NULL),
    // sharedGLContext(NULL),
    fillObject(NULL)
{
}

MuCanvasContext2D::MuCanvasContext2D(short widthp, short heightp) :
    viewFrameBuffer(0),
    viewRenderBuffer(0),
    msaaFrameBuffer(0),
    msaaRenderBuffer(0),
    stencilBuffer(0),
    vertexBufferIndex(0),
    upsideDown(false),
    currentProgram(NULL)
{
    // sharedGLContext = MuApp::instance()->getOpenGLContext();
    // if (sharedGLContext != NULL) {
    //     vertexBuffer = sharedGLContext->getVertexBuffer();
    //     vertexBufferSize = Mu_OPENGL_VERTEX_BUFFER_SIZE;
    // } else {
    //     vertexBuffer = NULL;
    //     vertexBufferSize = 0;
    // }

    memset(stateStack, 0, sizeof(stateStack));
    stateIndex = 0;
    state = &stateStack[stateIndex];
    state->globalAlpha = 1;
    state->globalCompositeOperation = kMuCompositeOperationSourceOver;
    state->transform = MuAffineTransformIdentity;
    state->lineWidth = 1;
    state->lineCap = kMuLineCapButt;
    state->lineJoin = kMuLineJoinMiter;
    state->miterLimit = 10;
    state->textBaseline = kMuTextBaselineAlphabetic;
    state->textAlign = kMuTextAlignStart;
    // state->font = new UIFont(NSStringMake("simsun.ttc"),32);
    state->clipPath = NULL;

    setScreenSize(widthp, heightp);

    path = new MuPath();
    backingStoreRatio = 1;

    // fontCache = new NSCache();
    // fontCache->setCountLimit(8);

    imageSmoothingEnabled = true;
    msaaEnabled = false;
    msaaSamples = 2;
    fillObject = NULL;
}

MuCanvasContext2D::~MuCanvasContext2D()
{
    // fontCache->release();

    // Release all fonts and clip paths from the stack
    for( int i = 0; i < stateIndex + 1; i++ ) {
        // stateStack[i].font->release();
    }

    if (viewFrameBuffer) {
        glDeleteFramebuffers( 1, &viewFrameBuffer);
    }
    if (viewRenderBuffer) {
        glDeleteRenderbuffers(1, &viewRenderBuffer);
    }
    if (msaaFrameBuffer) {
        glDeleteFramebuffers( 1, &msaaFrameBuffer);
    }
    if (msaaRenderBuffer) {
        glDeleteRenderbuffers(1, &msaaRenderBuffer);
    }
    if (stencilBuffer) {
        glDeleteRenderbuffers(1, &stencilBuffer);
    }

    // path->release();
    delete path;

    // sharedGLContext = NULL;
    vertexBuffer = NULL;
}

void MuCanvasContext2D::resizeToWidth(short newWidth, short newHeight) {
    // This function is a stub - Overwritten in both subclasses
    width = newWidth;
    height = newHeight;

    // backingStoreRatio = (useRetinaResolution && [UIScreen mainScreen].scale == 2) ? 2 : 1;
    backingStoreRatio = 1;
    bufferWidth = width * backingStoreRatio;
    bufferHeight = height * backingStoreRatio;

    // resetFramebuffer();
}

void MuCanvasContext2D::setScreenSize(int widthp, int heightp)
{
    bufferWidth = width = widthp;
    bufferHeight = height = heightp;
}

void MuCanvasContext2D::createStencilBufferOnce()
{
    if( stencilBuffer ) { return; }

    glGenRenderbuffers(1, &stencilBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, stencilBuffer);
    if( msaaEnabled ) {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaSamples, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
    }
    else {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, stencilBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilBuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, msaaEnabled ? msaaRenderBuffer : viewRenderBuffer );

    glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MuCanvasContext2D::bindVertexBuffer()
{
    glEnableVertexAttribArray(kMuGLProgram2DAttributePos);
    glVertexAttribPointer(kMuGLProgram2DAttributePos, 2, GL_FLOAT, GL_FALSE, sizeof(MuVertex), (char *)vertexBuffer + offsetof(MuVertex, pos));

    glEnableVertexAttribArray(kMuGLProgram2DAttributeUV);
    glVertexAttribPointer(kMuGLProgram2DAttributeUV, 2, GL_FLOAT, GL_FALSE, sizeof(MuVertex), (char *)vertexBuffer + offsetof(MuVertex, uv));

    glEnableVertexAttribArray(kMuGLProgram2DAttributeColor);
    glVertexAttribPointer(kMuGLProgram2DAttributeColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(MuVertex), (char *)vertexBuffer + offsetof(MuVertex, color));
}

void MuCanvasContext2D::prepare()
{
    //Bind the frameBuffer and vertexBuffer array
    glBindFramebuffer(GL_FRAMEBUFFER, msaaEnabled ? msaaFrameBuffer : viewFrameBuffer );
    glBindRenderbuffer(GL_RENDERBUFFER, msaaEnabled ? msaaRenderBuffer : viewRenderBuffer );

    glViewport(0, 0, width, height);


    MuCompositeOperation op = state->globalCompositeOperation;
    glBlendFunc( MuCompositeOperationFuncs[op].source, MuCompositeOperationFuncs[op].destination );
    currentTexture = NULL;
    currentProgram = NULL;
    // MuTexture::setSmoothScaling(imageSmoothingEnabled);

    bindVertexBuffer();

    if( state->clipPath ) {
        glDepthFunc(GL_EQUAL);
    }
    else {
        glDepthFunc(GL_ALWAYS);
    }
}

void MuCanvasContext2D::setWidth(short newWidth) {
    if (newWidth == width) {
        // Same width as before? Just clear the canvas, as per the spec
        flushBuffers();
        glClear(GL_COLOR_BUFFER_BIT);
        return;
    }
    resizeToWidth(newWidth, height);
}

short MuCanvasContext2D::getWidth() const {
    return width;
}

void MuCanvasContext2D::setHeight(short newHeight) {
    if( newHeight == height ) {
        // Same height as before? Just clear the canvas, as per the spec
        flushBuffers();
        glClear(GL_COLOR_BUFFER_BIT);
        return;
    }
    resizeToWidth(width, newHeight);
}

short MuCanvasContext2D::getHeight() const {
    return height;
}

void MuCanvasContext2D::setTexture(MuTexture * newTexture) {
    if( currentTexture == newTexture ) { return; }

    flushBuffers();

    currentTexture = newTexture;
    if (currentTexture) {
        // currentTexture->bind();
    }
}

void MuCanvasContext2D::setProgram(MuGLProgram2D *newProgram) {
    if( currentProgram == newProgram ) { return; }

    flushBuffers();
    currentProgram = newProgram;
    if(currentProgram == NULL)
    {
        return;
    }

    glUseProgram(currentProgram->getProgram());
    glUniform2f(currentProgram->getScreen(), width, height * (upsideDown ? -1 : 1));
}

void MuCanvasContext2D::pushTri(float x1, float y1, float x2, float y2, float x3, float y3, MuColorRGBA color, MuAffineTransform transform)
{
    if( vertexBufferIndex >= vertexBufferSize - 3 ) {
        flushBuffers();
    }

    MuVector2 d1 = { x1, y1 };
    MuVector2 d2 = { x2, y2 };
    MuVector2 d3 = { x3, y3 };

    if( !MuAffineTransformIsIdentity(transform) ) {
        d1 = MuVector2ApplyTransform( d1, transform );
        d2 = MuVector2ApplyTransform( d2, transform );
        d3 = MuVector2ApplyTransform( d3, transform );
    }

    MuVertex * vb = &vertexBuffer[vertexBufferIndex];

    MuVertex vb_0 = {d1, {0.5, 1}, color};
    MuVertex vb_1 = { d2, {0.5, 0.5}, color };
    MuVertex vb_2 = { d3, {0.5, 1}, color };

    vb[0] = vb_0;
    vb[1] = vb_1;
    vb[2] = vb_2;

    vertexBufferIndex += 3;
}

void MuCanvasContext2D::pushQuad(MuVector2 v1, MuVector2 v2, MuVector2 v3, MuVector2 v4, MuVector2 t1, MuVector2 t2, MuVector2 t3, MuVector2 t4, MuColorRGBA color, MuAffineTransform transform)
{
    if( vertexBufferIndex >= vertexBufferSize - 6 ) {
        flushBuffers();
    }

    if( !MuAffineTransformIsIdentity(transform) ) {
        v1 = MuVector2ApplyTransform( v1, transform );
        v2 = MuVector2ApplyTransform( v2, transform );
        v3 = MuVector2ApplyTransform( v3, transform );
        v4 = MuVector2ApplyTransform( v4, transform );
    }

    MuVertex * vb = &vertexBuffer[vertexBufferIndex];

    MuVertex vb_0 = { v1, t1, color };
    MuVertex vb_1 = { v2, t2, color };
    MuVertex vb_2 = { v3, t3, color };
    MuVertex vb_3 = { v2, t2, color };
    MuVertex vb_4 = { v3, t3, color };
    MuVertex vb_5 = { v4, t4, color };

    vb[0] = vb_0;
    vb[1] = vb_1;
    vb[2] = vb_2;
    vb[3] = vb_3;
    vb[4] = vb_4;
    vb[5] = vb_5;

    vertexBufferIndex += 6;
}

void MuCanvasContext2D::pushRect(float x, float y, float w, float h, float tx, float ty, float tw, float th, MuColorRGBA color, MuAffineTransform transform)
{

    if( vertexBufferIndex >= vertexBufferSize - 6 ) {
        flushBuffers();
    }

    MuVector2 d11 = { x, y };
    MuVector2 d21 = { x+w, y };
    MuVector2 d12 = { x, y+h };
    MuVector2 d22 = { x+w, y+h };

    if( !MuAffineTransformIsIdentity(transform) ) {
        d11 = MuVector2ApplyTransform( d11, transform );
        d21 = MuVector2ApplyTransform( d21, transform );
        d12 = MuVector2ApplyTransform( d12, transform );
        d22 = MuVector2ApplyTransform( d22, transform );
    }

    MuVertex * vb = &vertexBuffer[vertexBufferIndex];

    MuVertex vb_0 = { d11, {0, 0}, color }; // top left
    MuVertex vb_1 = { d21, {0, 0}, color }; // top right
    MuVertex vb_2 = { d12, {0, 0}, color }; // bottom left

    MuVertex vb_3 = { d21, {0, 0}, color }; // top right
    MuVertex vb_4 = { d12, {0, 0}, color }; // bottom left
    MuVertex vb_5 = { d22, {0, 0}, color };// bottom right

    vb[0] = vb_0;   // top left
    vb[1] = vb_1;   // top right
    vb[2] = vb_2;   // bottom left

    vb[3] = vb_3;   // top right
    vb[4] = vb_4;   // bottom left
    vb[5] = vb_5;// bottom right

    vertexBufferIndex += 6;
}

void MuCanvasContext2D::pushTexturedRect(float x, float y, float w, float h, float tx, float ty, float tw, float th, MuColorRGBA color, MuAffineTransform transform)
{

    if( vertexBufferIndex >= vertexBufferSize - 6 ) {
        flushBuffers();
    }

    MuVector2 d11 = { x, y };
    MuVector2 d21 = { x+w, y };
    MuVector2 d12 = { x, y+h };
    MuVector2 d22 = { x+w, y+h };

    if( !MuAffineTransformIsIdentity(transform) ) {
        d11 = MuVector2ApplyTransform( d11, transform );
        d21 = MuVector2ApplyTransform( d21, transform );
        d12 = MuVector2ApplyTransform( d12, transform );
        d22 = MuVector2ApplyTransform( d22, transform );
    }

    MuVertex * vb = &vertexBuffer[vertexBufferIndex];

    MuVertex vb_0 = { d11, {tx, ty}, color };   // top left
    MuVertex vb_1 = { d21, {tx+tw, ty}, color };    // top right
    MuVertex vb_2 = { d12, {tx, ty+th}, color };    // bottom left

    MuVertex vb_3 = { d21, {tx+tw, ty}, color };    // top right
    MuVertex vb_4 = { d12, {tx, ty+th}, color };    // bottom left
    MuVertex vb_5 = { d22, {tx+tw, ty+th}, color };// bottom right

    vb[0] = vb_0;   // top left
    vb[1] = vb_1;   // top right
    vb[2] = vb_2;   // bottom left

    vb[3] = vb_3;   // top right
    vb[4] = vb_4;   // bottom left
    vb[5] = vb_5;// bottom right

    vertexBufferIndex += 6;
}

void MuCanvasContext2D::pushFilledRect(float x, float y, float w, float h, MuFillable* fillable, MuColorRGBA color, MuAffineTransform transform)
{
    // if (typeid(*fillable) == typeid(MuCanvasPattern))
    {
        // MuCanvasPattern *pattern = (MuCanvasPattern *)fillable;
        // pushPatternedRect(x, y, w, h, pattern, color, transform);
    }
    // else
    {
        printf("Unsupported MuFillable type (%s)", typeid(fillable).name());
    }
    /*

    if( [fillable isKindOfClass:[MuCanvasPattern class]] ) {
        MuCanvasPattern *pattern = (MuCanvasPattern *)fillable;
        [self pushPatternedRectX:x y:y w:w h:h pattern:pattern color:color withTransform:transform];
    }
    else if( [fillable isKindOfClass:[MuCanvasGradient class]] ) {
        MuCanvasGradient *gradient = (MuCanvasGradient *)fillable;
        [self pushGradientRectX:x y:y w:w h:h gradient:gradient color:color withTransform:transform];
    }
}
*/
}

// void MuCanvasContext2D::pushPatternedRect(float x, float y, float w, float h, MuCanvasPattern* pattern, MuColorRGBA color, MuAffineTransform transform)
// {
//     MuTexture *texture = pattern->GetTexture();
//     float scale = texture->contentScale;
//     float tw = texture->width / scale;
//     float th = texture->height / scale;
//     float pw = w;
//     float ph = h;

//     if( !(pattern->GetRepeat() & kMuCanvasPatternRepeatX) )
//     {
//         pw = MIN(tw - x, w);
//     }

//     if( !(pattern->GetRepeat() & kMuCanvasPatternRepeatY) )
//     {
//         ph = MIN(th - y, h);
//     }

//     if( pw > 0 && ph > 0 )
//     {
//         setProgram(sharedGLContext->getGlProgram2DPattern());
//         setTexture(texture);
//         pushTexturedRect(x, y, pw, ph, x/tw, y/th, pw/tw, ph/th, color, transform);
//     }

//     if( pw < w || ph < h )
//     {
//         // Draw clearing rect for the stencil buffer if we didn't fill everything with
//         // the pattern image - happens when not repeating in both directions
//         setProgram(sharedGLContext->getGlProgram2DFlat());
//         static MuColorRGBA transparentBlack = {0x0};
//         pushRect(x, y, w, h, 0, 0, 0, 0, transparentBlack, state->transform);
//     }
// }

void MuCanvasContext2D::flushBuffers()
{
    if (vertexBufferIndex == 0) { return; }

    glDrawArrays(GL_TRIANGLES, 0, vertexBufferIndex);
    vertexBufferIndex = 0;
}

void MuCanvasContext2D::setGlobalCompositeOperation(MuCompositeOperation op) {
    // Same composite operation or switching between SourceOver <> Lighter? We don't
    // have to flush and set the blend mode then, but we still need to update the state,
    // as the alphaFactor may be different.
    if (
        op == state->globalCompositeOperation ||
        (op == kMuCompositeOperationLighter && state->globalCompositeOperation == kMuCompositeOperationSourceOver) ||
        (op == kMuCompositeOperationSourceOver && state->globalCompositeOperation == kMuCompositeOperationLighter)
    ) {
        state->globalCompositeOperation = op;
        return;
    }

    flushBuffers();
    glBlendFunc(MuCompositeOperationFuncs[op].source, MuCompositeOperationFuncs[op].destination);
    state->globalCompositeOperation = op;
}

MuCompositeOperation MuCanvasContext2D::getGlobalCompositeOperation() const {
    return state->globalCompositeOperation;
}

void MuCanvasContext2D::save()
{
    if (stateIndex == MU_CANVAS_STATE_STACK_SIZE-1 ) {
        printf("Warning: Mu_CANVAS_STATE_STACK_SIZE (%d) reached\n", MU_CANVAS_STATE_STACK_SIZE);
        return;
    }

    stateStack[stateIndex+1] = stateStack[stateIndex];
    stateIndex++;
    state = &stateStack[stateIndex];
    // state->font->retain();
    // if (state->clipPath) state->clipPath->retain();
}

void MuCanvasContext2D::restore()
{
    if( stateIndex == 0 ) {
        printf("Warning: Can't pop stack at index 0\n");
        return;
    }

    MuCompositeOperation oldCompositeOp = state->globalCompositeOperation;
    MuPath * oldClipPath = state->clipPath;

    // Clean up current state
    // state->font->release();

    // if (state->clipPath && state->clipPath != stateStack[stateIndex-1].clipPath) {
    //     resetClip();
    // }
    // if (state->clipPath) state->clipPath->release();

    // Load state from stack
    stateIndex--;
    state = &stateStack[stateIndex];

    // path->transform = state->transform;

    // Set Composite op, if different
    if( state->globalCompositeOperation != oldCompositeOp ) {
        setGlobalCompositeOperation(state->globalCompositeOperation);
    }

    // Render clip path, if present and different
    if( state->clipPath && state->clipPath != oldClipPath ) {
        // setProgram(sharedGLContext->getGlProgram2DFlat());
        // state->clipPath->drawPolygonsToContext(this,kMuPathPolygonTargetDepth);
    }
}

void MuCanvasContext2D::rotate(float angle)
{
    state->transform = MuAffineTransformRotate( state->transform, angle );
    // path->transform = state->transform;
}

void MuCanvasContext2D::translate(float x, float y)
{
    state->transform = MuAffineTransformTranslate( state->transform, x, y );
    // path->transform = state->transform;
}

void MuCanvasContext2D::scale(float x, float y)
{
    state->transform = MuAffineTransformScale( state->transform, x, y );
    // path->transform = state->transform;
}

void MuCanvasContext2D::transform(float m11, float m12, float m21, float m22, float dx, float dy)
{
    MuAffineTransform t = MuAffineTransformMake( m11, m12, m21, m22, dx, dy );
    state->transform = MuAffineTransformConcat( t, state->transform );
    // path->transform = state->transform;
}

void MuCanvasContext2D::setTransform(float m11, float m12, float m21, float m22, float dx, float dy)
{
    state->transform = MuAffineTransformMake( m11, m12, m21, m22, dx, dy );
    // path->transform = state->transform;
}

void MuCanvasContext2D::drawImage(MuTexture * texture, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh)
{
    if (texture)
    {
        float tw = texture->width;
        float th = texture->height;

        // setProgram(sharedGLContext->getGlProgram2DTexture());
        setTexture(texture);
        pushTexturedRect(dx, dy, dw, dh, sx/tw, sy/th, sw/tw, sh/th, MuCanvasBlendWhiteColor(state), state->transform);
    }
}

void MuCanvasContext2D::fillRect(float x, float y, float w, float h)
{
    if (state->fillObject) {
        pushFilledRect(x, y, w, h, state->fillObject, MuCanvasBlendWhiteColor(state), state->transform);
    }
    else {
        // setProgram(sharedGLContext->getGlProgram2DFlat());
        MuColorRGBA cc = MuCanvasBlendFillColor(state);
        pushRect(x, y, w, h, 0, 0, 0, 0, cc, state->transform);
    }
}

void MuCanvasContext2D::strokeRect(float x, float y, float w, float h)
{
    // strokeRect should not affect the current path, so we create
    // a new, tempPath instead.
    MuPath * tempPath = new MuPath();
    // tempPath->transform = state->transform;

    tempPath->moveTo(x, y);
    tempPath->lineTo(x+w, y);
    tempPath->lineTo(x+w, y+h);
    tempPath->lineTo(x, y+h);
    tempPath->close();

    // setProgram(sharedGLContext->getGlProgram2DFlat());
    // tempPath->drawLinesToContext(this);
    // tempPath->release();
}

void MuCanvasContext2D::clearRect(float x, float y, float w, float h)
{
    // setProgram(sharedGLContext->getGlProgram2DFlat());

    MuCompositeOperation oldOp = state->globalCompositeOperation;
    setGlobalCompositeOperation(kMuCompositeOperationDestinationOut);

    static MuColorRGBA white = {0xffffffff};
    pushRect(x, y, w, h, 0, 0, 0, 0, white, state->transform);

    setGlobalCompositeOperation(oldOp);
}

// MuImageData* MuCanvasContext2D::getImageData(float sx, float sy, float sw, float sh)
// {
//     flushBuffers();
//     GLubyte * pixels = (GLubyte*)malloc( (size_t)sw * (size_t)sh * 4 * sizeof(GLubyte));
//     glReadPixels((GLint)sx, (GLint)sy, (GLsizei)sw, (GLsizei)sh, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//     MuImageData* imageData = new MuImageData((int)sw, (int)sh, pixels);
//     imageData->autorelease();
//     return imageData;
// }

// void MuCanvasContext2D::putImageData(MuImageData* imageData, float dx, float dy)
// {
//     MuTexture * texture = imageData->texture();
//     setProgram(sharedGLContext->getGlProgram2DTexture());
//     setTexture(texture);

//     short tw = texture->realWidth;
//     short th = texture->realHeight;

//     static MuColorRGBA white = {0xffffffff};

//     pushTexturedRect(dx, dy, tw, th, 0, 0, 1, 1, white, MuAffineTransformIdentity);
//     flushBuffers();
// }

void MuCanvasContext2D::beginPath()
{
    path->reset();
}

void MuCanvasContext2D::closePath()
{
    path->close();
}

void MuCanvasContext2D::fill()
{
    // setProgram(sharedGLContext->getGlProgram2DFlat());
    // path->drawPolygonsToContext(this,  kMuPathPolygonTargetColor);
}

void MuCanvasContext2D::stroke()
{
    // setProgram(sharedGLContext->getGlProgram2DFlat());
    path->drawLinesToContext(this);
}

void MuCanvasContext2D::moveTo(float x, float y)
{
    path->moveTo(x, y);
}

void MuCanvasContext2D::lineTo(float x, float y)
{
    path->lineTo(x, y);
}

void MuCanvasContext2D::bezierCurveTo(float cpx, float cpy, float cpx2, float cpy2, float x, float y)
{
    float scale = MuAffineTransformGetScale( state->transform );
    path->quadraticCurveTo(cpx, cpy, x, y, scale);
}

void MuCanvasContext2D::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
    float scale = MuAffineTransformGetScale( state->transform );
    path->quadraticCurveTo(cpx, cpy, x, y, scale);
}

void MuCanvasContext2D::rect(float x, float y, float w, float h)
{
    path->moveTo(x, y);
    path->lineTo(x+w, y);
    path->lineTo(x+w, y+h);
    path->lineTo(x, y+h);
    path->close();
}

void MuCanvasContext2D::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    path->arcTo(x1, y1, x2, y2, radius);
}

void MuCanvasContext2D::arc(float x, float y, float radius, float startAngle, float endAngle, bool antiClockwise)
{
    path->arc(x, y, radius, startAngle, endAngle, antiClockwise);
}

// MuFont* MuCanvasContext2D::acquireFont(const String& fontName , float pointSize ,bool fill ,float contentScale) {
//     //NSString * cacheKey = NSString::createWithFormat("%s_%.2f_%d_%.2f", fontName->getCString(), pointSize, fill, contentScale);
//     MuFont * font = (MuFont *)fontCache->objectForKey(fontName->getCString());
//     if( !font ) {
//         font =new MuFont(fontName,pointSize ,fill ,contentScale);
//         fontCache->setObject(font,fontName->getCString());
//         font->release();
//     }else{
//         font->setFill(fill);
//     }
//     return font;
// }

// void MuCanvasContext2D::fillText(const String& text, float x, float y)
// {
//     MuFont *font =acquireFont(state->font->fontName ,state->font->pointSize,true,backingStoreRatio);

//     setProgram(sharedGLContext->getGlProgram2DAlphaTexture());
//     font->drawString(text, this, x, y);
// }

// void MuCanvasContext2D::strokeText(NSString * text, float x, float y)
// {
//     MuFont *font =acquireFont(state->font->fontName ,state->font->pointSize,false,backingStoreRatio);

//     setProgram(sharedGLContext->getGlProgram2DAlphaTexture());
//     font->drawString(text, this, x, y);
//     fillText(text,x,y);
// }

// float MuCanvasContext2D::measureText(NSString * text)
// {
//     MuFont *font =acquireFont(state->font->fontName ,state->font->pointSize,true,backingStoreRatio);
//     return font->measureString(text);
// }

// void MuCanvasContext2D::clip()
// {
//     flushBuffers();

//     if (state == NULL) {
//         return;
//     }

//     if (state->clipPath) {
//         state->clipPath->release();
//         state->clipPath = NULL;
//     }

//     state->clipPath = (MuPath*)(path->copy());
//     setProgram(sharedGLContext->getGlProgram2DFlat());
//     state->clipPath->drawPolygonsToContext(this, kMuPathPolygonTargetDepth);
// }

// void MuCanvasContext2D::resetClip()
// {
//     if( state->clipPath ) {
//         flushBuffers();
//         state->clipPath->release();
//         state->clipPath = NULL;

//         glDepthMask(GL_TRUE);
//         glClear(GL_DEPTH_BUFFER_BIT);
//         glDepthMask(GL_FALSE);
//         glDepthFunc(GL_ALWAYS);
//     }
// }

// void MuCanvasContext2D::setFillObject(MuFillable* pFillObject)
// {
//     if (state) {
//         if (state->fillObject) {
//             state->fillObject->release();
//         }
//         if (pFillObject) {
//             pFillObject->retain();
//         }
//         state->fillObject = pFillObject;
//     }
// }

// MuFillable* MuCanvasContext2D::getFillObject()
// {
//     return state->fillObject;
// }

}
