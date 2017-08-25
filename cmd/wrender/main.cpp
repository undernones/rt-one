//
//  Copyright © 2017 Undernones. All rights reserved.
//

#include <iostream>
#include <xmmintrin.h>
#include <pmmintrin.h>

#include <geom/Ray.h>
#include <render/Camera.h>
#include <render/Renderer.h>

int
main(int argc, const char * argv[])
{
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

    const auto NX = 800;
    const auto NY = 400;
    const auto NS = 50;

    auto renderer = render::Renderer();

    std::cout << "P3\n" << NX << " " << NY << "\n255\n";

    auto origin = geom::Vec3(-2, 2, 1);
    auto lookAt = geom::Vec3(0, 0, -1);
    auto up = geom::Vec3(0, 1, 0);
    auto camera = render::Camera(origin, lookAt, up, 90, NX, NY);

    for (auto row = NY - 1; row >= 0; --row) {
        for (auto col = 0; col < NX; ++col) {
            auto color = geom::Vec3(0, 0, 0);
            for (auto sample = 0; sample < NS; ++sample) {
                auto s = (col + drand48()) / NX;
                auto t = (row + drand48()) / NY;
                auto ray = camera.getRay(s, t);

                color += renderer.color(ray);
            }
            color /= NS;
            color = geom::Vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
            auto ir = int(255.99*color[0]);
            auto ig = int(255.99*color[1]);
            auto ib = int(255.99*color[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}
