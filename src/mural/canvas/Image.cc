#include "Image.h"

namespace mural
{

Image::Image():
    texture(nullptr),
    path(""),
    loading(false),
    loadCallback(nullptr)
{}

Image::~Image() {}

Texture *Image::getTexture()
{
    return this->texture;
}

double Image::getWidth()
{
    if (this->texture && this->texture->dimensionsKnown) {
        return this->texture->width / this->texture->contentScale;
    }
    else {
        return 0;
    }
}
double Image::getHeight()
{
    if (this->texture && this->texture->dimensionsKnown) {
        return this->texture->height / this->texture->contentScale;
    }
    else {
        return 0;
    }
}
bool Image::getComplete()
{
    if (this->texture && this->texture->dimensionsKnown) {
        return this->texture->textureId;
    }
    else {
        return true;
    }
}

void Image::setSrc(const String& path)
{
    if (this->loading) {
        return;
    }

    if (this->texture) {
        this->texture = nullptr;
    }

    this->path = path;
    this->beginLoad();
}

String Image::getSrc()
{
    return this->path;
}

void Image::beginLoad()
{
    this->loading = true;

    this->texture = Texture::cachedTextureWithPath(this->path, MuOperationQueue::defaultQueue(), [&] {
        this->endLoad();
        // printf("IMAGE: endLoad\n");
    });
}

void Image::endLoad()
{
    this->loading = false;
    if (this->loadCallback) {
        this->loadCallback();
        this->loadCallback = nullptr;
    }
}

int w_Image_constructor(duk_context *ctx)
{
    Image *inst = new Image();
    setNativePointer(ctx, inst);
    duk_push_this(ctx);
    inst->jsObjectRef = jsRef(ctx);

    inst->loadCallback = [=] {
        bool successful = inst->getTexture()->dimensionsKnown;
        const char *evtCode = successful ? "new window.Event('load')" : "new window.Event('error')";
        jsPushRef(ctx, inst->jsObjectRef);
        duk_push_string(ctx, "dispatchEvent");
        duk_eval_string(ctx, evtCode);
        duk_call_prop(ctx, -3, 1);
        duk_pop(ctx);
    };

    return 1;
}

int w_Image_prototype_get_nodeName(duk_context *ctx)
{
    duk_push_string(ctx, "IMG");
    return 1;
}
int w_Image_prototype_get_width(duk_context *ctx)
{
    auto inst = getNativePointer<Image>(ctx);
    duk_push_number(ctx, inst->getWidth());
    return 1;
}
int w_Image_prototype_get_height(duk_context *ctx)
{
    auto inst = getNativePointer<Image>(ctx);
    duk_push_number(ctx, inst->getHeight());
    return 1;
}
int w_Image_prototype_get_complete(duk_context *ctx)
{
    auto inst = getNativePointer<Image>(ctx);
    duk_push_boolean(ctx, inst->getComplete());
    return 1;
}
int w_Image_prototype_get_src(duk_context *ctx)
{
    auto inst = getNativePointer<Image>(ctx);
    duk_push_string(ctx, inst->getSrc().c_str());
    return 1;
}
int w_Image_prototype_set_src(duk_context *ctx)
{
    const char *path = duk_to_string(ctx, 0);
    auto inst = getNativePointer<Image>(ctx);
    inst->setSrc(path);
    return 0;
}

const duk_number_list_entry numbers_of_Image[] = {
    { NULL, 0.0 }
};
const duk_function_list_entry methods_of_Image[] = {
    { NULL, NULL, 0 }
};

void js_register_Image(duk_context *ctx)
{
    MU_START_BINDING(Image);

    MU_BIND_METHODS_AND_NUMBERS(Image); /* global, __MURAL__, constructor */

    MU_BIND_GET(Image, nodeName);
    MU_BIND_GET(Image, width);
    MU_BIND_GET(Image, height);
    MU_BIND_GET(Image, complete);
    MU_BIND_SET_GET(Image, src);

    MU_FINISH_BINDING(Image);

    // Inject eventMixin
    duk_eval_string(ctx, "__MURAL__.eventMixin.call(__MURAL__.Image.prototype);");
    duk_pop(ctx);
}

}
