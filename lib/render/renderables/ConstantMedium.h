//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_CONSTANT_MEDIUM_H
#define RENDER_CONSTANT_MEDIUM_H

#include <memory>

#include <render/Renderable.h>
#include <render/Texture.h>

namespace render
{

class ConstantMedium : public Renderable
{
public:
    ConstantMedium(std::shared_ptr<Renderable>& boundary, float density, std::shared_ptr<Texture>& texture);
    ConstantMedium(std::shared_ptr<Renderable>& boundary, float density, std::shared_ptr<Texture>&& texture);
    ConstantMedium(std::shared_ptr<Renderable>&& boundary, float density, std::shared_ptr<Texture>&& texture);
    virtual ~ConstantMedium();

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

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
    std::shared_ptr<Renderable> mBoundary;
    float mDensity;
};

}

#endif // RENDER_CONSTANT_MEDIUM_H
