#include "MuSharedTextureCache.h"

namespace mural
{

MuSharedTextureCache::~MuSharedTextureCache()
{
    this->textures.clear();
}

}
