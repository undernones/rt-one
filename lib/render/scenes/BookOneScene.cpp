//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "BookOneScene.h"

#include "ConstantTexture.h"
#include "CheckerTexture3D.h"
#include "Dielectric.h"
#include "RenderableList.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Sphere.h"

namespace render
{

BookOneScene::BookOneScene(int width, int height)
    : Scene()
{
    auto list = std::vector<std::shared_ptr<Renderable>>();

    // Ground
    auto tex0 = std::make_shared<ConstantTexture>(geom::Vec3(0.2, 0.3, 0.1));
    auto tex1 = std::make_shared<ConstantTexture>(geom::Vec3(0.9, 0.9, 0.9));
    auto groundTexture = std::make_shared<CheckerTexture3D>(tex0, tex1);
    list.emplace_back(std::make_shared<Sphere>(geom::Vec3(0, -1000, 0), 1000, std::make_shared<Lambertian>(groundTexture)));

    // Primary spheres
    list.emplace_back(std::make_shared<Sphere>(geom::Vec3(0, 1, 0), 1, std::make_shared<Dielectric>(geom::Vec3(0.9, 1, 0.9), 1.5)));
    list.emplace_back(std::make_shared<Sphere>(geom::Vec3(-4, 1, 0), 1, std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(geom::Vec3(0.4, 0.2, 0.1)))));
    list.emplace_back(std::make_shared<Sphere>(geom::Vec3(4, 1, 0), 1, std::make_shared<Metal>(geom::Vec3(0.7, 0.6, 0.5), 0.05)));

    // Random spheres
    auto t0 = 0.f;
    auto t1 = 3.f;
    for (auto a = -11; a < 11; a++) {
        for (auto b = -11; b < 11; b++) {
            auto chooseMat = drand48();
            auto center = geom::Vec3(a+0.9*drand48(), 0.2, b+0.9*drand48());
            if ((center - geom::Vec3(4, 0.2, 0)).length() > 0.9) {
                if (chooseMat < 0.8) { // diffuse
                    auto color = geom::Vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48());
                    auto m = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(color));
                    auto center1 = center + geom::Vec3(0.5-drand48(), 0.5*drand48(), 0.5-drand48());
                    list.emplace_back(std::make_shared<Sphere>(center, center1, t0, t1, 0.2, m));
                } else if (chooseMat < 0.95) { // metal
                    auto color = geom::Vec3(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1+drand48()));
                    auto m = std::make_shared<Metal>(color, 0.5*drand48());
                    list.emplace_back(std::make_shared<Sphere>(center, 0.2, m));
                } else { // glass
                    auto color = geom::Vec3(0.1*(9+drand48()), 0.1*(9+drand48()), 0.1*(9+drand48()));
                    auto m = std::make_shared<Dielectric>(color, 1.5);
                    list.emplace_back(std::make_shared<Sphere>(center, 0.2, m));
                }
            }
        }
    }
    mRoot = std::make_shared<RenderableList>(list);

    commit();

    auto eye = geom::Vec3(12, 2, 4);
    auto lookAt = geom::Vec3(0, 0.5, 0);
    auto up = geom::Vec3(0, 1, 0);
    auto focusDistance = (eye - lookAt).length();
    auto aperture = 0.15;
    t0 = 0.f;
    t1 = 1.f;
    mCamera = render::Camera(eye, lookAt, up, 20, width, height, aperture, focusDistance, t0, t1);
}

}
