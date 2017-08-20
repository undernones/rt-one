//
//  Copyright © 2017 Undernones. All rights reserved.
//

#include <iostream>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include <xmmintrin.h>
#include <pmmintrin.h>

#include <geom/Ray.h>
#include <geom/Sphere.h>
#include <geom/Vec3.h>

class DeviceHandle
{
public:
    DeviceHandle(const char* cfg = NULL) : DeviceHandle(rtcNewDevice(cfg)) {}
    DeviceHandle(RTCDevice device) : mDevice(device) {}

    ~DeviceHandle() {
        if (mDevice != nullptr) {
            rtcDeleteDevice(mDevice);
        }
    }

    operator RTCDevice() const { return mDevice; }
    operator bool() const { return mDevice != nullptr; }

private:
    RTCDevice mDevice;
};

class SceneHandle
{
public:
    SceneHandle(RTCDevice device, RTCSceneFlags flags, RTCAlgorithmFlags aflags) : SceneHandle(rtcDeviceNewScene(device, flags, aflags)) {}
    SceneHandle(RTCScene scene) : mScene(scene) {}

    ~SceneHandle() {
        if (mScene != nullptr) {
            rtcDeleteScene(mScene);
        }
    }

    operator RTCScene() const { return mScene; }
    operator bool() const { return mScene != nullptr; }

private:
    RTCScene mScene;
};

std::string
EmbreeErrorToString(RTCError error)
{
    switch (error) {
        case RTC_NO_ERROR:
            return "No error";

        case RTC_UNKNOWN_ERROR:
            return "Unknown";

        case RTC_INVALID_ARGUMENT:
            return "Invalid argument";

        case RTC_INVALID_OPERATION:
            return "Invalid operation";

        case RTC_OUT_OF_MEMORY:
            return "Out of memory";

        case RTC_UNSUPPORTED_CPU:
            return "Unsupported CPU";

        case RTC_CANCELLED:
            return "Cancelled";
    }
}

void
RTCSphereBoundsFunc(void* userPtr,         /*!< pointer to user data */
                    void* geomUserPtr,     /*!< pointer to geometry user data */
                    size_t item,           /*!< item to calculate bounds for */
                    RTCBounds* bounds_o    /*!< returns calculated bounds */)
{
    // Assume we can dereference geomUserPtr
    const auto& sphere = *(static_cast<geom::Sphere*>(userPtr));

    auto lower = sphere.center() - geom::Vec3(sphere.radius());
    auto upper = sphere.center() + geom::Vec3(sphere.radius());

    bounds_o->lower_x = lower.x();
    bounds_o->lower_y = lower.y();
    bounds_o->lower_z = lower.z();

    bounds_o->upper_x = upper.x();
    bounds_o->upper_y = upper.y();
    bounds_o->upper_z = upper.z();
}

void
RTCSphereIntersectFunc(void* ptr,           /*!< pointer to user data */
                       RTCRay& ray,         /*!< ray to intersect */
                       size_t item          /*!< item to intersect */)
{
    // Assume we can dereference ptr
    const auto& sphere = *(static_cast<geom::Sphere*>(ptr));
    if (sphere.hit(ray)) {
        ray.geomID = static_cast<unsigned int>(item);
    }
}

geom::Vec3
color(RTCScene scene, RTCRay ray)
{
    rtcIntersect(scene, ray);
    if (ray.geomID != RTC_INVALID_GEOMETRY_ID) {
        return 0.5 * geom::Vec3(ray.Ng[0]+1, ray.Ng[1]+1, ray.Ng[2]+1);
    }
    auto unitDir = geom::Vec3(ray.dir).normalized();
    auto t = 0.5 * (unitDir.y() + 1.0);
    return (1.0 - t) * geom::Vec3(1, 1, 1) + t * geom::Vec3(0.5, 0.7, 1);
}

int
main(int argc, const char * argv[])
{
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

    const auto NX = 800;
    const auto NY = 400;

    auto device = DeviceHandle();
    if (!device) {
        auto error = rtcDeviceGetError(NULL);
        std::cerr << "Failed to initialize device: " << EmbreeErrorToString(error) << std::endl;
        return error;
    }

    auto scene = SceneHandle(device, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);
    if (!scene) {
        auto error = rtcDeviceGetError(device);
        std::cerr << "Failed to initialize device: " << EmbreeErrorToString(error) << std::endl;
        return error;
    }

    auto sphereId = rtcNewUserGeometry3(scene, RTC_GEOMETRY_STATIC, 1);
    auto sphere = geom::Sphere(geom::Vec3(0, 0, -1), 0.5);
    rtcSetUserData(scene, sphereId, &sphere);
    rtcSetBoundsFunction2(scene, sphereId, RTCSphereBoundsFunc, &sphere);
    rtcSetIntersectFunction(scene, sphereId, RTCSphereIntersectFunc);

    rtcCommit(scene);

    std::cout << "P3\n" << NX << " " << NY << "\n255\n";

    auto lowerLeft = geom::Vec3(-2, -1, -1);
    auto horizontal = geom::Vec3(4, 0, 0);
    auto vertical = geom::Vec3(0, 2, 0);
    auto origin = geom::Vec3(0, 0, 0);

    for (auto j = NY - 1; j >= 0; --j) {
        for (auto i = 0; i < NX; ++i) {
            auto u = float(i) / NX;
            auto v = float(j) / NY;
            auto ray = newRay(origin, lowerLeft + u*horizontal + v*vertical);
            auto col = color(scene, ray);
            auto ir = int(255.99*col[0]);
            auto ig = int(255.99*col[1]);
            auto ib = int(255.99*col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}
