#include "Ray.h"

namespace geom
{

Ray::Ray(const Vec3& origin, const Vec3& direction, float time)
    : mOrigin(origin)
    , mDirection(direction)
    , mTime(time)
{
}

Vec3
Ray::pointAt(float t) const
{
    return mOrigin + (t * mDirection);
}

}
