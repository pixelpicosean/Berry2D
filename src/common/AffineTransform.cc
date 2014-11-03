#include "AffineTransform.h"
#include <cmath>

namespace mural
{

MuAffineTransform MuAffineTransformMake(float a, float b, float c, float d, float tx, float ty) {
    MuAffineTransform t;
    t.a = a; t.b = b; t.c = c; t.d = d; t.tx = tx; t.ty = ty;
    return t;
}
MuPoint MuPointApplyAffineTransform(const MuPoint& point, const MuAffineTransform& t) {
    MuPoint p(
        t.a * point.x + t.c * point.y + t.tx,
        t.b * point.x + t.d * point.y + t.ty
    );
    return p;
}

MuSize MuSizeApplyAffineTransform(const MuSize& size, const MuAffineTransform& t) {
    MuSize s(
        t.a * size.width + t.c * size.height,
        t.b * size.width + t.d * size.height
    );
    return s;
}

MuAffineTransform MuAffineTransformMakeIdentity() {
    return MuAffineTransformMake(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
}
MuRect MuRectApplyAffineTransform(const MuRect& rect, const MuAffineTransform& anAffineTransform) {
    float top    = rect.getMinY();
    float left   = rect.getMinX();
    float right  = rect.getMaxX();
    float bottom = rect.getMaxY();

    MuPoint topLeft = MuPointApplyAffineTransform(MuPointMake(left, top), anAffineTransform);
    MuPoint topRight = MuPointApplyAffineTransform(MuPointMake(right, top), anAffineTransform);
    MuPoint bottomLeft = MuPointApplyAffineTransform(MuPointMake(left, bottom), anAffineTransform);
    MuPoint bottomRight = MuPointApplyAffineTransform(MuPointMake(right, bottom), anAffineTransform);

    float minX = fmin(fmin(topLeft.x, topRight.x), fmin(bottomLeft.x, bottomRight.x));
    float maxX = fmax(fmax(topLeft.x, topRight.x), fmax(bottomLeft.x, bottomRight.x));
    float minY = fmin(fmin(topLeft.y, topRight.y), fmin(bottomLeft.y, bottomRight.y));
    float maxY = fmax(fmax(topLeft.y, topRight.y), fmax(bottomLeft.y, bottomRight.y));

    return MuRectMake(minX, minY, (maxX - minX), (maxY - minY));
}

MuAffineTransform MuAffineTransformTranslate(const MuAffineTransform& t, float tx, float ty) {
    return MuAffineTransformMake(t.a, t.b, t.c, t.d, t.tx + t.a * tx + t.c * ty, t.ty + t.b * tx + t.d * ty);
}
MuAffineTransform MuAffineTransformRotate(const MuAffineTransform& aTransform, float anAngle) {
    float fSin = sin(anAngle);
    float fCos = cos(anAngle);

    return MuAffineTransformMake(
        aTransform.a * fCos + aTransform.c * fSin,
        aTransform.b * fCos + aTransform.d * fSin,
        aTransform.c * fCos - aTransform.a * fSin,
        aTransform.d * fCos - aTransform.b * fSin,
        aTransform.tx,
        aTransform.ty
    );
}
MuAffineTransform MuAffineTransformScale(const MuAffineTransform& t, float sx, float sy) {
    return MuAffineTransformMake(t.a * sx, t.b * sx, t.c * sy, t.d * sy, t.tx, t.ty);
}
MuAffineTransform MuAffineTransformConcat(const MuAffineTransform& t1, const MuAffineTransform& t2) {
    return MuAffineTransformMake(
        t1.a * t2.a + t1.b * t2.c, t1.a * t2.b + t1.b * t2.d,   // a, b
        t1.c * t2.a + t1.d * t2.c, t1.c * t2.b + t1.d * t2.d,   // c, d
        t1.tx * t2.a + t1.ty * t2.c + t2.tx,                    // tx
        t1.tx * t2.b + t1.ty * t2.d + t2.ty                     // ty
    );
}
bool MuAffineTransformEqualToTransform(const MuAffineTransform& t1, const MuAffineTransform& t2) {
    return (t1.a == t2.a && t1.b == t2.b && t1.c == t2.c && t1.d == t2.d && t1.tx == t2.tx && t1.ty == t2.ty);
}
bool MuAffineTransformIsIdentity(const MuAffineTransform& t) {
    return MuAffineTransformEqualToTransform(MuAffineTransformIdentity, t);
}
MuAffineTransform MuAffineTransformInvert(const MuAffineTransform& t) {
    float determinant = 1 / (t.a * t.d - t.b * t.c);
    return MuAffineTransformMake(
        determinant * t.d, -determinant * t.b, -determinant * t.c, determinant * t.a,
        determinant * (t.c * t.ty - t.d * t.tx), determinant * (t.b * t.tx - t.a * t.ty)
    );
}

}
