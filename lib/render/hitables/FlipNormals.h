//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_FLIP_NORMALS_H
#define RENDER_FLIP_NORMALS_H

#include <render/Hitable.h>

namespace render
{

class FlipNormals : public Hitable
{
public:
    FlipNormals(std::shared_ptr<Hitable>& object);
    FlipNormals(std::shared_ptr<Hitable>&& object);
    virtual ~FlipNormals();

    virtual bool hit(Ray& ray) const;
    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

private:
    RTCScene mLocalScene;
    std::shared_ptr<Hitable> mObject;

    static void intersectFunc(void* userPtr,   /*!< pointer to user data */
                              RTCRay& rtcRay,  /*!< ray to intersect */
                              size_t item      /*!< item to intersect */);
};

}

#endif // RENDER_FLIP_NORMALS_H
