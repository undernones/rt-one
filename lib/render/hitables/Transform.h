//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_TRANSFORM_H
#define RENDER_TRANSFORM_H

#include <render/Hitable.h>

namespace render
{

class Transform : public Hitable
{
public:
    Transform(std::shared_ptr<Hitable>& object);
    Transform(std::shared_ptr<Hitable>&& object);
    virtual ~Transform();

    virtual bool hit(Ray& ray) const;
    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

protected:
    virtual void preIntersect(Ray& ray) { }  // Empty by default
    virtual void postIntersect(Ray& ray) { } // Empty by default
    virtual void transform(Ray& ray) = 0;

private:
    RTCScene mLocalScene;
    std::shared_ptr<Hitable> mObject;

    static void boundsFunc(void* userPtr,         /*!< pointer to user data */
                           void* geomUserPtr,     /*!< pointer to geometry user data */
                           size_t item,           /*!< item to calculate bounds for */
                           RTCBounds* bounds_o    /*!< returns calculated bounds */);
    
    static void intersectFunc(void* userPtr,   /*!< pointer to user data */
                              RTCRay& rtcRay,  /*!< ray to intersect */
                              size_t item      /*!< item to intersect */);
};

}

#endif // RENDER_TRANSFORM_H