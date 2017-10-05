//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "BookOneWithLightScene.h"

#include <geom/Utils.h>

#include "ConstantTexture.h"
#include "CheckerTexture3D.h"
#include "Dielectric.h"
#include "DiffuseLight.h"
#include "RenderableList.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Sphere.h"

using namespace std;

namespace render
{

BookOneWithLightScene::BookOneWithLightScene(int width, int height)
    : Scene()
{
    auto list = vector<shared_ptr<Renderable>>();

    // Ground
    auto tex0 = make_shared<ConstantTexture>(geom::Vec3(0.2, 0.3, 0.1));
    auto tex1 = make_shared<ConstantTexture>(geom::Vec3(0.9, 0.9, 0.9));
    auto groundTexture = make_shared<CheckerTexture3D>(tex0, tex1);
    list.emplace_back(make_shared<Sphere>(geom::Vec3(0, -1000, 0), 1000, make_shared<Lambertian>(groundTexture)));

    // Primary spheres
    list.emplace_back(make_shared<Sphere>(geom::Vec3(0, 1, 0), 1, make_shared<Dielectric>(geom::Vec3(0.9, 1, 0.9), 1.5)));
    list.emplace_back(make_shared<Sphere>(geom::Vec3(-4, 1, 0), 1, make_shared<Lambertian>(make_shared<ConstantTexture>(geom::Vec3(0.4, 0.2, 0.1)))));
    list.emplace_back(make_shared<Sphere>(geom::Vec3(4, 1, 0), 1, make_shared<Metal>(geom::Vec3(0.7, 0.6, 0.5), 0.05)));
    list.emplace_back(make_shared<Sphere>(geom::Vec3(0, 8, 2), 5, make_shared<DiffuseLight>(make_shared<ConstantTexture>(geom::Vec3(3.99, 3.99, 3.99)))));

    // Random spheres
    auto t0 = 0.f;
    auto t1 = 3.f;
    for (auto a = -11; a < 11; a++) {
        for (auto b = -11; b < 11; b++) {
            auto chooseMat = geom::rand();
            auto center = geom::Vec3(a+0.9*geom::rand(), 0.2, b+0.9*geom::rand());
            if ((center - geom::Vec3(4, 0.2, 0)).length() > 0.9) {
                if (chooseMat < 0.8) { // diffuse
                    auto color = geom::Vec3(geom::rand()*geom::rand(), geom::rand()*geom::rand(), geom::rand()*geom::rand());
                    auto m = make_shared<Lambertian>(make_shared<ConstantTexture>(color));
                    auto center1 = center + geom::Vec3(0.5-geom::rand(), 0.5*geom::rand(), 0.5-geom::rand());
                    list.emplace_back(make_shared<Sphere>(center, center1, t0, t1, 0.2, m));
                } else if (chooseMat < 0.95) { // metal
                    auto color = geom::Vec3(0.5*(1+geom::rand()), 0.5*(1+geom::rand()), 0.5*(1+geom::rand()));
                    auto m = make_shared<Metal>(color, 0.5*geom::rand());
                    list.emplace_back(make_shared<Sphere>(center, 0.2, m));
                } else { // glass
                    auto color = geom::Vec3(0.1*(9+geom::rand()), 0.1*(9+geom::rand()), 0.1*(9+geom::rand()));
                    auto m = make_shared<Dielectric>(color, 1.5);
                    list.emplace_back(make_shared<Sphere>(center, 0.2, m));
                }
            }
        }
    }
    mRoot = make_shared<RenderableList>(list);
    commit();

    auto eye = geom::Vec3(12, 2, 4);
    auto lookAt = geom::Vec3(0, 0.5, 0);
    auto up = geom::Vec3(0, 1, 0);
    auto focusDistance = (eye - lookAt).length();
    auto aperture = 0.15;
    t0 = 0.f;
    t1 = 1.f;
    mCamera = render::Camera(eye, lookAt, up, 20, width, height, aperture, focusDistance, t0, t1);

    mEnvMap = make_shared<EnvironmentMap>(make_shared<ConstantTexture>(geom::Vec3(0.01f)));
}

}
