#pragma once

#include <libpng/png.h>
#include "../../common/StringUtil.h"
#include "../../common/MuOperationQueue.h"
#include "TextureStorage.h"

namespace mural
{

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;
typedef png_byte PixelData;
typedef png_uint_32 tex_uint;

class Texture
{
    TextureParams params;
    GLuint fbo;
    std::shared_ptr<TextureStorage> textureStorage;

public:
    MuOperation callback;

    bool dimensionsKnown;
    tex_uint width, height;
    GLenum format;
    String fullPath;
    float contentScale;

    GLuint textureId;

    PixelData *data;
public:
    Texture();
    ~Texture();

    // For loading on a background thread (non-blocking), but tries the cache first
    static TexturePtr cachedTextureWithPath(String path, MuOperationQueue& queue, MuOperation callback);
    // For loading on a background thread (non-blocking)
    TexturePtr initWithPath(String path, MuOperationQueue& queue, MuOperation callback);

    void createWithTexture(Texture *other);
    void createWithPixels(PixelData *pixels, GLenum format);
    void createWithPixels(PixelData *pixels, GLenum format, GLenum target);
    bool loadPixelsFromPath(const std::string& filename, PixelData* &image_buffer, tex_uint &width, tex_uint &height, bool optional);
};

}
