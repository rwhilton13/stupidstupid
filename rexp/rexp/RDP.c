//
//  RDP.c
//  rexp
//
//  Created by Ryan Hilton on 10/7/20.
//  Copyright © 2020 Ryan Hilton. All rights reserved.
//

#include "RDP.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

struct RDP {
    char label;
    RDP leftmostChild;
    RDP rightSibling;
};

RDP parseTree;
char *expr;
int indents;

RDP makeNode(char x) {
    RDP root;
    root = (RDP)malloc(sizeof(struct RDP));
    root->label = x;
    root->leftmostChild = NULL;
    root->rightSibling = NULL;
    
    return root;
}
RDP make1Node(char x, RDP tree) {
    RDP root;
    root = makeNode(x);
    root->leftmostChild = tree;
    
    return root;
}
RDP make2Node(char x, RDP t1, RDP t2) {
    RDP root;
    root = make1Node(x,t1);
    t1->rightSibling = t2;
    
    return root;
}
RDP make3Node(char x, RDP t1, RDP t2, RDP t3) {
    RDP root;
    root = make1Node(x,t1);
    t1->rightSibling = t2;
    t2->rightSibling = t3;
    
    return root;
}
RDP make4Node(char x, RDP t1, RDP t2, RDP t3, RDP t4) {
    RDP root;
    root = make1Node(x,t1);
    t1->rightSibling = t2;
    t2->rightSibling = t3;
    t3->rightSibling = t4;
    
    return root;
}

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
