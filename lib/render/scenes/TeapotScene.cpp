//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "TeapotScene.h"

#include <geom/TriangleMesh.h>

#include "ConstantMedium.h"
#include "ConstantTexture.h"
#include "Dielectric.h"
#include "DiffuseLight.h"
#include "Lambertian.h"
#include "Mesh.h"
#include "Metal.h"
#include "Rectangle.h"
#include "RenderableList.h"
#include "Rotate.h"
#include "Sphere.h"

using namespace std;

namespace render
{

TeapotScene::TeapotScene(int width, int height)
    : Scene()
{
    auto list = std::vector<std::shared_ptr<Renderable>>();

    auto texture = make_shared<ConstantTexture>(geom::Vec3(0.793, 0.03, 0.081));
    auto teapotMaterial = make_shared<Lambertian>(texture);
    auto metalMaterial = make_shared<Metal>(geom::Vec3(0.7, 0.6, 0.5), 0.05);
    auto light = make_shared<DiffuseLight>(make_shared<ConstantTexture>(geom::Vec3(7.f)));

    auto triMesh = make_shared<geom::TriangleMesh>("/Users/stephen_ward/code/wray/scenes/obj/teapot.obj");
    auto mesh = make_shared<Mesh>(triMesh, teapotMaterial);
    list.emplace_back(make_shared<Rotate>(geom::Vec3(1, 0, 0), -90, mesh));
    list.emplace_back(make_shared<Rectangle>(Plane::XZ, -600, 600, -600, 600, 0.5, metalMaterial));
    list.emplace_back(make_shared<Rectangle>(Plane::XZ, -15, 15, -15, 15, 35, light));

    auto sceneBoundary = make_shared<Sphere>(geom::Vec3(0.f), 5000, make_shared<Dielectric>(geom::Vec3(1.f), 1.5));
    list.emplace_back(make_shared<ConstantMedium>(sceneBoundary, 0.0001, make_shared<ConstantTexture>(geom::Vec3(1.f))));

    mRoot = std::make_shared<RenderableList>(list);
    commit();

    auto eye = geom::Vec3(0, 10, 60);
    auto lookAt = geom::Vec3(0, 10, 0);
    auto up = geom::Vec3(0, 1, 0);
    auto focusDistance = (eye - lookAt).length();
    auto aperture = 0.f;
    mCamera = Camera(eye, lookAt, up, 45, width, height, aperture, focusDistance, 0, 1);
}

}
