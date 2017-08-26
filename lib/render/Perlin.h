#ifndef RENDER_PERLIN_H
#define RENDER_PERLIN_H

#include <geom/Vec3.h>

namespace render
{

class Perlin
{
public:
    static float noise(const geom::Vec3& p);
    static float turbulence(const geom::Vec3& p, int depth = 7);
};

}

#endif // RENDER_PERLIN_H
