#pragma once

#include "MuCanvasContext2D.h"
#include "../MuPresentable.h"

namespace mural
{

class MuCanvasContext2DScreen : public MuCanvasContext2D,
                                public MuPresentable
{
public:
    MuCanvasContext2DScreen();
    ~MuCanvasContext2DScreen();

    void create() override;

    void present() override;
    void finish() override;
};

}
