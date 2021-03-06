//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_TRANSLATE_H
#define RENDER_TRANSLATE_H

#include <render/Transform.h>

namespace render
{

class Translate : public Transform
{
public:
    Translate(const geom::Vec3& displacement, std::shared_ptr<Renderable>& object);
    Translate(const geom::Vec3& displacement, std::shared_ptr<Renderable>&& object);
    virtual ~Translate() = default;

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;

protected:
    virtual Ray preIntersect(const Ray& ray) const;
    virtual Ray postIntersect(const Ray& ray) const;
    virtual void transform(Ray& ray) const { /* Nothing to do */ }

    virtual Ray8 preIntersect(const Ray8& rays) const;
    virtual Ray8 postIntersect(const Ray8& rays) const;
    virtual void transform(Ray8& rays) const { /* Nothing to do */ }

private:
    geom::Vec3 mDisplacement;
};

}
#endif // RENDER_TRANSLATE_H
