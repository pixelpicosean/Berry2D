#pragma once

namespace mural
{

struct MuSize
{
    float width, height;
    MuSize(): width(0), height(0) {}
    MuSize(float width, float height): width(width), height(height) {}
    MuSize(const MuSize& other): width(other.width), height(other.height) {}
    void setSize(float width, float height)
    {
        this->width = width;
        this->height = height;
    }
    MuSize& operator=(const MuSize& other)
    {
        this->setSize(other.width, other.height);
        return *this;
    }
    bool operator==(const MuSize& other) const
    {
        return (this->width == other.width) && (this->height == other.height);
    }
};

struct MuPoint
{
    float x, y;
    MuPoint(): x(0), y(0) {}
    MuPoint(float x, float y): x(x), y(y) {}
    MuPoint(const MuPoint& other): x(other.x), y(other.y) {}
    void setPoint(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    MuPoint& operator=(const MuPoint& other)
    {
        this->setPoint(other.x, other.y);
        return *this;
    }
    bool operator==(const MuPoint& target) const
    {
        return (this->x == target.x) && (this->y == target.y);
    }
};

struct MuRect
{
    MuPoint origin;
    MuSize size;

    MuRect():
        origin(MuPoint(0.0f, 0.0f)),
        size(MuSize(0.0f, 0.0f))
    {}
    MuRect(float x, float y, float width, float height):
        origin(MuPoint(x, y)),
        size(MuSize(width, height))
    {}
    MuRect(const MuRect& other):
        origin(other.origin.x, other.origin.y),
        size(other.size.width, other.size.height)
    {}
    MuRect& operator=(const MuRect& other)
    {
        this->setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
        return *this;
    }
    bool operator==(const MuRect& other) const
    {
        return (this->origin == other.origin) && (this->size == other.size);
    }
    void setRect(float x, float y, float width, float height)
    {
        this->origin.setPoint(x, y);
        this->size.setSize(width, height);
    }
    bool containsPoint(const MuPoint& point) const
    {
        return (point.x >= getMinX() && point.x <= getMaxX()
            && point.y >= getMinY() && point.y <= getMaxY());
    }
    bool intersectsRect(const MuRect& rect) const
    {
        return !(this->getMaxX() < rect.getMinX() ||
            rect.getMaxX() < this->getMinX() ||
            this->getMaxY() < rect.getMinY() ||
            rect.getMaxY() < this->getMinY());
    }

    float getMinX() const
    {
        return origin.x;
    }
    float getMidX() const
    {
        return origin.x + size.width / 2;
    }
    float getMaxX() const
    {
        return origin.x + size.width;
    }
    float getMinY() const
    {
        return origin.y;
    }
    float getMidY() const
    {
        return origin.y + size.height / 2;
    }
    float getMaxY() const
    {
        return origin.y + size.height;
    }
};

#define MuPointMake(x, y) MuPoint((float)(x), (float)(y))
#define MuSizeMake(width, height) MuSize((float)(width), (float)(height))
#define MuRectMake(x, y, width, height) MuRect((float)(x), (float)(y), (float)(width), (float)(height))

struct MuAffineTransform
{
    float a, b, c, d;
    float tx, ty;
};

MuAffineTransform MuAffineTransformMake(float a, float b, float c, float d, float tx, float ty);
MuPoint MuPointApplyAffineTransform(const MuPoint& point, const MuAffineTransform& t);

MuSize MuSizeApplyAffineTransform(const MuSize& size, const MuAffineTransform& t);

MuAffineTransform MuAffineTransformMakeIdentity();
MuRect MuRectApplyAffineTransform(const MuRect& rect, const MuAffineTransform& anAffineTransform);

MuAffineTransform MuAffineTransformTranslate(const MuAffineTransform& t, float tx, float ty);
MuAffineTransform MuAffineTransformRotate(const MuAffineTransform& aTransform, float anAngle);
MuAffineTransform MuAffineTransformScale(const MuAffineTransform& t, float sx, float sy);
MuAffineTransform MuAffineTransformConcat(const MuAffineTransform& t1, const MuAffineTransform& t2);
bool MuAffineTransformEqualToTransform(const MuAffineTransform& t1, const MuAffineTransform& t2);
bool MuAffineTransformIsIdentity(const MuAffineTransform& t);
MuAffineTransform MuAffineTransformInvert(const MuAffineTransform& t);

extern const MuAffineTransform MuAffineTransformIdentity;

}
