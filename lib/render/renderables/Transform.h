//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_TRANSFORM_H
#define RENDER_TRANSFORM_H

#include <render/Renderable.h>
#include <geom/AABB.h>

namespace render
{

class Transform : public Renderable
{
public:
    Transform(std::shared_ptr<Renderable>& object);
    Transform(std::shared_ptr<Renderable>&& object);
    virtual ~Transform();

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

protected:
    virtual Ray preIntersect(const Ray& ray)  const { return ray; }
    virtual Ray postIntersect(const Ray& ray) const { return ray; }
    virtual void transform(Ray& ray) const = 0;

    virtual Ray8 preIntersect(const Ray8& rays)  const { return rays; }
    virtual Ray8 postIntersect(const Ray8& rays) const { return rays; }
    virtual void transform(Ray8& rays) const = 0;

    std::shared_ptr<Renderable> mObject;

private:
    static void boundsFunc(void* userPtr,         /*!< pointer to user data */
                           void* geomUserPtr,     /*!< pointer to geometry user data */
                           size_t item,           /*!< item to calculate bounds for */
                           RTCBounds* bounds_o    /*!< returns calculated bounds */);

    static void intersectFunc(void* userPtr,      /*!< pointer to user data */
                              RTCRay& rtcRay,     /*!< ray to intersect */
                              size_t item         /*!< item to intersect */);

    static void intersectFunc8(const void* valid, /*!< pointer to valid mask */
                               void* userPtr,     /*!< pointer to user data */
                               RTCRay8& rtcRays,  /*!< ray packet to intersect */
                               size_t item        /*!< item to intersect */);

    RTCScene mLocalScene;
};

}

#endif // RENDER_TRANSFORM_H
