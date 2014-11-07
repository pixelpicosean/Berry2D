#include "MuCanvasContext2DScreen.h"

namespace mural
{

void MuCanvasContext2DScreen::create()
{
    if (msaaEnabled) {
        glGenFramebuffers(1, &msaaFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);

        glGenRenderbuffers(1, &msaaRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, msaaRenderBuffer);

        //glRenderbufferStorageMultisampleIMG(GL_RENDERBUFFER_OES, msaaSamples, GL_RGBA8_OES, bufferWidth, bufferHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaRenderBuffer);
    }

    viewFrameBuffer = (GLuint) 0;
    viewRenderBuffer = (GLuint) 0;

    glBindFramebuffer(GL_FRAMEBUFFER, viewFrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, viewRenderBuffer);
}

}
