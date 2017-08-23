//
//  Copyright © 2017 Undernones. All rights reserved.
//

#import "AppDelegate.h"

// TODO: get these values from [[NSProcessInfo processInfo] arguments]
static const int WIDTH = 800;
static const int HEIGHT = 400;

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    NSApplication *application = (NSApplication *)[notification object];

    CGSize size = CGSizeMake(WIDTH, HEIGHT);
    [[application mainWindow] setContentSize:size];
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    // Insert code here to tear down your application
}


@end
