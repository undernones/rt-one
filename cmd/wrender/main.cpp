//
//  Copyright © 2017 Undernones. All rights reserved.
//

#include <iostream>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

int
main(int argc, const char * argv[])
{
    RTCDevice device = rtcNewDevice(NULL);
    rtcDeleteDevice(device);
    std::cout << "Hello, World!\n";
    return 0;
}
