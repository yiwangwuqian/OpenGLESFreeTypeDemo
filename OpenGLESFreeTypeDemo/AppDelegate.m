//
//  AppDelegate.m
//  OpenGLESFreeTypeDemo
//
//  Created by guohaoyang on 2019/11/6.
//  Copyright © 2019 guohaoyang. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate
@synthesize window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    UIWindow *window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    self.window = window;
    ViewController *controller = [[ViewController alloc] init];
    window.rootViewController = controller;
    [window makeKeyAndVisible];
    
    return YES;
}

@end
