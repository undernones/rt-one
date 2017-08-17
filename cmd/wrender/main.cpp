//
//  Copyright © 2017 Undernones. All rights reserved.
//

#include <iostream>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include <xmmintrin.h>
#include <pmmintrin.h>

#include <geom/Ray.h>
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

bool
hitSphere(const geom::Vec3& center, float radius, const geom::Ray& r)
{
    auto oc = r.origin() - center;
    auto a = r.direction().dot(r.direction());
    auto b = 2.0 * oc.dot(r.direction());
    auto c = oc.dot(oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return discriminant > 0;
}

geom::Vec3
color(const geom::Ray& r)
{
    if (hitSphere(geom::Vec3(0, 0, -1), 0.5, r)) {
        return geom::Vec3(1, 0, 0);
    }
    auto unitDir = r.direction().normalized();
    auto t = 0.5 * (unitDir.y() + 1.0);
    return (1.0 - t) * geom::Vec3(1, 1, 1) + t * geom::Vec3(0.5, 0.7, 1);
}

int
main(int argc, const char * argv[])
{
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

    const auto NX = 200;
    const auto NY = 100;

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

    std::cout << "P3\n" << NX << " " << NY << "\n255\n";

    auto lowerLeft = geom::Vec3(-2, -1, -1);
    auto horizontal = geom::Vec3(4, 0, 0);
    auto vertical = geom::Vec3(0, 2, 0);
    auto origin = geom::Vec3(0, 0, 0);

    for (auto j = NY - 1; j >= 0; --j) {
        for (auto i = 0; i < NX; ++i) {
            auto u = float(i) / NX;
            auto v = float(j) / NY;
            auto r = geom::Ray(origin, lowerLeft + u*horizontal + v*vertical);
            auto col = color(r);
            auto ir = int(255.99*col[0]);
            auto ig = int(255.99*col[1]);
            auto ib = int(255.99*col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}
