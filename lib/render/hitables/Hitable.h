//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_HITABLE_H
#define RENDER_HITABLE_H

#include <memory>
#include <vector>

#include <render/Material.h>
#include <render/Ray.h>

namespace geom { class AABB; }

namespace render
{

class Hitable
{
public:
    Hitable(std::shared_ptr<Material>& material);
    Hitable(std::shared_ptr<Material>&& material);
    virtual ~Hitable() = default;

    virtual bool hit(Ray& ray) const = 0;
    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const = 0;
    virtual std::vector<unsigned> commit(RTCScene scene) = 0;

    unsigned geomId() const { return mGeomId; }
    const std::shared_ptr<Material> material() const { return mMaterial; }

protected:
    unsigned mGeomId;

private:
    std::shared_ptr<Material> mMaterial;
};

}

#endif // RENDER_HITABLE_H
