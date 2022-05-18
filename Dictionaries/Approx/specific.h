//
//  approx.h
//  assignment11
//
//  Created by mac on 06/12/2021.
//

#ifndef approx_h
#define approx_h

#include "../dict.h"
#define KHASHES 11

struct dict{
    bool* hashtable;
    int sz;
};

void test(void);
unsigned long* _hashes(dict* x, const char* s);

#endif /* approx_h */
