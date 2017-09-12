//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_PERLIN_H
#define TROUBLE_PERLIN_H

#include <geom/Vec3.h>

class Perlin
{
public:
    static float noise(const geom::Vec3& p);
    static float turbulence(const geom::Vec3& p, int depth = 7);
};

#endif // TROUBLE_PERLIN_H
