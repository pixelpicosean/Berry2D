#pragma once

#include "MuCanvas2DTypes.h"

#define Mu_PATH_RECURSION_LIMIT 8
#define Mu_PATH_DISTANCE_EPSILON 1.0f
#define Mu_PATH_COLLINEARITY_EPSILON FLT_EPSILON
#define Mu_PATH_MIN_STEPS_FOR_CIRCLE 20.0f
#define Mu_PATH_MAX_STEPS_FOR_CIRCLE 64.0f

namespace mural
{

typedef enum {
    kMuPathPolygonTargetColor,
    kMuPathPolygonTargetDepth
} MuPathPolygonTarget;

typedef enum {
    kMuPathFillRuleNonZero,
    kMuPathFillRuleEvenOdd
} MuPathFillRule;

class MuCanvasContext2D;
class MuPath
{
    MuVector2 currentPos, lastPushed;
    MuVector2 minPos, maxPos;
    MuPathFillRule fillRule;
    unsigned int longestSubpath;

    float distanceTolerance;

    // CGAffineTransform transform;

public:
    // CGAffineTransform transform();
    // MuPathFillRule fillRule();

    void push(MuVector2 v);
    void reset();
    void close();
    void endSubPath();
    void moveTo(float x, float y);
    void lineTo(float x, float y);
    void bezierCurveTo(float cpx1, float cpy1, float cpx2, float cpy2, float x, float y, float scale);
    void recursiveBezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int level);
    void quadraticCurveTo(float cpx, float cpy, float x, float y, float scale);
    void recursiveQuadratic(float x1, float y1, float x2, float y2, float x3, float y3, int level);
    void arcTo(float x1, float y1, float x2, float y2, float radius);
    void arc(float x, float y, float radius, float startAngle, float endAngle, bool antiClockwise);

    void drawPolygonsToContext(MuCanvasContext2D *context, MuPathFillRule fillRule, MuPathPolygonTarget target);
    void drawLinesToContext(MuCanvasContext2D *context);
};

}
