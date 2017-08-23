//
//  Copyright © 2017 Undernones. All rights reserved.
//

#import "ViewController.h"
#import "PixelView.h"

#import <render/Image.h>

@implementation ViewController

- (PixelView *)pixelView
{
    return (PixelView *)[self view];
}

- (void)viewDidLayout
{
    // Check to see if we have a scene
    auto size = [[self view] frame].size;
    auto rows = static_cast<int>(size.height);
    auto cols = static_cast<int>(size.width);
    auto image = render::Image(rows, cols);

    for (auto r = 0; r < rows; ++r) {
        for (auto c = 0; c < cols; ++c) {
            auto tmp = 0xFF00FF00;
            image.setValue(r, c, tmp);
        }
    }
    [[self pixelView] setBytes:image.bytes() width:cols height:rows];
}

@end
