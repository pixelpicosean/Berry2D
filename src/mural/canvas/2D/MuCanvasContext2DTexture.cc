#include "MuCanvasContext2DTexture.h"

namespace mural
{

void MuCanvasContext2DTexture::create()
{
    if (msaaEnabled) {
        glGenFramebuffers(1, &msaaFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);

        glGenRenderbuffers(1, &msaaRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, msaaRenderBuffer);

        //glRenderbufferStorageMultisampleIMG(GL_RENDERBUFFER_OES, msaaSamples, GL_RGBA8_OES, bufferWidth, bufferHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaRenderBuffer);
    }

    glGenFramebuffers(1, &viewFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, viewFrameBuffer);

    glGenRenderbuffers(1, &viewRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, viewRenderBuffer);
}

}
