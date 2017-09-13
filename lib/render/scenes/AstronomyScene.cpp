//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "AstronomyScene.h"

#include <memory>

#include "CheckerTexture3D.h"
#include "ConstantTexture.h"
#include "RenderableList.h"
#include "ImageTexture.h"
#include "Lambertian.h"
#include "Sphere.h"

namespace render
{

AstronomyScene::AstronomyScene(int width, int height)
    : Scene()
{
    auto list = std::vector<std::shared_ptr<Renderable>>();

    auto earthTexture = ImageTexture::loadFromFile("earthmap_hires.jpg");
    auto earthMaterial = std::make_shared<Lambertian>(earthTexture);

    auto moonTexture = ImageTexture::loadFromFile("moon_map_mercator.jpg");
    auto moonMaterial = std::make_shared<Lambertian>(moonTexture);

    auto tex0 = std::make_shared<ConstantTexture>(geom::Vec3(0.2, 0.3, 0.1));
    auto tex1 = std::make_shared<ConstantTexture>(geom::Vec3(0.9, 0.9, 0.9));
    auto groundTexture = std::make_shared<CheckerTexture3D>(tex0, tex1);
    auto lambertGround = std::make_shared<Lambertian>(groundTexture);

    list.emplace_back(std::make_shared<Sphere>(geom::Vec3(0, 0, 0), 2, earthMaterial));
    list.emplace_back(std::make_shared<Sphere>(geom::Vec3(0, -1002, 0), 1000, lambertGround));
    list.emplace_back(std::make_shared<Sphere>(geom::Vec3(0.3, 0, 4), 0.5, moonMaterial));
    mRoot = std::make_shared<RenderableList>(list);

    commit();

    auto eye = geom::Vec3(15, 2, 3);
    auto lookAt = geom::Vec3(0, 0, 1);
    auto up = geom::Vec3(0, 1, 0);
    auto focusDistance = (eye - lookAt).length();
    auto aperture = 0.1f;
    auto t0 = 0.f;
    auto t1 = 1.f;
    mCamera = render::Camera(eye, lookAt, up, 20, width, height, aperture, focusDistance, t0, t1);
}

}
