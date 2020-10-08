//
//  RDP.h
//  rexp
//
//  Created by Ryan Hilton on 10/7/20.
//  Copyright © 2020 Ryan Hilton. All rights reserved.
//

#ifndef RDP_h
#define RDP_h

#include <stdio.h>

typedef struct RDP* RDP;

RDP makeNode(char x);
RDP make1Node(char x, RDP tree);
RDP make2Node(char x, RDP t1, RDP t2);
RDP make3Node(char x, RDP t1, RDP t2, RDP t3);
RDP make4Node(char x, RDP t1, RDP t2, RDP t3, RDP t4);

RDP E(void);
RDP ET(void);
RDP C(void);
RDP CT(void);
RDP S(void);
RDP ST(void);
RDP A(void);
RDP X(void);

void preOrder(RDP tree);
void run(void);

#endif /* RDP_h */
