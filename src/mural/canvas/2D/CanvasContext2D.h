#pragma once

#include "../Texture.h"
#include "Canvas2DTypes.h"

namespace mural
{

typedef enum {
    kMuLineCapButt,
    kMuLineCapRound,
    kMuLineCapSquare
} MuLineCap;

typedef enum {
    kMuLineJoinMiter,
    kMuLineJoinBevel,
    kMuLineJoinRound
} MuLineJoin;

typedef enum {
    kMuTextBaselineAlphabetic,
    kMuTextBaselineMiddle,
    kMuTextBaselineTop,
    kMuTextBaselineHanging,
    kMuTextBaselineBottom,
    kMuTextBaselineIdeographic
} MuTextBaseline;

typedef enum {
    kMuTextAlignStart,
    kMuTextAlignEnd,
    kMuTextAlignLeft,
    kMuTextAlignCenter,
    kMuTextAlignRight
} MuTextAlign;

typedef enum {
    kMuCompositeOperationSourceOver,
    kMuCompositeOperationLighter,
    kMuCompositeOperationDarker,
    kMuCompositeOperationDestinationOut,
    kMuCompositeOperationDestinationOver,
    kMuCompositeOperationSourceAtop,
    kMuCompositeOperationXOR,
    kMuCompositeOperationCopy,
    kMuCompositeOperationSourceIn,
    kMuCompositeOperationDestinationIn,
    kMuCompositeOperationSourceOut,
    kMuCompositeOperationDestinationAtop
} MuCompositeOperation;

typedef struct { GLenum source; GLenum destination; float alphaFactor; } MuCompositeOperationFunc;
extern const MuCompositeOperationFunc MuCompositeOperationFuncs[];

}
