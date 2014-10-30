#pragma once

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
    TextureStorage();
    ~TextureStorage();
};

}
