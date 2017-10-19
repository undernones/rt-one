//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Translate.h"


namespace render
{

Translate::Translate(const geom::Vec3& displacement, std::shared_ptr<Renderable>& object)
    : Translate(displacement, std::move(object))
{
}

Translate::Translate(const geom::Vec3& displacement, std::shared_ptr<Renderable>&& object)
    : Transform(object)
    , mDisplacement(displacement)
{
}

Ray
Translate::preIntersect(const Ray& ray) const
{
    auto result = ray;
    result.origin -= mDisplacement;
    return result;
}

Ray
Translate::postIntersect(const Ray& ray) const
{
    auto result = ray;
    result.origin += mDisplacement;
    return result;
}

Ray8
Translate::preIntersect(const Ray8& rays) const
{
    auto result = rays;
    result.orgx -= mDisplacement.x();
    result.orgy -= mDisplacement.y();
    result.orgz -= mDisplacement.z();
    return result;
}

Ray8
Translate::postIntersect(const Ray8& rays) const
{
    auto result = rays;
    result.orgx += mDisplacement.x();
    result.orgy += mDisplacement.y();
    result.orgz += mDisplacement.z();
    return result;
}

bool
Translate::bbox(float t0, float t1, geom::AABB& bbox) const
{
    if (mObject != nullptr && mObject->bbox(t0, t1, bbox)) {
        auto min = bbox.min() + mDisplacement;
        auto max = bbox.max() + mDisplacement;
        bbox = geom::AABB(min, max);
        return true;
    }
    return false;
}

}
