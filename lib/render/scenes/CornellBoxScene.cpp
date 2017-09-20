//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "CornellBoxScene.h"

#include "Box.h"
#include "ConstantTexture.h"
#include "DiffuseLight.h"
#include "FlipNormals.h"
#include "RenderableList.h"
#include "Lambertian.h"
#include "Rectangle.h"
#include "Rotate.h"
#include "Translate.h"

using namespace std;

namespace render
{

CornellBoxScene::CornellBoxScene(int width, int height)
    : Scene()
{
    auto list = std::vector<std::shared_ptr<Renderable>>();

    auto redTex = make_shared<ConstantTexture>(geom::Vec3(0.65, 0.05, 0.05));
    auto whiteTex = make_shared<ConstantTexture>(geom::Vec3(0.73, 0.73, 0.73));
    auto greenTex = make_shared<ConstantTexture>(geom::Vec3(0.12, 0.45, 0.15));

    auto red = make_shared<Lambertian>(redTex);
    auto white = make_shared<Lambertian>(whiteTex);
    auto green = make_shared<Lambertian>(greenTex);
    auto light = make_shared<DiffuseLight>(make_shared<ConstantTexture>(geom::Vec3(15, 15, 15)));

    // Walls
    list.emplace_back(make_shared<FlipNormals>(make_shared<Rectangle>(Plane::YZ, 0, 555, 0, 555, 555, green)));
    list.emplace_back(make_shared<Rectangle>(Plane::YZ, 0, 555, 0, 555, 0, red));
    list.emplace_back(make_shared<Rectangle>(Plane::XZ, 0, 555, 0, 555, 0, white));
    list.emplace_back(make_shared<FlipNormals>(make_shared<Rectangle>(Plane::XZ, 0, 555, 0, 555, 555, white)));
    list.emplace_back(make_shared<FlipNormals>(make_shared<Rectangle>(Plane::XY, 0, 555, 0, 555, 555, white)));

    // Light
    list.emplace_back(make_shared<Rectangle>(Plane::XZ, 213, 343, 227, 332, 554.99, light));

    // Boxes
    list.emplace_back(make_shared<Translate>(geom::Vec3(130, 0, 65), make_shared<Rotate>(geom::Vec3(0, 1, 0), -18, make_shared<Box>(geom::Vec3(0, 0, 0), geom::Vec3(165, 165, 165), white))));
    list.emplace_back(make_shared<Translate>(geom::Vec3(265, 0, 295), make_shared<Rotate>(geom::Vec3(0, 1, 0), 15, make_shared<Box>(geom::Vec3(0, 0, 0), geom::Vec3(165, 330, 165), white))));

    mRoot = std::make_shared<RenderableList>(list);
    commit();

    auto eye = geom::Vec3(278, 278, -800);
    auto lookAt = geom::Vec3(278, 278, 0);
    auto up = geom::Vec3(0, 1, 0);
    auto focusDistance = 10.f;
    auto aperture = 0.f;
    auto t0 = 0.f;
    auto t1 = 1.f;
    mCamera = render::Camera(eye, lookAt, up, 40, width, height, aperture, focusDistance, t0, t1);

    // No environment light
}

}
