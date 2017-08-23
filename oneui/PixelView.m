//
//  Copyright © 2017 Undernones. All rights reserved.
//

#import "PixelView.h"

#import <AppKit/AppKit.h>

@implementation PixelView
{
    NSBitmapImageRep *_image;
}

- (void)setBytes:(const void *)bytes width:(int)width height:(int)height
{
    _image = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:(unsigned char **)&bytes
                                                     pixelsWide:width
                                                     pixelsHigh:height
                                                  bitsPerSample:8
                                                samplesPerPixel:4
                                                       hasAlpha:YES
                                                       isPlanar:NO
                                                 colorSpaceName:NSDeviceRGBColorSpace
                                                    bytesPerRow:width * 4
                                                   bitsPerPixel:32];
}

- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];
    [_image drawInRect:dirtyRect];
}

@end
