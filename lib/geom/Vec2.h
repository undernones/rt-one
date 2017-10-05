//
//  Copyright © 2017 Undernones. All rights reserved.
//

#ifndef GEOM_VEC2_H
#define GEOM_VEC2_H

#include <array>
#include <cmath>
#include <simd/simd.h>
#include <iostream>

namespace geom
{
class Vec2
{
public:
    Vec2() : Vec2(0.f) {}
    Vec2(float value) : Vec2(value, value) {}
    Vec2(const float values[2]) : Vec2(values[0], values[1]) {}
    Vec2(const std::array<float, 2>& values) : Vec2(values[0], values[1]) {}
    Vec2(float e0, float e1) { e[0] = e0; e[1] = e1; }
    Vec2(const simd::float2& v) : e(v) {}

    operator simd::float2() const { return e; }
    operator std::array<float, 2>() const { return { e[0], e[1] }; }

    bool operator==(const Vec2& rhs) const {
        return fabs(e[0] - rhs.e[0]) < 1e-4
            && fabs(e[1] - rhs.e[1]) < 1e-4;
    }

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float u() const { return e[0]; }
    inline float v() const { return e[1]; }
    inline float s() const { return e[0]; }
    inline float t() const { return e[1]; }

    inline const Vec2& operator+() const { return *this; }
    inline Vec2 operator-() const { return Vec2(-e); }
    inline float operator[](int i) const { return e[i]; }
    //inline float& operator[](int i) { return e[i]; }

    inline Vec2& operator+=(const Vec2& v2);
    inline Vec2& operator-=(const Vec2& v2);
    inline Vec2& operator*=(const Vec2& v2);
    inline Vec2& operator/=(const Vec2& v2);
    inline Vec2& operator*=(const float t);
    inline Vec2& operator/=(const float t);

    inline float length() const {
        return simd::length(e);
    }
    inline float squaredLength() const {
        return simd::length_squared(e);
    }
    inline void normalize();
    inline Vec2 normalized() const;

    simd::float2 e;

private:
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v);
};

inline Vec2&
Vec2::operator+=(const Vec2& rhs)
{
    e += rhs.e;
    return *this;
}

inline Vec2&
Vec2::operator-=(const Vec2& rhs)
{
    e -= rhs.e;
    return *this;
}

inline Vec2&
Vec2::operator*=(const Vec2& rhs)
{
    e *= rhs.e;
    return *this;
}

inline Vec2&
Vec2::operator/=(const Vec2& rhs)
{
    e /= rhs.e;
    return *this;
}

inline Vec2&
Vec2::operator*=(float t)
{
    e *= t;
    return *this;
}

inline Vec2&
Vec2::operator/=(float t)
{
    e *= (1 / t);
    return *this;
}

inline Vec2
operator+(const Vec2& lhs, const Vec2& rhs)
{
    return Vec2(lhs.e + rhs.e);
}

inline Vec2
operator-(const Vec2& lhs, const Vec2& rhs)
{
    return Vec2(lhs.e - rhs.e);
}

inline Vec2
operator*(const Vec2& lhs, const Vec2& rhs)
{
    return Vec2(lhs.e * rhs.e);
}

inline Vec2
operator/(const Vec2& lhs, const Vec2& rhs)
{
    return Vec2(lhs.e / rhs.e);
}

inline Vec2
operator*(float t, const Vec2& rhs)
{
    // Mysteriously, this is faster than just (t * rhs.e)
    return Vec2(t * rhs.e[0], t * rhs.e[1]);
}

inline Vec2
operator*(const Vec2& lhs, float t)
{
    // Mysteriously, this is faster than just (t * lhs.e)
    return Vec2(t * lhs.e[0], t * lhs.e[1]);
}

inline Vec2
operator/(const Vec2& lhs, float t)
{
    return Vec2(lhs.e[0] / t, lhs.e[1] / t);
}

inline void
Vec2::normalize()
{
    e = simd::normalize(e);
}

inline Vec2
Vec2::normalized() const
{
    auto result = simd::normalize(e);
    return Vec2(result);
}

inline std::ostream&
operator<<(std::ostream& os, const Vec2& v)
{
    return os << "[" << v.x() << ", " << v.y() << "]";
}


}

#endif /* Vec2_h */
