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

Vec3
pointAlongRay(float origin[3], float direction[3], float t)
{
    return Vec3(origin[0] + t * direction[0],
                origin[1] + t * direction[1],
                origin[2] + t * direction[2]);
}

}
