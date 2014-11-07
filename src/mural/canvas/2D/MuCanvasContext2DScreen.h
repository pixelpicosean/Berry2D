#pragma once

#include "MuCanvasContext2D.h"

namespace mural
{

class MuCanvasContext2DScreen : public MuCanvasContext2D
{
public:
    MuCanvasContext2DScreen();
    ~MuCanvasContext2DScreen();

    void create() override;
};

}
