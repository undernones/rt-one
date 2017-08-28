//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include <embree2/rtcore_ray.h>

#include <geom/Vec3.h>

namespace render
{

class Scene;
class Renderer
{
public:
    static geom::Vec3 color(RTCRay ray, const Scene& scene, int depth = 0);
};

}

#endif //  RENDER_RENDERER_H
