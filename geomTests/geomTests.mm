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

- (void)testPolarCartesianRoundTrip
{
    auto v = geom::Vec3(256, 5, -130);
    auto polar = geom::cartesianToPolar(v);
    auto cartesian = geom::polarToCartesian(polar) * v.length();

    auto stream = std::stringstream();
    stream << "Reference: " << v << ", Computed: " << cartesian;
    XCTAssert(v == cartesian, @"%s", stream.str().c_str());
}

- (void)testPolarCartesianRoundTripMultipleTimes
{
    const auto v = geom::Vec3(0, 1, 14);
    auto polar = geom::cartesianToPolar(v);
    auto cartesian = geom::polarToCartesian(polar) * v.length();

    for (auto i = 0; i < 200; ++i) {
        XCTAssert(v == cartesian, @"Iteration: %d, Reference: %@, Computed: %@", i, vectorToString(v), vectorToString(cartesian));
        polar = geom::cartesianToPolar(cartesian);
        cartesian = geom::polarToCartesian(polar) * v.length();
    }
}

@end
