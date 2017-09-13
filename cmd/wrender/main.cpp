//
//  Copyright © 2017 Undernones. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <tbb/tbb.h>
#include <xmmintrin.h>
#include <pmmintrin.h>

#include <render/Ray.h>
#include <render/BookOneScene.h>
#include <render/Renderer.h>

#if DEBUG
#define PARALLEL 0
#else
#define PARALLEL 1
#endif

int
main(int argc, const char * argv[])
{
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

    const auto NX = 800;
    const auto NY = 400;
    const auto NS = 50;

    auto scene = render::BookOneScene(NX, NY);
    const auto& camera = scene.camera();

    std::cout << "P3\n" << NX << " " << NY << "\n255\n";

    for (auto row = NY - 1; row >= 0; --row) {
        for (auto col = 0; col < NX; ++col) {
            auto color = geom::Vec3(0, 0, 0);
#if PARALLEL
            tbb::parallel_for(int(0), NS, int(1), [&](int sample) {
#else
                for (auto sample = 0; sample < NS; ++sample) {
#endif
                auto s = (col + drand48()) / NX;
                auto t = (row + drand48()) / NY;
                auto ray = camera.getRay(s, t);

                color += render::Renderer::trace(ray, scene);
#if PARALLEL
                });
#else
            }
#endif
            color /= NS;
            color = geom::Vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
            auto ir = std::clamp(int(255.99*color[0]), 0, 255);
            auto ig = std::clamp(int(255.99*color[1]), 0, 255);
            auto ib = std::clamp(int(255.99*color[2]), 0, 255);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}
