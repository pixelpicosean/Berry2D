#include "Texture.h"
#include <cstdlib>

#include "SharedTextureCache.h"

namespace mural
{

Texture::Texture():
    textureStorage(nullptr),
    callback(nullptr),
    dimensionsKnown(false),
    width(0), height(0),
    fullPath(""),
    contentScale(1.0f),
    textureId(0)
{}

Texture::~Texture() {}

Texture *Texture::cachedTextureWithPath(String path, MuOperationQueue& queue, MuOperation callback)
{
    Texture *texture = nullptr;
    auto it = theTextureCache.textures.find(path);
    if (it != theTextureCache.textures.end()) {
        texture = it->second;
    }

    if (texture) {
        MuOperationQueue::defaultQueue().addBlockOperation(callback);
    }
    else {
        texture = new Texture();
        texture->initWithPath(path, queue, callback);
    }

    return texture;
}

Texture *Texture::initWithPath(String path, MuOperationQueue& queue, MuOperation initCallback)
{
    this->fullPath = path;
    this->loadCompleteCallback = initCallback;
    this->callback = [&] {
        bool result = this->loadPixelsFromPath(this->fullPath, this->data, this->width, this->height, false);

        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        if (result) {
            queue.addBlockOperation([&] {
                // printf("IMAGE: %s load completed\n", this->fullPath.c_str());
                this->dimensionsKnown = true;
                this->createWithPixels(this->data, GL_RGBA);
                if (this->loadCompleteCallback) {
                    this->loadCompleteCallback();
                }
            });
        }
        else {
            queue.addBlockOperation([&] {
                // printf("IMAGE: fail to load %s\n", this->fullPath.c_str());
                if (this->loadCompleteCallback) {
                    this->loadCompleteCallback();
                }
            });
        }
    };

    queue.addOperation(this->callback);

    return this;
}

void Texture::createWithTexture(Texture *other) {}

void Texture::createWithPixels(PixelData *pixels, GLenum format)
{
    this->createWithPixels(pixels, format, GL_TEXTURE_2D);
}
void Texture::createWithPixels(PixelData *pixels, GLenum formatp, GLenum target)
{
    if (this->textureStorage) {
        this->textureStorage = nullptr;
    }

    // Set the default texture params for Canvas2D
    this->params.kTextureParamMinFilter = GL_LINEAR;
    this->params.kTextureParamMagFilter = GL_LINEAR;
    this->params.kTextureParamWrapS = GL_CLAMP_TO_EDGE;
    this->params.kTextureParamWrapT = GL_CLAMP_TO_EDGE;

    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    if (this->width > maxTextureSize || this->height > maxTextureSize) {
        printf("Warning: Image %s larger than MAX_TEXTURE_SIZE (%i)\n", this->fullPath.c_str(), maxTextureSize);
        return;
    }
    this->format = formatp;

    GLint boundTexture = 0;
    GLenum bindingName = (target == GL_TEXTURE_2D) ?
        GL_TEXTURE_BINDING_2D :
        GL_TEXTURE_BINDING_CUBE_MAP;
    glGetIntegerv(bindingName, &boundTexture);

    this->textureStorage = new TextureStorage(true);
    this->textureStorage->bindToTarget(target, this->params);
    glTexImage2D(target, 0, this->format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, this->data);

    glBindTexture(target, boundTexture);
}

bool Texture::loadPixelsFromPath(const String& filename, PixelData* &image_buffer, tex_uint &width, tex_uint &height, bool optional)
{
    FILE *PNG_file = fopen(filename.c_str(), "rb");
    if (PNG_file == NULL) {
        if (!optional)
            printf("ERROR: Couldn't open %s.\n", filename.c_str());
        return false;
    }

    GLubyte PNG_header[8];

    fread(PNG_header, 1, 8, PNG_file);
    if (png_sig_cmp(PNG_header, 0, 8) != 0) {
        if (!optional)
            printf("ERROR: %s is not a PNG.\n", filename.c_str());
        return false;
    }

    png_structp PNG_reader = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (PNG_reader == NULL) {
        if (!optional)
            printf("ERROR: Can't start reading %s.\n", filename.c_str());
        fclose(PNG_file);
        return false;
    }

    png_infop PNG_info = png_create_info_struct(PNG_reader);
    if (PNG_info == NULL) {
        if (!optional)
            printf("ERROR: Can't get info for %s.\n", filename.c_str());
        png_destroy_read_struct(&PNG_reader, NULL, NULL);
        fclose(PNG_file);
        return false;
    }

    png_infop PNG_end_info = png_create_info_struct(PNG_reader);
    if (PNG_end_info == NULL) {
        if (!optional)
            printf("ERROR: Can't get end info for %s.\n", filename.c_str());
        png_destroy_read_struct(&PNG_reader, &PNG_info, NULL);
        fclose(PNG_file);
        return false;
    }

    if (setjmp(png_jmpbuf(PNG_reader))) {
        if (!optional)
            printf("ERROR: Can't load %s.\n", filename.c_str());
        png_destroy_read_struct(&PNG_reader, &PNG_info, &PNG_end_info);
        fclose(PNG_file);
        return false;
    }

    png_init_io(PNG_reader, PNG_file);
    png_set_sig_bytes(PNG_reader, 8);

    png_read_info(PNG_reader, PNG_info);

    width = png_get_image_width(PNG_reader, PNG_info);
    height = png_get_image_height(PNG_reader, PNG_info);

    png_uint_32 bit_depth, color_type;
    bit_depth = png_get_bit_depth(PNG_reader, PNG_info);
    color_type = png_get_color_type(PNG_reader, PNG_info);

    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(PNG_reader);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(PNG_reader);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(PNG_reader);
    }

    if (png_get_valid(PNG_reader, PNG_info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(PNG_reader);
    }
    else {
        png_set_filler(PNG_reader, 0xff, PNG_FILLER_AFTER);
    }

    if (bit_depth == 16) {
        png_set_strip_16(PNG_reader);
    }

    png_read_update_info(PNG_reader, PNG_info);

    image_buffer = (PixelData*)malloc(4 * width * height);
    PixelData** PNG_rows = (PixelData**)malloc(height * sizeof(PixelData*));

    unsigned int row;
    for (row = 0; row < height; ++row) {
        PNG_rows[height - 1 - row] = image_buffer + (row * 4 * width);
    }

    png_read_image(PNG_reader, PNG_rows);

    free(PNG_rows);

    png_destroy_read_struct(&PNG_reader, &PNG_info, &PNG_end_info);
    fclose(PNG_file);

    return true;
}

}
