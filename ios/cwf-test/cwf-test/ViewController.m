//
//  ViewController.m
//  cwf-test
//
//  Created by vty on 11/17/15.
//  Copyright © 2015 vty. All rights reserved.
//

#import "ViewController.h"
//#import <cwf/cwf.h>
#import "cwf/cwf.h"
@interface ViewController ()

@end

void log_test();
void netw_test();
void proc_test();

void testAll() {
    log_test();
    netw_test();
    proc_test();
}

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    testAll();
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
