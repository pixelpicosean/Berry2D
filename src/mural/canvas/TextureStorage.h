#pragma once

#include <GLFW/glfw3.h>

namespace mural
{

struct TextureParams {
    int kTextureParamMinFilter;
    int kTextureParamMagFilter;
    int kTextureParamWrapS;
    int kTextureParamWrapT;
};

class TextureStorage
{
public:
    TextureParams params;
    GLuint textureId;
    bool immutable;
    /*lastBound*/

public:
    TextureStorage(bool immutable = false);
    ~TextureStorage();

    void bindToTarget(GLenum target, const TextureParams& params);
};

}
