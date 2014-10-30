#include "TextureStorage.h"

namespace mural
{

TextureStorage::TextureStorage(bool immutable):
    textureId(0),
    immutable(immutable)
{
    glGenTextures(1, &textureId);
}

TextureStorage::~TextureStorage()
{
    if (this->textureId) {
        glDeleteTextures(1, &textureId);
    }
}

void TextureStorage::bindToTarget(GLenum target, const TextureParams& newParams)
{
    glBindTexture(target, this->textureId);

    // Check if we have to set a param
    if(params.kTextureParamMinFilter != newParams.kTextureParamMinFilter) {
        params.kTextureParamMinFilter = newParams.kTextureParamMinFilter;
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, params.kTextureParamMinFilter);
    }
    if(params.kTextureParamMagFilter != newParams.kTextureParamMagFilter) {
        params.kTextureParamMagFilter = newParams.kTextureParamMagFilter;
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, params.kTextureParamMagFilter);
    }
    if(params.kTextureParamWrapS != newParams.kTextureParamWrapS) {
        params.kTextureParamWrapS = newParams.kTextureParamWrapS;
        glTexParameteri(target, GL_TEXTURE_WRAP_S, params.kTextureParamWrapS);
    }
    if(params.kTextureParamWrapT != newParams.kTextureParamWrapT) {
        params.kTextureParamWrapT = newParams.kTextureParamWrapT;
        glTexParameteri(target, GL_TEXTURE_WRAP_T, params.kTextureParamWrapT);
    }
}

}
