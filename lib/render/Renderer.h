//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include <embree2/rtcore_ray.h>
#include <vector>

#include <geom/Vec3.h>

namespace render
{

class Sphere;
class Renderer
{
public:
    Renderer();
    ~Renderer();

    geom::Vec3 color(RTCRay ray);

private:
    RTCDevice mDevice;
    RTCScene mScene;
    std::vector<Sphere> mSpheres;
};

}

#endif //  RENDER_RENDERER_H
