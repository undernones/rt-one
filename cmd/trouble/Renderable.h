//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_HITABLE_H
#define TROUBLE_HITABLE_H

#include <memory>
#include <vector>
#include <geom/AABB.h>

#include "Material.h"
#include "Ray.h"

class Renderable
{
public:
    Renderable(std::shared_ptr<Material>& material);
    Renderable(std::shared_ptr<Material>&& material);
    virtual ~Renderable() = default;

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const = 0;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene) = 0;

    unsigned geomId() const { return mGeomId; }
    const std::shared_ptr<Material> material() const { return mMaterial; }

protected:
    unsigned mGeomId;

private:
    std::shared_ptr<Material> mMaterial;
};

#endif // TROUBLE_HITABLE_H
