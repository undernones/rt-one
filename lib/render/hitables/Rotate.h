//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_ROTATE_H
#define RENDER_ROTATE_H

#include <render/Transform.h>
#include <geom/AABB.h>

namespace render
{

class Rotate : public Transform
{
public:
    Rotate(const geom::Vec3& axis, float degrees, std::shared_ptr<Hitable>& object);
    Rotate(const geom::Vec3& axis, float degrees, std::shared_ptr<Hitable>&& object);
    virtual ~Rotate() = default;

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    
protected:
    virtual Ray preIntersect(const Ray& ray);
    virtual Ray postIntersect(const Ray& ray);
    virtual void transform(Ray& ray);

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
