//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "BookTwoScene.h"

#include "Box.h"
#include "ConstantMedium.h"
#include "ConstantTexture.h"
#include "Dielectric.h"
#include "DiffuseLight.h"
#include "ImageTexture.h"
#include "Lambertian.h"
#include "Metal.h"
#include "PerlinTexture.h"
#include "Rectangle.h"
#include "RenderableList.h"
#include "Rotate.h"
#include "Sphere.h"
#include "Translate.h"

using namespace std;

namespace render
{

BookTwoScene::BookTwoScene(int width, int height)
{
    auto list = std::vector<std::shared_ptr<Renderable>>();

    auto white = make_shared<Lambertian>(make_shared<ConstantTexture>(geom::Vec3(0.73, 0.73, 0.73)));
    auto ground = make_shared<Lambertian>(make_shared<ConstantTexture>(geom::Vec3(0.48, 0.83, 0.53)));

    const auto nb = 20;
    for (auto i = 0; i < nb; ++i) {
        for (auto j = 0; j < nb; ++j) {
            auto w = 100.f;
            auto x0 = -1000 + i*w;
            auto z0 = -1000 + j*w;
            auto y0 = 0.f;
            auto x1 = x0 + w;
            auto y1 = 100*(drand48()+0.01);
            auto z1 = z0 + w;
            list.emplace_back(make_shared<Box>(geom::Vec3(x0, y0, z0), geom::Vec3(x1, y1, z1), ground));
        }
    }

    auto light = make_shared<DiffuseLight>(make_shared<ConstantTexture>(geom::Vec3(7.f)));
    list.emplace_back(make_shared<Rectangle>(Plane::XZ, 123, 423, 147, 412, 554, light));

    auto center = geom::Vec3(400, 400, 200);
    list.emplace_back(make_shared<Sphere>(center, center+geom::Vec3(30, 0, 0), 0, 1, 50, make_shared<Lambertian>(make_shared<ConstantTexture>(geom::Vec3(0.7, 0.3, 0.1)))));

    list.emplace_back(make_shared<Sphere>(geom::Vec3(260, 150, 45), 50, make_shared<Dielectric>(geom::Vec3(1.f), 1.5)));

    list.emplace_back(make_shared<Sphere>(geom::Vec3(0, 150, 145), 50, make_shared<Metal>(geom::Vec3(0.8, 0.8, 0.9), 10)));

    auto dielectricBoundary = make_shared<Sphere>(geom::Vec3(360, 150, 145), 70, make_shared<Dielectric>(geom::Vec3(1.f), 1.5));
    list.push_back(dielectricBoundary);
    list.emplace_back(make_shared<ConstantMedium>(dielectricBoundary, 0.2, make_shared<ConstantTexture>(geom::Vec3(0.2, 0.4, 0.9))));

    auto sceneBoundary = make_shared<Sphere>(geom::Vec3(0.f), 5000, make_shared<Dielectric>(geom::Vec3(1.f), 1.5));
    list.emplace_back(make_shared<ConstantMedium>(sceneBoundary, 0.0001, make_shared<ConstantTexture>(geom::Vec3(1.f))));

    auto earthTexture = ImageTexture::loadFromFile("earthmap_hires.jpg");
    auto earthMaterial = make_shared<Lambertian>(earthTexture);
    list.emplace_back(make_shared<Sphere>(geom::Vec3(400, 200, 400), 100, earthMaterial));

    auto perlinTexture = make_shared<PerlinTexture>(0.1);
    auto perlinMaterial = make_shared<Lambertian>(perlinTexture);
    list.emplace_back(make_shared<Sphere>(geom::Vec3(220, 280, 300), 80, perlinMaterial));

    const auto ns = 1000;
    auto boxList2 = vector<shared_ptr<Renderable>>();
    boxList2.reserve(ns);
    for (auto j = 0; j < ns; ++j) {
        boxList2.emplace_back(make_shared<Sphere>(geom::Vec3(165*drand48(), 165*drand48(), 165*drand48()), 10, white));
    }
    list.emplace_back(make_shared<Translate>(geom::Vec3(-100, 270, 395), make_shared<Rotate>(geom::Vec3(0, 1, 0), 15, make_shared<RenderableList>(boxList2))));

    mRoot = std::make_shared<RenderableList>(list);
    commit();

    auto eye = geom::Vec3(378, 280, -610);
    auto lookAt = geom::Vec3(278, 240, 0);
    auto up = geom::Vec3(0, 1, 0);
    auto focusDistance = 10.f;
    auto aperture = 0.f;
    auto t0 = 0.f;
    auto t1 = 1.f;
    mCamera = Camera(eye, lookAt, up, 40, width, height, aperture, focusDistance, t0, t1);

    mBgIntensity = 0.f;
}

}
