//
//  Copyright © 2017 Undernones. All rights reserved.
//

#import <XCTest/XCTest.h>
#import <geom/Utils.h>

@interface geomTests : XCTestCase

@end

@implementation geomTests

- (void)testExample
{
    auto toCamera = geom::Vec3(2048, 5, -130);
    auto polar = geom::cartesianToPolar(toCamera);
    auto cartesian = geom::polarToCartesian(polar) * toCamera.length();
    XCTAssertEqualWithAccuracy(toCamera.x(), cartesian.x(), 0.01);
    XCTAssertEqualWithAccuracy(toCamera.y(), cartesian.y(), 0.01);
    XCTAssertEqualWithAccuracy(toCamera.z(), cartesian.z(), 0.01);
}

@end
