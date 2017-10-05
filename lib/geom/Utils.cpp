//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Utils.h"

#include <stdlib.h>

#include "FastRand.h"
#include "Vec2.h"

namespace geom
{

Vec3
randomInUnitSphere()
{
    auto p = Vec3();
    const auto unitSphere = Vec3(1, 1, 1);
    do {
        p = 2 * Vec3(rand3()) - unitSphere;
    } while (p.squaredLength() >= 1);
    return p;
}

Vec3
randomInUnitDisk()
{
    auto p = Vec3();
    const auto unitDisk = Vec3(1, 1, 0);
    do {
        auto r = rand2();
        p = 2 * Vec3(r[0], r[1], 0) - unitDisk;
    } while (p.dot(p) >= 1);
    return p;
}

Vec3
reflect(const Vec3& v, const Vec3& normal)
{
    return v - 2 * v.dot(normal) * normal;
}

bool
refract(const Vec3& v, const Vec3& normal, float ni_over_nt, Vec3& refracted)
{
    auto uv = v.normalized();
    auto dt = uv.dot(normal);
    auto discriminant = 1 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant <= 0) {
        return false;
    }

    refracted = ni_over_nt * (uv - normal * dt) - normal*sqrt(discriminant);
    return true;
}

float
toRadians(float degrees)
{
    return degrees * M_PI / 180;
}

float
toDegrees(float radians)
{
    return radians * 180 / M_PI;
}

Vec3
cartesianToSpherical(const Vec3& cartesian)
{
    // See: http://keisan.casio.com/exec/system/1359533867
    auto x = cartesian.x();
    auto y = cartesian.y();
    auto z = cartesian.z();

    auto theta = std::atan2(x, z);
    auto phi = std::atan2(std::hypot(x, z), y);

    return Vec3(cartesian.length(), theta, phi);
}

Vec3
sphericalToCartesian(const Vec3& spherical)
{
    auto radius = spherical.u();
    auto theta = spherical.v();
    auto phi = spherical.w();

    auto sinPhi = std::sin(phi);
    auto cosPhi = std::cos(phi);
    auto sinTheta = std::sin(theta);
    auto cosTheta = std::cos(theta);

    auto x = sinPhi * sinTheta;
    auto z = sinPhi * cosTheta;
    auto y = cosPhi;
    return Vec3(x, y, z) * radius;
}

float
rand()
{
    return static_cast<float>(fastrand()) / INT16_MAX;
}

std::array<float, 2>
rand2()
{
    auto integers = std::array<uint32_t, 4>();
    rand_sse((unsigned int*)integers.data());

    auto result = Vec2(integers[0], integers[1]);
    result /= static_cast<float>(INT16_MAX);

    return result;
}

std::array<float, 3>
rand3()
{
    auto integers = std::array<uint32_t, 4>();
    rand_sse((unsigned int*)&integers);

    simd::float3 floats = {
        static_cast<float>(integers[0]),
        static_cast<float>(integers[1]),
        static_cast<float>(integers[2]),
    };
    floats /= static_cast<float>(INT16_MAX);

    auto result = std::array<float, 3>();
    result[0] = floats[0];
    result[1] = floats[1];
    result[2] = floats[2];
    return result;
}

std::array<float, 4>
rand4()
{
    auto integers = std::array<uint32_t, 4>();
    rand_sse((unsigned int*)&integers);

    simd::float4 floats = {
        static_cast<float>(integers[0]),
        static_cast<float>(integers[1]),
        static_cast<float>(integers[2]),
        static_cast<float>(integers[3]),
    };
    floats /= static_cast<float>(INT16_MAX);

    auto result = std::array<float, 4>();
    result[0] = floats[0];
    result[1] = floats[1];
    result[2] = floats[2];
    result[3] = floats[3];
    return result;
}

std::array<float, 8>
rand8()
{
    auto integers = std::array<uint32_t, 4>();

    auto scale = 1.f / INT16_MAX;

    rand_sse((unsigned int*)integers.data());
    simd::float4 floats03 = {
        static_cast<float>(integers[0]),
        static_cast<float>(integers[1]),
        static_cast<float>(integers[2]),
        static_cast<float>(integers[3]),
    };
    floats03 *= scale;

    rand_sse((unsigned int*)integers.data());
    simd::float4 floats47 = {
        static_cast<float>(integers[0]),
        static_cast<float>(integers[1]),
        static_cast<float>(integers[2]),
        static_cast<float>(integers[3]),
    };
    floats47 *= scale;

    auto result = std::array<float, 8>();
    result[0] = floats03[0];
    result[1] = floats03[1];
    result[2] = floats03[2];
    result[3] = floats03[3];
    result[4] = floats47[0];
    result[5] = floats47[1];
    result[6] = floats47[2];
    result[7] = floats47[3];
    return result;
}

}
