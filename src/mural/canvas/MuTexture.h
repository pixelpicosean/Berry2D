#pragma once

#include <libpng/png.h>
#include "../../common/StringUtil.h"
#include "../../common/MuOperationQueue.h"
#include "MuTextureStorage.h"

namespace mural
{

typedef png_byte PixelData;
typedef png_uint_32 tex_uint;

class MuTexture
{
    MuTextureParams params;
    // GLuint fbo;
    MuTextureStorage *textureStorage = nullptr;

    MuOperation callback = nullptr;
    MuOperation loadCompleteCallback = nullptr;
public:

    bool dimensionsKnown;
    tex_uint width, height;
    GLenum format;
    String fullPath;
    float contentScale;

    GLuint textureId;

    PixelData *data;
public:
    MuTexture();
    ~MuTexture();

    // For loading on a background thread (non-blocking), but tries the cache first
    static MuTexture *cachedTextureWithPath(String path, MuOperationQueue& queue, MuOperation callback);
    // For loading on a background thread (non-blocking)
    MuTexture *initWithPath(String path, MuOperationQueue& queue, MuOperation callback);

    void createWithTexture(MuTexture *other);
    void createWithPixels(PixelData *pixels, GLenum format);
    void createWithPixels(PixelData *pixels, GLenum format, GLenum target);
    bool loadPixelsFromPath(const std::string& filename, PixelData* &image_buffer, tex_uint &width, tex_uint &height, bool optional);
};

}
