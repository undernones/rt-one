//
//  Copyright © 2017 Undernones. All rights reserved.
//

#include <iostream>
#include <xmmintrin.h>
#include <pmmintrin.h>

#include <geom/Ray.h>
#include <render/Renderer.h>

int
main(int argc, const char * argv[])
{
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

    const auto NX = 800;
    const auto NY = 400;

    auto renderer = render::Renderer();

    std::cout << "P3\n" << NX << " " << NY << "\n255\n";

    auto lowerLeft = geom::Vec3(-2, -1, -1);
    auto horizontal = geom::Vec3(4, 0, 0);
    auto vertical = geom::Vec3(0, 2, 0);
    auto origin = geom::Vec3(0, 0, 0);

    for (auto j = NY - 1; j >= 0; --j) {
        for (auto i = 0; i < NX; ++i) {
            auto u = float(i) / NX;
            auto v = float(j) / NY;
            auto ray = newRay(origin, lowerLeft + u*horizontal + v*vertical);
            auto col = renderer.color(ray);
            auto ir = int(255.99*col[0]);
            auto ig = int(255.99*col[1]);
            auto ib = int(255.99*col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}
