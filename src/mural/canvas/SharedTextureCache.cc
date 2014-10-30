#include "SharedTextureCache.h"

namespace mural
{

SharedTextureCache::~SharedTextureCache()
{
    this->textures.clear();
}

}
