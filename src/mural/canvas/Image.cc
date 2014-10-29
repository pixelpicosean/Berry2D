#include "Image.h"

namespace mural
{

Image::Image():
    texture(nullptr),
    path(""),
    loading(false)
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

    // this->texture = Texture::cachedTextureWithPath(this->path, MuOperationQueue::defaultQueue(), [&] {
    //     this->endLoad();
    // });

    printf("Image loading\n");
    MuOperationQueue::defaultQueue().addOperation([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        this->endLoad();
    });
}

void Image::endLoad()
{
    this->loading = false;
}

int w_Image_constructor(duk_context *ctx)
{
    auto inst = new Image();
    setNativePointer(ctx, inst);

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

const duk_function_list_entry methods_of_Image[] = {
    { NULL, NULL, 0 }
};

void js_register_Image(duk_context *ctx)
{
    duk_push_global_object(ctx); /* global */
    duk_get_prop_string(ctx, -1, MURAL_JS_NAMESPACE); /* global, __MURAL__ */

    duk_push_c_function(ctx, w_Image_constructor, 0); /* global, __MURAL__, constructor */
    duk_push_object(ctx); /* global, __MURAL__, constructor, prototype */
    duk_put_function_list(ctx, -1, methods_of_Image); /* global, __MURAL__, constructor, prototype */
    duk_put_prop_string(ctx, -2, "prototype"); /* global, __MURAL__, constructor */

    // Setup properties
    duk_eval_string(ctx, "__MURAL__.__defineGetter__"); /* global, __MURAL__, constructor, function */
    duk_get_prop_string(ctx, -2, "prototype"); /* global, __MURAL__, constructor, function, prototype */
    duk_push_string(ctx, "width"); /* global, __MURAL__, constructor, function, prototype, width */
    duk_push_c_function(ctx, w_Image_prototype_get_width, 0); /* global, __MURAL__, constructor, function, prototype, width, getter */
    duk_call(ctx, 3); /* global, __MURAL__, constructor, return */
    duk_pop(ctx); /* global, __MURAL__, constructor */

    duk_eval_string(ctx, "__MURAL__.__defineGetter__"); /* global, __MURAL__, constructor, function */
    duk_get_prop_string(ctx, -2, "prototype"); /* global, __MURAL__, constructor, function, prototype */
    duk_push_string(ctx, "height"); /* global, __MURAL__, constructor, function, prototype, width */
    duk_push_c_function(ctx, w_Image_prototype_get_height, 0); /* global, __MURAL__, constructor, function, prototype, width, getter */
    duk_call(ctx, 3); /* global, __MURAL__, constructor, return */
    duk_pop(ctx); /* global, __MURAL__, constructor */

    duk_eval_string(ctx, "__MURAL__.__defineGetter__"); /* global, __MURAL__, constructor, function */
    duk_get_prop_string(ctx, -2, "prototype"); /* global, __MURAL__, constructor, function, prototype */
    duk_push_string(ctx, "complete"); /* global, __MURAL__, constructor, function, prototype, width */
    duk_push_c_function(ctx, w_Image_prototype_get_complete, 0); /* global, __MURAL__, constructor, function, prototype, width, getter */
    duk_call(ctx, 3); /* global, __MURAL__, constructor, return */
    duk_pop(ctx); /* global, __MURAL__, constructor */

    duk_eval_string(ctx, "__MURAL__.__defineAccessor__");
    duk_get_prop_string(ctx, -2, "prototype");
    duk_push_string(ctx, "src");
    duk_push_c_function(ctx, w_Image_prototype_set_src, 1);
    duk_push_c_function(ctx, w_Image_prototype_get_src, 0);
    duk_call(ctx, 4);
    duk_pop(ctx);

    // Register as Image
    duk_put_prop_string(ctx, -2, "Image"); /* global, __MURAL__ */
    duk_pop_2(ctx);
}

}
