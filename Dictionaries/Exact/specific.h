//
//  exact.h
//  assignment11
//
//  Created by mac on 03/12/2021.
//

#ifndef exact_h
#define exact_h

#include "../dict.h"

typedef struct node{
    char* word;
    struct node* next;
}node;

struct dict{
    struct node** table;
    int sz;
};

void test(void);
int gethashnum(dict* x, const char* s);
node* allocate_node(const char* s);

#endif /* exact_h */
