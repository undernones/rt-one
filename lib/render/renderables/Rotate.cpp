//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Rotate.h"

#include <geom/Utils.h>

namespace
{

simd::float3x3
constructTransform(const geom::Vec3& axis, float degrees)
{
    auto radians = geom::toRadians(degrees);

    auto x = axis[0];
    auto y = axis[1];
    auto z = axis[2];
    auto s = std::sin(radians);
    auto c = std::cos(radians);
    auto C = 1 - c;

    simd::float3 c0 = {
        (x * x * C) + c,
        (y * x * C) + (z * s),
        (z * x * C) - (y * s),
    };
    simd::float3 c1 = {
        (x * y * C) - (z * s),
        (y * y * C) + c,
        (z * y * C) + (x * s),
    };
    simd::float3 c2 = {
        (x * z * C) + (y * s),
        (y * z * C) - (x * s),
        (z * z * C) + c,
    };
    auto result = simd::float3x3(c0, c1, c2);
    return result;
}

}

namespace render
{

Rotate::Rotate(const geom::Vec3& axis, float degrees, std::shared_ptr<Renderable>& object)
    : Rotate(axis, degrees, std::move(object))
{
}

Rotate::Rotate(const geom::Vec3& axis, float degrees, std::shared_ptr<Renderable>&& object)
    : Transform(object)
    , mAxis(axis.normalized())
    , mDegrees(degrees)
    , mMatrix(constructTransform(axis, degrees))
    , mInverse(simd::inverse(mMatrix))
{
}

Ray
Rotate::preIntersect(const Ray& ray) const
{
    auto result = ray;
    result.origin = mInverse * ray.origin;
    result.direction = mInverse * ray.direction;
    return result;
}

Ray
Rotate::postIntersect(const Ray& ray) const
{
    auto result = ray;
    result.origin = mMatrix * ray.origin;
    result.direction = mMatrix * ray.direction;
    return result;
}

void
Rotate::transform(Ray& ray) const
{
    ray.normal = geom::Vec3(simd::transpose(mInverse) * ray.normal);
}

Ray8
Rotate::preIntersect(const Ray8& rays) const
{
    auto result = rays;
    for (auto i = 0; i < 8; ++i) {
        result.setOrigin(i, mInverse * rays.origin(i));
        result.setDirection(i, mInverse * rays.direction(i));
    }
    return result;
}

Ray8
Rotate::postIntersect(const Ray8& rays) const
{
    auto result = rays;
    for (auto i = 0; i < 8; ++i) {
        result.setOrigin(i, mMatrix * rays.origin(i));
        result.setDirection(i, mMatrix * rays.direction(i));
    }
    return result;
}

void
Rotate::transform(Ray8& rays) const
{
    for (auto i = 0; i < 8; ++i) {
        if (rays.geomID[i] != RTC_INVALID_GEOMETRY_ID) {
            rays.setNormal(i, simd::transpose(mInverse) * rays.normal(i));
        }
    }
}

bool
Rotate::bbox(float t0, float t1, geom::AABB& bbox) const
{
    if (mObject == nullptr) {
        return false;
    }

    auto tmpBox = geom::AABB();
    if (mObject->bbox(0, 1, tmpBox)) {
        for (auto i = 0; i <= 1; ++i) {
            for (auto j = 0; j <= 1; ++j) {
                for (auto k = 0; k <= 1; ++k) {
                    auto x = i*tmpBox.max().x() + (1-i)*tmpBox.min().x();
                    auto y = j*tmpBox.max().y() + (1-j)*tmpBox.min().y();
                    auto z = k*tmpBox.max().z() + (1-k)*tmpBox.min().z();
                    auto v = mMatrix * geom::Vec3(x, y, z);
                    tmpBox.includePoint(v);
                }
            }
        }
        bbox = tmpBox;
        return true;
    }
    return false;
}

}
