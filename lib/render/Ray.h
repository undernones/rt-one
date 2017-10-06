//
//  Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_RAY_H
#define RENDER_RAY_H

#include <array>
#include <embree2/rtcore_ray.h>
#include <geom/Vec2.h>
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

    geom::Vec2 uv;        //!< Barycentric uv coordinates of hit

    unsigned geomID;      //!< geometry ID
    unsigned primID;      //!< primitive ID
    unsigned instID;      //!< instance ID

    /* extended hit data */
public:
    Material* material; // No ownership!
    char align1; // No idea why this works!
};

// -----------------------------------------------------------------------

struct RTCORE_ALIGN(32) Ray8
{
public:
    Ray8() = default;

    Ray8(const std::array<geom::Vec3, 8>& origins, const std::array<geom::Vec3, 8>& directions, float times[8], float tnears[8])
    {
        for (auto i = 0; i < 8; ++i) {
            orgx[i] = origins[i].x();
            orgy[i] = origins[i].y();
            orgz[i] = origins[i].z();

            dirx[i] = directions[i].x();
            diry[i] = directions[i].y();
            dirz[i] = directions[i].z();

            tnear[i] = tnears[i];
            tfar[i] = MAXFLOAT;

            time[i] = times[i];
            mask[i] = 0xFFFFFFFF;

            geomID[i] = RTC_INVALID_GEOMETRY_ID;
            primID[i] = RTC_INVALID_GEOMETRY_ID;
            instID[i] = RTC_INVALID_GEOMETRY_ID;

            material[i] = nullptr;
        }
    }

    std::array<geom::Vec3, 8> pointstAt(const std::array<float, 8>& t) const
    {
        return {
            geom::Vec3(orgx[0], orgy[0], orgz[0]) + t[0] * geom::Vec3(dirx[0], diry[0], dirz[0]),
            geom::Vec3(orgx[1], orgy[1], orgz[1]) + t[1] * geom::Vec3(dirx[1], diry[1], dirz[1]),
            geom::Vec3(orgx[2], orgy[2], orgz[2]) + t[2] * geom::Vec3(dirx[2], diry[2], dirz[2]),
            geom::Vec3(orgx[3], orgy[3], orgz[3]) + t[3] * geom::Vec3(dirx[3], diry[3], dirz[3]),
            geom::Vec3(orgx[4], orgy[4], orgz[4]) + t[4] * geom::Vec3(dirx[4], diry[4], dirz[4]),
            geom::Vec3(orgx[5], orgy[5], orgz[5]) + t[5] * geom::Vec3(dirx[5], diry[5], dirz[5]),
            geom::Vec3(orgx[6], orgy[6], orgz[6]) + t[6] * geom::Vec3(dirx[6], diry[6], dirz[6]),
            geom::Vec3(orgx[7], orgy[7], orgz[7]) + t[7] * geom::Vec3(dirx[7], diry[7], dirz[7]),
        };
    }

    Ray ray(int i) const
    {
        auto origin = geom::Vec3(orgx[i], orgy[i], orgz[i]);
        auto direction = geom::Vec3(dirx[i], diry[i], dirz[i]);
        auto result = Ray(origin, direction, time[i], tnear[i]);
        result.tfar = tfar[i];
        result.mask = mask[i];
        result.normal = { Ngx[i], Ngy[i], Ngz[i] };
        result.uv = { u[i], v[i] };
        result.geomID = geomID[i];
        result.primID = primID[i];
        result.instID = instID[i];
        result.material = material[i];
        return result;
    }

    /* ray data */
public:
    float orgx[8];  //!< x coordinate of ray origin
    float orgy[8];  //!< y coordinate of ray origin
    float orgz[8];  //!< z coordinate of ray origin

    float dirx[8];  //!< x coordinate of ray direction
    float diry[8];  //!< y coordinate of ray direction
    float dirz[8];  //!< z coordinate of ray direction

    float tnear[8]; //!< Start of ray segment
    float tfar[8];  //!< End of ray segment (set to hit distance)

    float time[8];  //!< Time of this ray for motion blur
    unsigned mask[8];  //!< Used to mask out objects during traversal

    /* hit data */
public:
    float Ngx[8];   //!< x coordinate of geometry normal
    float Ngy[8];   //!< y coordinate of geometry normal
    float Ngz[8];   //!< z coordinate of geometry normal

    float u[8];     //!< Barycentric u coordinate of hit
    float v[8];     //!< Barycentric v coordinate of hit

    unsigned geomID[8];  //!< geometry ID
    unsigned primID[8];  //!< primitive ID
    unsigned instID[8];  //!< instance ID

    /* extended hit data */
public:
    Material* material[8]; // No ownership!
};

}

#endif // RENDER_RAY_H
