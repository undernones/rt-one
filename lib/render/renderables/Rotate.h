//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_ROTATE_H
#define RENDER_ROTATE_H

#include <render/Transform.h>

namespace render
{

class Rotate : public Transform
{
public:
    Rotate(const geom::Vec3& axis, float degrees, std::shared_ptr<Renderable>& object);
    Rotate(const geom::Vec3& axis, float degrees, std::shared_ptr<Renderable>&& object);
    virtual ~Rotate() = default;

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;

protected:
    virtual Ray preIntersect(const Ray& ray) const;
    virtual Ray postIntersect(const Ray& ray) const;
    virtual void transform(Ray& ray) const;

private:
    geom::Vec3 mAxis;
    float mDegrees;
    simd::float3x3 mMatrix;
    simd::float3x3 mInverse;
    bool mBoxIsValid;
    geom::AABB mBox;
};

}

#endif // RENDER_ROTATE_H
