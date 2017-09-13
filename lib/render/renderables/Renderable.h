//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_RENDERABLE_H
#define RENDER_RENDERABLE_H

#include <memory>
#include <vector>

#include <render/Material.h>
#include <render/Ray.h>

namespace geom { class AABB; }

namespace render
{

class Renderable
{
public:
    Renderable(std::shared_ptr<Material>& material);
    Renderable(std::shared_ptr<Material>&& material);
    virtual ~Renderable() = default;

    virtual bool hit(Ray& ray) const = 0;
    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const = 0;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene) = 0;

    unsigned geomId() const { return mGeomId; }
    const std::shared_ptr<Material> material() const { return mMaterial; }

protected:
    unsigned mGeomId;

private:
    std::shared_ptr<Material> mMaterial;
};

}

#endif // RENDER_RENDERABLE_H
