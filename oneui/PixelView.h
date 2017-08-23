//
//  Copyright © 2017 Undernones. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface PixelView : NSView

- (void)setBytes:(const void *)bytes width:(int)width height:(int)height;

@end

NS_ASSUME_NONNULL_END
