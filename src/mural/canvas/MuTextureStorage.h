#pragma once

#include <GLFW/glfw3.h>

namespace mural
{

struct MuTextureParams {
    int kTextureParamMinFilter;
    int kTextureParamMagFilter;
    int kTextureParamWrapS;
    int kTextureParamWrapT;
};

class MuTextureStorage
{
public:
    MuTextureParams params;
    GLuint textureId;
    bool immutable;
    /*lastBound*/

public:
    MuTextureStorage(bool immutable = false);
    ~MuTextureStorage();

    void bindToTarget(GLenum target, const MuTextureParams& params);
};

}
