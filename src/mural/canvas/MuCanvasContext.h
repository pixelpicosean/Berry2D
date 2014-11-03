#pragma once

#include "MuCanvasTypes.h"

namespace mural
{

class MuCanvasContext
{
protected:
    bool needsPresenting;
public:
    bool msaaEnabled = false;
    int msaaSamples = 2;
    short width = 0;
    short height = 0;

    virtual void create();
    virtual void flushBuffers();
    virtual void prepare();
};

}
