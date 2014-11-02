#pragma once

#include <cmath>

namespace mural
{

union MuColorRGBA
{
    struct rgba
    {
        unsigned char r, g, b, a;
    };
    unsigned char components[4];
    unsigned int hex;
};

struct MuVector2
{
    float x, y;
};

struct MuVertex
{
    MuVector2 pos;
    MuVector2 uv;
    MuColorRGBA color;
};

static inline MuVector2 MuVector2Make( float x, float y ) {
    MuVector2 p = {x, y};
    return p;
}

static inline MuVector2 MuVector2Add( MuVector2 a, MuVector2 b ) {
    MuVector2 p = {a.x + b.x, a.y + b.y};
    return p;
}

static inline MuVector2 MuVector2Sub( MuVector2 a, MuVector2 b ) {
    MuVector2 p = {a.x - b.x, a.y - b.y};
    return p;
}

static inline MuVector2 MuVector2Normalize( MuVector2 v ) {
    double ln = sqrtf( v.x*v.x + v.y*v.y );
    if (ln == 0) { return v; }

    v.x /= ln;
    v.y /= ln;
    return v;
}

static inline float MuVector2Length( MuVector2 v ) {
    return sqrtf( v.x*v.x + v.y*v.y );
}

static inline float MuVector2LengthSquared( MuVector2 v ) {
    return v.x*v.x + v.y*v.y;
}

static inline float MuVector2Dot( const MuVector2 v1, const MuVector2 v2 ) {
    return v1.x * v2.x + v1.y * v2.y;
}

static inline float MuVector2Distance( const MuVector2 v1, const MuVector2 v2 ) {
    return (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y);
}

static inline float MuDistanceToLineSegmentSquared(const MuVector2 p, const MuVector2 v, const MuVector2 w) {
    float l2 = MuVector2Distance(v, w);
    if (l2 == 0) return MuVector2Distance(p, v);
    float t = ((p.x - v.x) * (w.x - v.x) + (p.y - v.y) * (w.y - v.y)) / l2;
    if (t < 0) return MuVector2Distance(p, v);
    if (t > 1) return MuVector2Distance(p, w);
    return MuVector2Distance(p, MuVector2Make(v.x + t * (w.x - v.x), v.y + t * (w.y - v.y) ));
}

static inline float MuDistanceToLineSegment(const MuVector2 p, const MuVector2 v, const MuVector2 w) {
    return sqrtf(MuDistanceToLineSegmentSquared(p,v,w));
}

// static inline MuVector2 MuVector2ApplyTransform(MuVector2 p, CGAffineTransform t) {
//     MuVector2 pt = {
//         t.a * p.x + t.c * p.y + t.tx,
//         t.b * p.x + t.d * p.y + t.ty
//     };
//     return pt;
// }

// static inline float CGAffineTransformGetScale( CGAffineTransform t ) {
//     return sqrtf( t.a*t.a + t.c*t.c );
// }

}
