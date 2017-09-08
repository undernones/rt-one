//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_HITABLE_LIST_H
#define RENDER_HITABLE_LIST_H

#include <render/Hitable.h>

#include <memory>
#include <vector>

namespace render
{

class HitableList : public Hitable
{
public:
    HitableList(std::vector<std::shared_ptr<Hitable>>& list);

    virtual bool hit(Ray& ray) const;
    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

    const std::vector<std::shared_ptr<Hitable>>& list() const { return mList; }

private:
    const std::vector<std::shared_ptr<Hitable>> mList;
};

}
#endif // RENDER_HITABLE_LIST_H
