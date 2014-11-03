#pragma once

#include "../../common/StringUtil.h"
#include <map>

namespace mural
{

class MuTexture;
class MuSharedTextureCache
{
public:
    std::map<String, MuTexture*> textures;

public:
    static MuSharedTextureCache& getInstance()
    {
        static MuSharedTextureCache instance;
        return instance;
    }

    // void releaseStoragesOlderThan(/*seconds*/);
private:
    MuSharedTextureCache() {}
    MuSharedTextureCache(MuSharedTextureCache&) {}
    void operator=(MuSharedTextureCache&) {}
    ~MuSharedTextureCache();
};

#define theTextureCache MuSharedTextureCache::getInstance()

}
