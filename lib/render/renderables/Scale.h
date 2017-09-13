//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_SCALE_H
#define RENDER_SCALE_H

#include <render/Transform.h>

namespace render
{

class Scale : public Transform
{
public:
    Scale(const geom::Vec3& scale, std::shared_ptr<Renderable>& object);
    Scale(const geom::Vec3& scale, std::shared_ptr<Renderable>&& object);
    virtual ~Scale() = default;

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;

protected:
    virtual Ray preIntersect(const Ray& ray) const;
    virtual Ray postIntersect(const Ray& ray) const;
    virtual void transform(Ray& ray) const;

private:
    geom::Vec3 mScale;
    bool mBoxIsValid;
    geom::AABB mBox;
};

}

#endif // RENDER_SCALE_H
