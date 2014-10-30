#pragma once

#include <GLFW/glfw3.h>
#include <libpng/png.h>
#include "../../common/StringUtil.h"
#include "../../common/MuOperationQueue.h"
#include "TextureStorage.h"

namespace mural
{

typedef png_byte PixelData;
typedef png_uint_32 tex_uint;

class Texture
{
    TextureParams params;

public:
    bool dimensionsKnown;
    tex_uint width, height;
    String fullPath;
    float contentScale;

    GLuint textureId;

    PixelData *data;
public:
    Texture();
    ~Texture();

    // For loading on a background thread (non-blocking), but tries the cache first
    static Texture *cachedTextureWithPath(String path, MuOperationQueue& queue, MuOperation callback);
    // For loading on a background thread (non-blocking)
    Texture *initWithPath(String path, MuOperationQueue& queue, MuOperation callback);

    void createWithTexture(Texture *other);
    void createWithPixels(PixelData *pixels, GLenum format);
    void createWithPixels(PixelData *pixels, GLenum format, GLenum target);
    bool loadPixelsFromPath(const std::string& filename, PixelData* &image_buffer, tex_uint &width, tex_uint &height, bool optional);
};

}
