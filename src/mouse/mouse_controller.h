#import <Foundation/Foundation.h>

// Objective-C code to manipulate the mouse
@interface MouseController : NSObject
@property(nonatomic, assign) size_t width;
@property(nonatomic, assign) size_t height;
- (instancetype)init;
- (void)moveMouseToX:(float)x Y:(float)y;
- (NSArray *)getCurrentMouseXY;
@end
