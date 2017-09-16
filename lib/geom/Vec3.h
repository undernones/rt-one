//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef GEOM_VEC3_H
#define GEOM_VEC3_H

#include <cmath>
#include <simd/simd.h>
#include <iostream>

namespace geom
{

// IMPORTANT: The size of this class must be exactly equal to 4 floats. Otherwise everything will break.
class Vec3
{
public:
    Vec3() : Vec3(0.f) {}
    Vec3(float value) : Vec3(value, value, value) {}
    Vec3(const float values[3]) : Vec3(values[0], values[1], values[2]) {}
    Vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
    Vec3(const simd::float3& v) : e(v) {}

    operator simd::float3() const { return e; }

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const Vec3& operator+() const { return *this; }
    inline Vec3 operator-() const { return Vec3(-e); }
    inline float operator[](int i) const { return e[i]; }
    //inline float& operator[](int i) { return e[i]; }

    inline Vec3& operator+=(const Vec3& v2);
    inline Vec3& operator-=(const Vec3& v2);
    inline Vec3& operator*=(const Vec3& v2);
    inline Vec3& operator/=(const Vec3& v2);
    inline Vec3& operator*=(const float t);
    inline Vec3& operator/=(const float t);

    inline float length() const {
        return simd::length(e);
    }
    inline float squaredLength() const {
        return simd::length_squared(e);
    }
    inline void normalize();
    inline Vec3 normalized() const;

    inline float dot(const Vec3& v) const;
    inline Vec3 cross(const Vec3& v) const;

    simd::float2 xy() const { return e.xy; }

    simd::float3 e;

private:
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v);
};

inline Vec3&
Vec3::operator+=(const Vec3& rhs)
{
    e += rhs.e;
    return *this;
}

inline Vec3&
Vec3::operator-=(const Vec3& rhs)
{
    e -= rhs.e;
    return *this;
}

inline Vec3&
Vec3::operator*=(const Vec3& rhs)
{
    e *= rhs.e;
    return *this;
}

inline Vec3&
Vec3::operator/=(const Vec3& rhs)
{
    e /= rhs.e;
    return *this;
}

inline Vec3&
Vec3::operator*=(float t)
{
    e *= t;
    return *this;
}

inline Vec3&
Vec3::operator/=(float t)
{
    e *= (1 / t);
    return *this;
}

inline float
Vec3::dot(const Vec3& v) const
{
    return simd::dot(e, v.e);
}

inline Vec3
Vec3::cross(const Vec3& v) const
{
    auto result = simd::cross(e, v.e);
    return Vec3(result);
}

inline Vec3
operator+(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.e + rhs.e);
}

inline Vec3
operator-(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.e - rhs.e);
}

inline Vec3
operator*(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.e * rhs.e);
}

inline Vec3
operator/(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.e / rhs.e);
}

inline Vec3
operator*(float t, const Vec3& rhs)
{
    // Mysteriously, this is faster than just (t * rhs.e)
    return Vec3(t * rhs.e[0], t * rhs.e[1], t * rhs.e[2]);
}

inline Vec3
operator*(const Vec3& lhs, float t)
{
    // Mysteriously, this is faster than just (t * lhs.e)
    return Vec3(t * lhs.e[0], t * lhs.e[1], t * lhs.e[2]);
}

inline Vec3
operator/(const Vec3& lhs, float t)
{
    return Vec3(lhs.e[0] / t, lhs.e[1] / t, lhs.e[2] / t);
}

inline void
Vec3::normalize()
{
    e = simd::normalize(e);
}

inline Vec3
Vec3::normalized() const
{
    auto result = simd::normalize(e);
    return Vec3(result);
}

inline std::ostream&
operator<<(std::ostream& os, const Vec3& v)
{
    return os << "[" << v.x() << ", " << v.y() << ", " << v.z() << "]";
}

}

#endif // GEOM_VEC3_H
