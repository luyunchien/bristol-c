//
//  exact.c
//  assignment11
//
//  Created by mac on 03/12/2021.
//

#include "specific.h"

dict* dict_init(unsigned int maxwords)
{
    dict* all = (dict*)ncalloc(1, sizeof(dict));
    
    all->table = (node**)ncalloc(maxwords*2, sizeof(node*));
    all->sz = maxwords*2;
    
    test();
    return all;
}

int gethashnum(dict* x, const char* s)
{
    int total = 0;
    int len = (int)strlen(s);
    for(int i=0; i<len; i++){
        total = total + (x->sz ^ (s[i]*s[i]));
    }
    int hnum = total%(x->sz);
    return hnum;
}

bool dict_add(dict* x, const char* s)
{
    if(!x){
        return false;
    }
    int len = (int)strlen(s);
    if(len == 0){
        return false;
    }
    int hnum = gethashnum(x, s);
    node* current;
    
    if(!dict_spelling(x, s)){
        
        if (!x->table[hnum]){
            current = allocate_node(s);
            current->next = x->table[hnum];
            x->table[hnum] = current;
            
        }else{
            x->table[hnum] = allocate_node(s);
        }
    }
    return true;
}

node* allocate_node(const char* s)
{
    node* p;
    int len = (int)strlen(s);
    p = (node*)ncalloc(1, sizeof(node));
    char* tmp = (char*)ncalloc(len+1, sizeof(char));
    strcpy(tmp, s);
    p->word = tmp;
    p->next = NULL;
    
    return p;
    
}

bool dict_spelling(dict* x, const char* s)
{
    if(!x){
        return false;
    }
    int hnum = gethashnum(x, s);
    node* p = x->table[hnum];
    
    while(p){
        if(strcmp(p->word, s) == 0){
            return true;
        }
        p = p->next;
    }
    
    return false;
}

void dict_free(dict* x)
{
    node* p;
    for(int i=0; i<(x->sz); i++){
        p = x->table[i];
        while(p){
            free(p->word);
            node* q = p->next;
            free(p);
            p = q;
        }
    }
    free(x->table);
    free(x);
}

void test(void)
{
    dict* d = (dict*)ncalloc(1, sizeof(dict));
    d->table = (node**)ncalloc(40, sizeof(node*));
    d->sz = 40;
    
    //test "hello"
    assert(gethashnum(d, "hello") == 26);
    assert(!dict_spelling(d, "hello"));
    assert(dict_add(d, "hello"));
    assert(dict_spelling(d, "hello"));

    //test "morning"
    assert(gethashnum(d, "morning") == 24);
    assert(!dict_spelling(d, "morning"));
    assert(dict_add(d, "morning"));
    assert(dict_spelling(d, "morning"));

    //test "bathroom"
    assert(gethashnum(d, "bathroom") == 36);
    assert(!dict_spelling(d, "bathroom"));
    assert(dict_add(d, "bathroom"));
    assert(dict_spelling(d, "bathroom"));

    //test "eleven"
    assert(gethashnum(d, "eleven") == 19);
    assert(!dict_spelling(d, "eleven"));
    assert(dict_add(d, "eleven"));
    assert(dict_spelling(d, "eleven"));

    //test "nightmare"
    assert(gethashnum(d, "nightmare") == 13);
    assert(!dict_spelling(d, "nightmare"));
    assert(dict_add(d, "nightmare"));

    //test "funny"
    assert(gethashnum(d, "funny") == 6);
    assert(!dict_spelling(d, "funny"));
    assert(dict_add(d, "funny"));

    //test "moonlight"
    assert(gethashnum(d, "moonlight") == 1);
    assert(!dict_spelling(d, "moonlight"));
    assert(dict_add(d, "moonlight"));

    //test "bristol"
    assert(gethashnum(d, "bristol") == 27);
    assert(!dict_spelling(d, "bristol"));
    assert(dict_add(d, "bristol"));
    
    dict_free(d);
}
