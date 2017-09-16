//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "RectLightScene.h"

#include "ConstantTexture.h"
#include "DiffuseLight.h"
#include "RenderableList.h"
#include "Lambertian.h"
#include "PerlinTexture.h"
#include "Rectangle.h"
#include "Sphere.h"

namespace render
{

RectLightScene::RectLightScene(int width, int height)
    : Scene()
{
    auto list = std::vector<std::shared_ptr<Renderable>>();

    auto texture = std::make_shared<PerlinTexture>(4);
    auto material = std::make_shared<Lambertian>(texture);
    auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(geom::Vec3(4, 4, 4)));

    list.emplace_back(std::make_shared<Sphere>(geom::Vec3(0, -1000, 0), 1000, material));
    list.emplace_back(std::make_shared<Sphere>(geom::Vec3(0, 2, 0), 2, material));
    list.emplace_back(std::make_shared<Rectangle>(Plane::XY, 3, 5, 1, 3, -2, light));
    list.emplace_back(std::make_shared<Sphere>(geom::Vec3(0, 7, 0), 2, light));
    mRoot = std::make_shared<RenderableList>(list);

    commit();

    auto eye = geom::Vec3(15, 4, 3);
    auto lookAt = geom::Vec3(0, 2.5, 0);
    auto up = geom::Vec3(0, 1, 0);
    auto focusDistance = (eye - lookAt).length();
    auto aperture = 0.1f;
    auto t0 = 0.f;
    auto t1 = 1.f;
    mCamera = render::Camera(eye, lookAt, up, 30, width, height, aperture, focusDistance, t0, t1);

    mBgIntensity = 0.f;
}

}
