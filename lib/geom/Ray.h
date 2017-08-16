#ifndef GEOM_RAY_H
#define GEOM_RAY_H

#include <geom/Vec3.h>

namespace geom
{

class Ray
{
public:
    Ray(const Vec3& origin, const Vec3& direction, float time = 0.f);

    const Vec3& origin() const { return mOrigin; }
    const Vec3& direction() const { return mDirection; }
    float time() const { return mTime; }

    Vec3 pointAt(float t) const;

private:
    Vec3 mOrigin;
    Vec3 mDirection;
    float mTime;
};

}

#endif // GEOM_RAY_H
