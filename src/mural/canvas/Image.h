#pragma once

#include "../../common/StringUtil.h"
#include "../Texture.h"
#include "../../common/BindUtil.h"

namespace mural
{

class Image
{
    Texture *texture;
    String path;
    bool loading;
public:
    Image();
    ~Image();

    Texture *getTexture();

    double getWidth();
    double getHeight();
    bool getComplete();

    void setSrc(const String& path);
    String getSrc();

private:
    void beginLoad();
    void endLoad();
};

int w_Image_constructor(duk_context *ctx);

int w_Image_prototype_get_width(duk_context *ctx);
int w_Image_prototype_get_height(duk_context *ctx);
int w_Image_prototype_get_complete(duk_context *ctx);
int w_Image_prototype_get_src(duk_context *ctx);
int w_Image_prototype_set_src(duk_context *ctx);

void js_register_Image(duk_context *ctx);

}
