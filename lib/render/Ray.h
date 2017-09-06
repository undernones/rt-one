//
//  Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_RAY_H
#define RENDER_RAY_H

#include <embree2/rtcore_ray.h>
#include <geom/Vec3.h>

namespace render
{

class Material;

struct RTCORE_ALIGN(16) Ray
{
    /* methods */
public:
    Ray() = default;

    Ray(const geom::Vec3& origin, const geom::Vec3& direction, float time, float tnear = 0.f)
        : origin(origin)
        , direction(direction)
        , tnear(tnear)
        , tfar(MAXFLOAT)
        , time(time)
        , mask(0xFFFFFFFF)
        , geomID(RTC_INVALID_GEOMETRY_ID)
        , primID(RTC_INVALID_GEOMETRY_ID)
        , instID(RTC_INVALID_GEOMETRY_ID)
        , material(nullptr)
    {
    }

    Ray& operator =(const RTCRay& rhs)
    {
        memcpy(this, &rhs, sizeof(rhs));
        return *this;
    }

    geom::Vec3 pointAt(float t) const
    {
        return origin + t * direction;
    }

    /* ray data */
public:
    geom::Vec3 origin;    //!< Ray origin
    geom::Vec3 direction; //!< Ray direction

    float tnear;          //!< Start of ray segment
    float tfar;           //!< End of ray segment (set to hit distance)

    float time;           //!< Time of this ray for motion blur
    unsigned mask;        //!< Used to mask out objects during traversal

    /* hit data */
public:
    geom::Vec3 normal;    //!< Unnormalized geometry normal

    float u;              //!< Barycentric u coordinate of hit
    float v;              //!< Barycentric v coordinate of hit

    unsigned geomID;        //!< geometry ID
    unsigned primID;        //!< primitive ID
    unsigned instID;        //!< instance ID

    /* extended hit data */
public:
    Material* material; // No ownership!
    char align1; // No idea why this works!
};

}

#endif // RENDER_RAY_H
