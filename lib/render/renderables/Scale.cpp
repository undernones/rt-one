//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Scale.h"

namespace render
{

Scale::Scale(const geom::Vec3& scale, std::shared_ptr<Renderable>& object)
    : Scale(scale, std::move(object))
{
}

Scale::Scale(const geom::Vec3& scale, std::shared_ptr<Renderable>&& object)
    : Transform(object)
    , mScale(scale)
{
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
    if (mObject == nullptr) {
        return false;
    }

    auto tmpBox = geom::AABB();
    if (mObject->bbox(0, 1, tmpBox)) {
        auto min = tmpBox.min() * mScale;
        auto max = tmpBox.max() * mScale;
        bbox = geom::AABB(min, max);
        return true;
    }
    return false;
}

}
