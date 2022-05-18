#include "crossword.h"
void string_add_num(char* str, int num);
void print_crossword(const crossword* c);
bool isEmpty(const crossword* c, int x, int y);
bool isvalid(int sz, char* ip, crossword* cw);
int checked_num = 0;
unsigned long ip_length = 0;

// Might be useful to be able to print them
// to help with debugging
void print_crossword(const crossword* c)
{
    for(int y=0; y<c->sz; y++){
        for(int x=0; x<c->sz; x++){
            printf("%c", c->arr[y][x]);
        }
        printf("\n");
    }
}

bool str2crossword(int sz, char* ip, crossword* cw)
{
    bool valid = isvalid(sz, ip, cw);
    if(valid == 1){
        cw->sz = sz;
        char* p = ip;
        for (int j = 0; j < sz; j++){
            for (int i = 0; i < sz; i++){
                cw -> arr[j][i] = *p++;
            }
        }
    }
    return valid;
}

void getcluestring(const crossword* c, char* ans)
{
    int size = c ->sz;
    int j = 0, i = 0, num = 0;
    memset(ans, 0, strlen(ans));
    char A[BIGSTR] = "A";
    char D[BIGSTR] = "D";
    for(j = 0; j < size; j++){
        for(i = 0; i < size; i++){
            bool isAhead = false;
            bool isDhead = false;
            if(isEmpty(c, i, j)){
                //know if current square is the first char of across word
                if((!isEmpty(c, i-1, j)) && (isEmpty(c, i+1, j)))
                    isAhead = true;
                //know if current square is the first char of down word
                if((!isEmpty(c, i, j-1)) && (isEmpty(c, i, j+1)))
                    isDhead = true;
            }
            //number current grid
            if(isAhead == 1 && isDhead == 1){
                num++;
                string_add_num(A, num);
                string_add_num(D, num);
            }else if(isAhead == 1 && isDhead == 0){
                num++;
                string_add_num(A, num);
            }else if(isAhead == 0 && isDhead == 1){
                num++;
                string_add_num(D, num);
            }
        }
    }
    checked_num = num;
    //combine A and D string
    strcat(ans, A);
    strcat(ans, "|");
    strcat(ans, D);
}

int getchecked(crossword c)
{
    int ept = 0;
    int checked = 0;
    int percentage = 0;
    
    for(int j = 0; j < c.sz; j++){
        for(int i = 0; i < c.sz; i++){
            if(isEmpty(&c, i, j)){
                ept++;
                bool is_in_A = false;
                bool is_in_D = false;
                if(isEmpty(&c, i-1, j) || isEmpty(&c, i+1, j))
                    is_in_A = true;
                if(isEmpty(&c, i, j+1) || isEmpty(&c, i, j-1))
                    is_in_D = true;
                if(is_in_A && is_in_D)
                    checked++;
            }
        }
    }
    percentage = ((1.0*checked)/(1.0*ept))*100+0.5;
    return percentage;
}

bool isvalid(int sz, char* ip, crossword* cw)
{
    if (ip == NULL){
        return 0;
    }else if (cw == NULL){
        return 0;
    }else if(sz <= 0){
        return 0;
    }else if (sz > 100){
        return 0;
    }else if ((int)strlen(ip) < sz*sz){
        return 0;
    }else if ((int)strlen(ip) > sz*sz){
        return 0;
    }else{
        return 1;
    }
}

void string_add_num(char* str, int num)
{
    char* end = str + strlen(str);
    *end = '-';
    end++;
    char NUM[3];
    sprintf(NUM,"%d",num);
    strcat(str, NUM);
    return;
}

bool isEmpty(const crossword* c, int x, int y)
{
    if(x<0 || x>= c->sz || y<0 || y>= c->sz)
        return 0;
    if(c->arr[y][x] == 'X' || c->arr[y][x] == '*')
        return 0;
    return 1;
}

void test(void)
{
    crossword c;
    assert(isvalid(5, "....X.XX.X.X......X.XX...", &c) == 1);
    assert(isvalid(2, "", NULL) == 0);
    assert(isvalid(-5, ".......XX.......XX.....X", &c) == 0);
    assert(isvalid(300, "..X....X.XX.X.X.XX.X..XX......X..XX.XX.....XX.XX.", &c) == 0);
    str2crossword(5, "....X.XX.X.X......X.XX...", &c);
    assert(isEmpty(&c, -1, 2) == 0);
    assert(isEmpty(&c, 3, -2) == 0);
    assert(isEmpty(&c, 0, 0) == 1);
    assert(isEmpty(&c, 5, 1) == 0);
    assert(isEmpty(&c, 10, 1) == 0);
    char str[100] = "A";
    int num = 50;
    string_add_num(str, num);
    assert(strcmp(str, "A-50") == 0);
    string_add_num(str, 48);
    assert(strcmp(str, "A-50-48") == 0);
    string_add_num(str, 35);
    assert(strcmp(str, "A-50-48-35") == 0);
}


