//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_RENDERABLE_LIST_H
#define RENDER_RENDERABLE_LIST_H

#include <render/Renderable.h>

#include <memory>
#include <vector>

namespace render
{

class RenderableList : public Renderable
{
public:
    RenderableList(std::vector<std::shared_ptr<Renderable>>& list);

    virtual bool hit(Ray& ray) const;
    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

    const std::vector<std::shared_ptr<Renderable>>& list() const { return mList; }

private:
    const std::vector<std::shared_ptr<Renderable>> mList;
};

}
#endif // RENDER_RENDERABLE_LIST_H
