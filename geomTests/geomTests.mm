//
//  Copyright © 2017 Undernones. All rights reserved.
//

#import <XCTest/XCTest.h>
#import <geom/Utils.h>
#import <sstream>

@interface geomTests : XCTestCase

@end

NSString *vectorToString(const geom::Vec3& v)
{
    auto stream = std::stringstream();
    stream << v;
    auto str = stream.str().c_str();
    return [NSString stringWithUTF8String:str];
}

@implementation geomTests

- (void)testSphericalCartesianRoundTrip
{
    auto v = geom::Vec3(256, 5, -130);
    auto spherical = geom::cartesianToSpherical(v);
    auto cartesian = geom::sphericalToCartesian(spherical);

    auto stream = std::stringstream();
    stream << "Reference: " << v << ", Computed: " << cartesian;
    XCTAssert(v == cartesian, @"%s", stream.str().c_str());
}

- (void)testSphericalCartesianRoundTripMultipleTimes
{
    const auto v = geom::Vec3(0, 1, 14);
    auto spherical = geom::cartesianToSpherical(v);
    auto cartesian = geom::sphericalToCartesian(spherical);

    for (auto i = 0; i < 200; ++i) {
        XCTAssert(v == cartesian, @"Iteration: %d, Reference: %@, Computed: %@", i, vectorToString(v), vectorToString(cartesian));
        spherical = geom::cartesianToSpherical(cartesian);
        cartesian = geom::sphericalToCartesian(spherical);
    }
}

@end
