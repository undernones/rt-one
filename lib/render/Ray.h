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

    geom::Vec3 origin(int i) const
    {
        return geom::Vec3(orgx[i], orgy[i], orgz[i]);
    }

    void setOrigin(int i, const geom::Vec3& origin)
    {
        orgx[i] = origin.x();
        orgy[i] = origin.y();
        orgz[i] = origin.z();
    }

    geom::Vec3 direction(int i) const
    {
        return geom::Vec3(dirx[i], diry[i], dirz[i]);
    }

    void setDirection(int i, const geom::Vec3& direction)
    {
        dirx[i] = direction.x();
        diry[i] = direction.y();
        dirz[i] = direction.z();
    }

    geom::Vec3 normal(int i) const
    {
        return geom::Vec3(Ngx[i], Ngy[i], Ngz[i]);
    }

    void setNormal(int i, const geom::Vec3& normal)
    {
        Ngx[i] = normal.x();
        Ngy[i] = normal.y();
        Ngz[i] = normal.z();
    }

    /* ray data */
public:
    simd::float8 orgx;  //!< x coordinate of ray origin
    simd::float8 orgy;  //!< y coordinate of ray origin
    simd::float8 orgz;  //!< z coordinate of ray origin

    simd::float8 dirx;  //!< x coordinate of ray direction
    simd::float8 diry;  //!< y coordinate of ray direction
    simd::float8 dirz;  //!< z coordinate of ray direction

    simd::float8 tnear; //!< Start of ray segment
    simd::float8 tfar;  //!< End of ray segment (set to hit distance)

    std::array<float, 8> time;  //!< Time of this ray for motion blur
    std::array<unsigned, 8> mask;  //!< Used to mask out objects during traversal

    /* hit data */
public:
    std::array<float, 8> Ngx;   //!< x coordinate of geometry normal
    std::array<float, 8> Ngy;   //!< y coordinate of geometry normal
    std::array<float, 8> Ngz;   //!< z coordinate of geometry normal

    std::array<float, 8> u;     //!< Barycentric u coordinate of hit
    std::array<float, 8> v;     //!< Barycentric v coordinate of hit

    std::array<unsigned, 8> geomID;  //!< geometry ID
    std::array<unsigned, 8> primID;  //!< primitive ID
    std::array<unsigned, 8> instID;  //!< instance ID

    /* extended hit data */
public:
    std::array<Material*, 8> material; // No ownership!
};

}

#endif // RENDER_RAY_H
