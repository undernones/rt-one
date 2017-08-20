//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef GEOM_HITABLE_H
#define GEOM_HITABLE_H

#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

namespace geom
{

class Hitable
{
public:
    virtual bool hit(RTCRay& ray) const = 0;
};

}
#endif // GEOM_HITABLE_H
