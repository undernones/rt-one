//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_BOX_H
#define RENDER_BOX_H

#include <render/Renderable.h>

namespace render
{

/// A unit cube. Move it and scale it with Translate, Rotate, and Scale transformations.
class Box : public Renderable
{
public:
    Box(const geom::Vec3& min, const geom::Vec3& max, std::shared_ptr<Material>& material);
    Box(const geom::Vec3& min, const geom::Vec3& max, std::shared_ptr<Material>&& material);

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

private:
    geom::Vec3 mMin;
    geom::Vec3 mMax;
    std::shared_ptr<Renderable> mChildren;
};

}

#endif // RENDER_BOX_H
