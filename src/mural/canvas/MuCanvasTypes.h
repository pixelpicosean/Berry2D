#pragma once

#include "../../common/MuAffineTransform.h"

namespace mural
{

union MuColorRGBA
{
    unsigned int hex;
    struct {
        unsigned char r, g, b, a;
    } rgba;
    unsigned char components[4];
};

}
