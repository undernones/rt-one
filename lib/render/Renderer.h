//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include <geom/Vec3.h>
#include <render/Ray.h>

namespace render
{

class Scene;
class Renderer
{
public:
    static geom::Vec3 trace(Ray ray, const Scene& scene, int depth = 0);
    static std::array<geom::Vec3, 8> trace(const std::array<int32_t, 8>& valid, Ray8 rays, const Scene& scene, int depth = 0);
};

}

#endif //  RENDER_RENDERER_H
