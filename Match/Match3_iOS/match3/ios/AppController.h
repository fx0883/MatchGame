#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate>
{
    UIWindow *window;
    RootViewController    *viewController;


}

//-(void) updateInterfaceWithReachability:(Reachability*) curReach;

@end

