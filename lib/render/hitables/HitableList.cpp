//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "HitableList.h"

#include <geom/AABB.h>

namespace render
{

HitableList::HitableList(std::vector<std::shared_ptr<Hitable>>& list)
    : Hitable(std::shared_ptr<Material>(nullptr))
    , mList(std::move(list))
{
}

std::vector<unsigned>
HitableList::commit(RTCScene scene)
{
    auto result = std::vector<unsigned>();
    for (auto& hitable : mList) {
        auto geomIds = hitable->commit(scene);
        result.insert(std::end(result), std::begin(geomIds), std::end(geomIds));
    }
    return result;
}

bool
HitableList::hit(Ray& ray) const
{
    for (const auto& hitable : mList) {
        if (hitable->hit(ray)) {
            return true;
        }
    }
    return false;
}

bool
HitableList::bbox(float t0, float t1, geom::AABB& bbox) const
{
    if (mList.size() < 1) {
        return false;
    }

    auto result = geom::AABB();
    auto localBBox = geom::AABB();
    for (const auto& hitable : mList) {
        if (!hitable->bbox(t0, t1, localBBox)) {
            return false;
        }
        result = geom::AABB(result, localBBox);
    }
    bbox = result;
    return true;
}

}
