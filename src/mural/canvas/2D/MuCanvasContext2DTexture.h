#pragma once

#include "MuCanvasContext2D.h"

namespace mural
{

class MuCanvasContext2DTexture : public MuCanvasContext2D
{
public:
    MuCanvasContext2DTexture();
    ~MuCanvasContext2DTexture();

    void create() override;
};

}
