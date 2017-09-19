//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "TeapotScene.h"

#include <geom/TriangleMesh.h>

#include "ConstantMedium.h"
#include "ConstantTexture.h"
#include "Dielectric.h"
#include "DiffuseLight.h"
#include "FlipNormals.h"
#include "Lambertian.h"
#include "Mesh.h"
#include "Metal.h"
#include "Rectangle.h"
#include "RenderableList.h"
#include "Rotate.h"
#include "Sphere.h"
#include "Translate.h"

using namespace std;

namespace render
{

TeapotScene::TeapotScene(int width, int height)
    : Scene()
{
    auto list = std::vector<std::shared_ptr<Renderable>>();

    auto teapotTexture = make_shared<ConstantTexture>(geom::Vec3(0.793, 0.03, 0.081));
    auto teapotMaterial = make_shared<Lambertian>(teapotTexture);
    auto metalMaterial = make_shared<Metal>(geom::Vec3(0.7, 0.6, 0.5), 0.05);

    auto triMesh = make_shared<geom::TriangleMesh>("obj/teapot.obj");
    auto mesh = make_shared<Mesh>(triMesh, teapotMaterial);
    list.emplace_back(make_shared<Rotate>(geom::Vec3(1, 0, 0), -90, mesh));
    list.emplace_back(make_shared<Rectangle>(Plane::XZ, -600, 600, -600, 600, 0.5, metalMaterial));

    auto otherPotMaterial = make_shared<Lambertian>(make_shared<ConstantTexture>(geom::Vec3(0, 0.2, 0.7)));
    auto otherMesh = make_shared<Rotate>(geom::Vec3(1, 0, 0), -90, make_shared<Mesh>(triMesh, otherPotMaterial));
    for (auto i = -10; i <= 10; ++i) {
        for (auto j = -10; j <= 10; ++j) {
            if (i == 0 && j == 0) continue;
            auto position = geom::Vec3(40 * i, 0, 40 * j);
            list.emplace_back(make_shared<Translate>(position, otherMesh));
        }
    }

    // Make a light and put a bit of a frame around it
    const auto LIGHT_LEFT = -12;
    const auto LIGHT_RIGHT = 12;
    const auto LIGHT_BACK = -12;
    const auto LIGHT_FRONT = 12;
    const auto LIGHT_TOP = 95;
    const auto LIGHT_BOX_HEIGHT = 15;
    auto lightBoxTexture = make_shared<ConstantTexture>(geom::Vec3(0.01f));
    auto lightBoxMaterial = make_shared<Lambertian>(lightBoxTexture);
    auto light = make_shared<DiffuseLight>(make_shared<ConstantTexture>(geom::Vec3(21.f)));
    list.emplace_back(make_shared<Rectangle>(Plane::XZ, LIGHT_LEFT, LIGHT_RIGHT, LIGHT_BACK, LIGHT_FRONT, LIGHT_TOP, light));
    list.emplace_back(make_shared<Rectangle>(Plane::XY, LIGHT_LEFT, LIGHT_RIGHT, LIGHT_TOP-LIGHT_BOX_HEIGHT, LIGHT_TOP, LIGHT_BACK, lightBoxMaterial));
    list.emplace_back(make_shared<Rectangle>(Plane::YZ, LIGHT_TOP-LIGHT_BOX_HEIGHT, LIGHT_TOP, LIGHT_BACK, LIGHT_FRONT, LIGHT_LEFT, lightBoxMaterial));
    list.emplace_back(make_shared<FlipNormals>(make_shared<Rectangle>(Plane::YZ, LIGHT_TOP-LIGHT_BOX_HEIGHT, LIGHT_TOP, LIGHT_BACK, LIGHT_FRONT, LIGHT_RIGHT, lightBoxMaterial)));
    list.emplace_back(make_shared<FlipNormals>(make_shared<Rectangle>(Plane::XY, LIGHT_LEFT, LIGHT_RIGHT, LIGHT_TOP-LIGHT_BOX_HEIGHT, LIGHT_TOP, LIGHT_FRONT, lightBoxMaterial)));

    auto sceneBoundary = make_shared<Sphere>(geom::Vec3(0.f), 5000, make_shared<Dielectric>(geom::Vec3(1.f), 1.5));
    list.emplace_back(make_shared<ConstantMedium>(sceneBoundary, 0.0001, make_shared<ConstantTexture>(geom::Vec3(1.f))));

    mRoot = std::make_shared<RenderableList>(list);
    commit();

    auto eye = geom::Vec3(0, 90, -130);
    auto lookAt = geom::Vec3(0, 0, -10);
    auto up = geom::Vec3(0, 1, 0);
    auto focusDistance = (eye - lookAt).length();
    auto aperture = 0.f;
    mCamera = Camera(eye, lookAt, up, 55, width, height, aperture, focusDistance, 0, 1);

    mBgIntensity = 0.5f;
}

}
