//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "RenderableList.h"

#include <geom/AABB.h>

namespace render
{

RenderableList::RenderableList(std::vector<std::shared_ptr<Renderable>>& list)
    : Renderable(std::shared_ptr<Material>(nullptr))
    , mList(std::move(list))
{
}

std::vector<unsigned>
RenderableList::commit(RTCDevice device, RTCScene scene)
{
    auto result = std::vector<unsigned>();
    for (auto& renderable : mList) {
        auto geomIds = renderable->commit(device, scene);
        result.insert(std::end(result), std::begin(geomIds), std::end(geomIds));
    }
    return result;
}

bool
RenderableList::bbox(float t0, float t1, geom::AABB& bbox) const
{
    if (mList.size() < 1) {
        return false;
    }

    auto result = geom::AABB();
    auto localBBox = geom::AABB();
    for (const auto& renderable : mList) {
        if (!renderable->bbox(t0, t1, localBBox)) {
            return false;
        }
        result = geom::AABB(result, localBBox);
    }
    bbox = result;
    return true;
}

}
