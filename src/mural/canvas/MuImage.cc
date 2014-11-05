#include "MuImage.h"

namespace mural
{

MuImage::MuImage():
    texture(nullptr),
    path(""),
    loading(false),
    loadCallback(nullptr)
{}

MuImage::~MuImage() {}

MuTexture *MuImage::getTexture()
{
    return this->texture;
}

double MuImage::getWidth()
{
    if (this->texture && this->texture->dimensionsKnown) {
        return this->texture->width / this->texture->contentScale;
    }
    else {
        return 0;
    }
}
double MuImage::getHeight()
{
    if (this->texture && this->texture->dimensionsKnown) {
        return this->texture->height / this->texture->contentScale;
    }
    else {
        return 0;
    }
}
bool MuImage::getComplete()
{
    if (this->texture && this->texture->dimensionsKnown) {
        return this->texture->textureId;
    }
    else {
        return true;
    }
}

void MuImage::setSrc(const String& path)
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

String MuImage::getSrc()
{
    return this->path;
}

void MuImage::beginLoad()
{
    this->loading = true;

    this->texture = MuTexture::cachedTextureWithPath(this->path, MuOperationQueue::defaultQueue(), [&] {
        this->endLoad();
        // printf("IMAGE: endLoad\n");
    });
}

void MuImage::endLoad()
{
    this->loading = false;
    if (this->loadCallback) {
        this->loadCallback();
        this->loadCallback = nullptr;
    }
}

int w_Image_constructor(duk_context *ctx)
{
    MuImage *inst = new MuImage();
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
    auto inst = getNativePointer<MuImage>(ctx);
    duk_push_number(ctx, inst->getWidth());
    return 1;
}
int w_Image_prototype_get_height(duk_context *ctx)
{
    auto inst = getNativePointer<MuImage>(ctx);
    duk_push_number(ctx, inst->getHeight());
    return 1;
}
int w_Image_prototype_get_complete(duk_context *ctx)
{
    auto inst = getNativePointer<MuImage>(ctx);
    duk_push_boolean(ctx, inst->getComplete());
    return 1;
}
int w_Image_prototype_get_src(duk_context *ctx)
{
    auto inst = getNativePointer<MuImage>(ctx);
    duk_push_string(ctx, inst->getSrc().c_str());
    return 1;
}
int w_Image_prototype_set_src(duk_context *ctx)
{
    const char *path = duk_to_string(ctx, 0);
    auto inst = getNativePointer<MuImage>(ctx);
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
