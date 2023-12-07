#import <ApplicationServices/ApplicationServices.h>
#import "mouse_controller.h"

@implementation MouseController

- (instancetype)init
{
    self = [super init];
    if (self) {
        _width = CGDisplayPixelsHigh(kCGDirectMainDisplay);;
        _height = CGDisplayPixelsWide(kCGDirectMainDisplay);
    }
    return self;
}
- (void)moveMouseToX:(float)x Y:(float)y
{
    CGDisplayShowCursor(kCGDirectMainDisplay);
    CGPoint newMousePosition = CGPointMake(x, y);
    CGDisplayMoveCursorToPoint(kCGDirectMainDisplay, newMousePosition);
}
- (NSArray *)getCurrentMouseXY
{
    CGEventRef event = CGEventCreate(NULL);
    CGPoint cursor = CGEventGetLocation(event);
    CFRelease(event);

    NSNumber *x = [NSNumber numberWithFloat:cursor.x];
    NSNumber *y = [NSNumber numberWithFloat:cursor.y];

    return @[x, y];
}

@end
