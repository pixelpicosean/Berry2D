#pragma once

#include "../../common/MuAffineTransform.h"

namespace mural
{

struct MuPresentable {
    MuRect style;

    virtual void present() = 0;
    virtual void finish() = 0;
};

}
