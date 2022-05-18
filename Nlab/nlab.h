//
//  nlab1.h
//  nlab
//
//  Created by mac on 14/01/2022.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 20
#define FACTOR 2
#define COLS 10
#define ROWS 10

typedef struct prog{
    char wds[MAXNUMTOKENS][MAXTOKENSIZE];
    int cw;
}prog;

typedef struct stacktype{
    int** arr;
    char name;
    int cols;
    int rows;
}stacktype;

typedef struct stack{
    stacktype* list;
    int sz;
    int capacity;
}stack;

void* ncalloc(int n, size_t size);
void Prog(prog* p, stack* line_stack, stack* all_stack);
void Instrclist(prog* p, stack* line_stack, stack* all_stack);
void Instrc(prog* p, stack* line_stack, stack* all_stack);
void Print(prog* p, stack* line_stack, stack* all_stack);
void Varname(prog* p);
void String(prog* p);
void Set(prog* p, stack* line_stack, stack* all_stack);
void Polishlist(prog* p, stack* line_stack, stack* all_stack);
void Pushdown(prog* p, stack* line_stack, stack* all_stack);
void Unaryop(prog* p, stack* line_stack);
bool isnum(prog* p);
void Binaryop(prog* p, stack* line_stack);
void Create(prog* p, stack* all_stack);
void Loop(prog* p, stack* line_stack, stack* all_stack);
void file2wds(prog* p, const char* filename);
void Polish(prog* p, stack* line_stack, stack* all_stack);
bool isin_stack(prog* p, stack* s, int* position);
void stack_push(stack* s, stacktype d);
bool stack_pop(stack* s, stacktype* d);
bool exe_Pop(stack* s, stacktype* a, stacktype* b);
bool issamesize(stacktype* a, stacktype* b);
void cpyarr(stack* s, int position, stacktype* d);
bool isBinary(stacktype* d);
int cnt_nbr(stacktype* c, int j, int i);
void freearray(stacktype* d, int rows);
void initial_arr(stacktype* d, int rows, int cols);
