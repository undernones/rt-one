//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_RECTANGLE_H
#define RENDER_RECTANGLE_H

#include <render/Renderable.h>

namespace render
{

// XY: Normal positive in Z
// XZ: Normal positive in Y
// YZ: Normal positive in X
enum class Plane { XY, XZ, YZ };

class Rectangle : public Renderable
{
public:
    Rectangle(Plane plane, float left, float right, float bottom, float top, float offset, std::shared_ptr<Material>& material);
    Rectangle(Plane plane, float left, float right, float bottom, float top, float offset, std::shared_ptr<Material>&& material);
    virtual ~Rectangle();

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

private:
    static void intersectFunc(void* userPtr,   /*!< pointer to user data */
                              RTCRay& rtcRay,  /*!< ray to intersect */
                              size_t item      /*!< item to intersect */);

    RTCScene mLocalScene;
    Plane mPlane;
    float mLeft, mRight, mBottom, mTop, mOffset;
};

}

#endif // RENDER_RECTANGLE_H
