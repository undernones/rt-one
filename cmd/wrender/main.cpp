//
//  Copyright © 2017 Undernones. All rights reserved.
//

#include <iostream>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include <xmmintrin.h>
#include <pmmintrin.h>

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

int
main(int argc, const char * argv[])
{
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

    auto device = DeviceHandle();
    if (!device) {
        auto error = rtcDeviceGetError(NULL);
        std::cerr << "Failed to initialize device: " << EmbreeErrorToString(error) << std::endl;
        return error;
    }

    std::cout << "Hello, World!\n";
    return 0;
}
