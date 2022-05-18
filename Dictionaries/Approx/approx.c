#include "specific.h"

dict* dict_init(unsigned int maxwords)
{
    int sz = maxwords * 20;
    dict* p = (dict*)ncalloc(1, sizeof(dict));
    p->hashtable = (bool*)ncalloc(sz, sizeof(bool));
    p->sz = sz;
    for(int i=0; i<sz; i++){
        p->hashtable[i] = 0;
    }
    test();
    return p;
}

bool dict_add(dict* x,  const char* s)
{
    if(!x){
        return false;
    }
    int len = (int)strlen(s);
    if(len == 0){
        return false;
    }
    unsigned long* hashes;
    hashes = _hashes(x, s);
    unsigned long num;
    for(int i=0; i<KHASHES; i++){
        num = hashes[i];
        x->hashtable[num] = 1;
    }
    free(hashes);
    return true;
}

unsigned long _hash(dict* x, const char* s)
{
    unsigned long len = strlen(s);
    unsigned long total = 0;
    for(unsigned long i=0; i<len; i++){
        total = total + (x->sz ^ (s[i]*s[i]));
    }

    unsigned long h = total%(x->sz);
    return h;
}

bool dict_spelling(dict* x, const char* s)
{
    if(!x){
        return false;
    }
    unsigned long* hashes;
    hashes = _hashes(x, s);
    unsigned long num;
    for(int i=0; i<KHASHES; i++){
        num = hashes[i];
        if(x->hashtable[num] != 1){
            free(hashes);
            return false;
        }
    }
    
    free(hashes);
    return true;
}

void dict_free(dict* x)
{
    free(x->hashtable);
    free(x);
}

unsigned long* _hashes(dict* x, const char* s)
{
    unsigned long* hashes = ncalloc(KHASHES, sizeof(unsigned long));
    unsigned long bh = _hash(x, s);
    int ln = (int)strlen(s);
    srand((unsigned int)bh*(ln*s[0] + s[ln-1]));
    unsigned long h2 = bh;
    unsigned long h3;
    for (int i=0; i<KHASHES; i++) {
        h2 = 33 * h2 ^ rand();
        h3 = h2 % x->sz;
        hashes[i] = h3;
    }
    return hashes;
}

void test(void)
{
    dict* d = (dict*)ncalloc(1, sizeof(dict));
    d->hashtable = (bool*)ncalloc(40, sizeof(bool));
    d->sz = 40;
    unsigned long* hashes;
    
    //test "hello"
    assert(_hash(d, "hello") == 26);
    hashes = _hashes(d, "hello");
    for(int i=0; i<KHASHES; i++){
        assert(hashes[i] < (unsigned long)d->sz);
    }
    assert(!dict_spelling(d, "hello"));
    assert(dict_add(d, "hello"));
    assert(dict_spelling(d, "hello"));

    //test "morning"
    assert(_hash(d, "morning") == 24);
    hashes = _hashes(d, "morning");
    for(int i=0; i<KHASHES; i++){
        assert(hashes[i] < (unsigned long)d->sz);
    }
    assert(!dict_spelling(d, "morning"));
    assert(dict_add(d, "morning"));
    assert(dict_spelling(d, "morning"));

    //test "bathroom"
    hashes = _hashes(d, "bathroom");
    for(int i=0; i<KHASHES; i++){
        assert(hashes[i] < (unsigned long)d->sz);
    }
    assert(_hash(d, "bathroom") == 36);
    assert(!dict_spelling(d, "bathroom"));
    assert(dict_add(d, "bathroom"));
    assert(dict_spelling(d, "bathroom"));

    //test "eleven"
    assert(_hash(d, "eleven") == 19);
    hashes = _hashes(d, "eleven");
    for(int i=0; i<KHASHES; i++){
        assert(hashes[i] < (unsigned long)d->sz);
    }
    assert(!dict_spelling(d, "eleven"));
    assert(dict_add(d, "eleven"));
    assert(dict_spelling(d, "eleven"));

    //test "nightmare"
    assert(_hash(d, "nightmare") == 13);
    hashes = _hashes(d, "nightmare");
    for(int i=0; i<KHASHES; i++){
        assert(hashes[i] < (unsigned long)d->sz);
    }
    assert(!dict_spelling(d, "nightmare"));
    assert(dict_add(d, "nightmare"));

    //test "funny"
    assert(_hash(d, "funny") == 6);
    hashes = _hashes(d, "funny");
    for(int i=0; i<KHASHES; i++){
        assert(hashes[i] < (unsigned long)d->sz);
    }
    assert(!dict_spelling(d, "funny"));
    assert(dict_add(d, "funny"));

    //test "moonlight"
    assert(_hash(d, "moonlight") == 1);
    hashes = _hashes(d, "moonlight");
    for(int i=0; i<KHASHES; i++){
        assert(hashes[i] < (unsigned long)d->sz);
    }
    assert(!dict_spelling(d, "moonlight"));
    assert(dict_add(d, "moonlight"));

    //test "bristol"
    assert(_hash(d, "bristol") == 27);
    hashes = _hashes(d, "bristol");
    for(int i=0; i<KHASHES; i++){
        assert(hashes[i] < (unsigned long)d->sz);
    }
    assert(dict_add(d, "bristol"));
    assert(dict_spelling(d, "moonlight"));

    dict_free(d);
}
