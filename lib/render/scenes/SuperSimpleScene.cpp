//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "SuperSimpleScene.h"

#include "ConstantTexture.h"
#include "GradientTexture.h"
#include "Lambertian.h"
#include "RenderableList.h"
#include "Sphere.h"

using namespace std;

namespace render
{

SuperSimpleScene::SuperSimpleScene(int width, int height)
    : Scene()
{
    auto list = std::vector<std::shared_ptr<Renderable>>();

    list.emplace_back(make_shared<Sphere>(geom::Vec3(0, -500, 0), 500, make_shared<Lambertian>(make_shared<ConstantTexture>(geom::Vec3(0.5, 0.5, 0.7)))));
    list.emplace_back(make_shared<Sphere>(geom::Vec3(0, 5, 0), 5, make_shared<Lambertian>(make_shared<ConstantTexture>(geom::Vec3(0.7, 0.5, 0.5)))));
    mRoot = std::make_shared<RenderableList>(list);
    commit();

    auto eye = geom::Vec3(0, 10, 30);
    auto lookAt = geom::Vec3(0, 0, 0);
    auto up = geom::Vec3(0, 1, 0);
    auto focusDistance = (eye - lookAt).length();
    auto aperture = 0.1f;
    mCamera = Camera(eye, lookAt, up, 45, width, height, aperture, focusDistance, 0, 1);

    mEnvMap = make_shared<EnvironmentMap>(make_shared<GradientTexture>(geom::Vec3(1.f), geom::Vec3(0.5, 0.7, 1)));
}

}
