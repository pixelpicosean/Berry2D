#pragma once

#include "MuTexture.h"
#include "../../common/common.h"
#include "../../common/BindUtil.h"

namespace mural
{

class MuImage
{
    MuTexture *texture;
    String path;
    bool loading;
public:
    MuImage();
    ~MuImage();

    int jsObjectRef;
    MuOperation loadCallback;

    MuTexture *getTexture();

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
