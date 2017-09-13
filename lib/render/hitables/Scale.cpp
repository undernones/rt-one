//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Scale.h"

namespace render
{

Scale::Scale(const geom::Vec3& scale, std::shared_ptr<Hitable>& object)
    : Scale(scale, std::move(object))
{
}

Scale::Scale(const geom::Vec3& scale, std::shared_ptr<Hitable>&& object)
    : Transform(object)
    , mScale(scale)
    , mBoxIsValid(false)
{
    if (mObject != nullptr && mObject->bbox(0, 1, mBox)) {
        mBoxIsValid = true;
        auto min = mBox.min() * mScale;
        auto max = mBox.max() * mScale;
        mBox = geom::AABB(min, max);
    }
}

Ray
Scale::preIntersect(const Ray& ray) const
{
    auto result = ray;
    result.origin /= mScale;
    result.direction /= mScale;
    return result;
}

Ray
Scale::postIntersect(const Ray& ray) const
{
    auto result = ray;
    result.origin *= mScale;
    result.direction /= mScale;
    return result;
}

void
Scale::transform(Ray& ray) const
{
    ray.normal /= mScale;
}

bool
Scale::bbox(float t0, float t1, geom::AABB& bbox) const
{
    if (mBoxIsValid) {
        bbox = mBox;
        return true;
    }
    return false;
}

}
