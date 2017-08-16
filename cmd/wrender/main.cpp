//
//  Copyright © 2017 Undernones. All rights reserved.
//

#include <iostream>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

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

int
main(int argc, const char * argv[])
{
    auto device = DeviceHandle();
    if (!device) {
        std::cerr << "Failed to initialize device" << std::endl;
        return -1;
    }
    
    std::cout << "Hello, World!\n";
    return 0;
}
