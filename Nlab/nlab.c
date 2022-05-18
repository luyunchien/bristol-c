#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define CMDWITHFILE 2
#define MAXNUMTOKENS 1000
#define MAXTOKENSIZE 20
#define FACTOR 2
#define MAXCOLS 100
#define MAXROWS 100

typedef struct prog{
    char wds[MAXNUMTOKENS][MAXTOKENSIZE];
    int cw;
}prog;

typedef struct stacktype{
    int arr[MAXROWS][MAXCOLS];
    char name;
    int cols;
    int rows;
}stacktype;

typedef struct stack{
    stacktype* list;
    int sz;
    int capacity;
}stack;

void test(void);
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
void B_and(stacktype* a, stacktype* b, stacktype* c);
void B_or(stacktype* a, stacktype* b, stacktype* c);
void B_greater(stacktype* a, stacktype* b, stacktype* c);
void B_less(stacktype* a, stacktype* b, stacktype* c);
void B_add(stacktype* a, stacktype* b, stacktype* c);
void B_times(stacktype* a, stacktype* b, stacktype* c);
void B_equals(stacktype* a, stacktype* b, stacktype* c);

int main(int argc, const char * argv[]) {
    test();
    prog* program = ncalloc(1, sizeof(prog));
    stack* all = ncalloc(1, sizeof(stack));
    stack* line_stack = ncalloc(1, sizeof(stack));
    stack* all_stack = ncalloc(1, sizeof(stack));
#ifdef INTERP
    line_stack->list = ncalloc(MAXTOKENSIZE, sizeof(stacktype));
    line_stack->capacity = MAXTOKENSIZE;
    all_stack->list = ncalloc(MAXTOKENSIZE, sizeof(stacktype));
    all_stack->capacity = MAXTOKENSIZE;
#endif
    if(argc == CMDWITHFILE){
        file2wds(program, argv[1]);
        Prog(program, line_stack, all_stack);
    }else{
        perror("Please command with input file's name.\n");
    }
#ifdef INTERP
        free(line_stack->list);
        free(all_stack->list);
#endif
    free(line_stack);
    free(all_stack);
    free(all);
    free(program);
    return 0;
}
    
void* ncalloc(int n, size_t size)
{
   void* v = calloc(n, size);
   if(v==NULL){
       perror("Cannot calloc() space\n");
       exit(EXIT_FAILURE);
   }
   return v;
}

void file2wds(prog* p, const char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file){
        perror("The file does not exist\n");
        exit(EXIT_FAILURE);
    }
    int i=0;
    while (fscanf(file, "%s", p->wds[i]) == 1 && i<MAXNUMTOKENS){
        i++;
    }
    assert(i<MAXNUMTOKENS);
    fclose(file);
}

void Prog(prog* p, stack* line_stack, stack* all_stack)
{
    if(strcmp(p->wds[p->cw], "BEGIN") != 0){
        perror("No BEGIN statement?\n");
        exit(EXIT_FAILURE);
    }
    p->cw = p->cw + 1;
    if(strcmp(p->wds[p->cw], "{") != 0){
        perror("No { statement?\n");
        exit(EXIT_FAILURE);
    }
    p->cw = p->cw + 1;
    Instrclist(p, line_stack, all_stack);
}

void Instrclist(prog* p, stack* line_stack, stack* all_stack)
{
    if(strcmp(p->wds[p->cw], "}") == 0){
        return;
    }else{
        Instrc(p, line_stack, all_stack);
        Instrclist(p, line_stack, all_stack);
    }
}

void Instrc(prog* p, stack* line_stack, stack* all_stack)
{
    if(strcmp(p->wds[p->cw], "PRINT") == 0){
        p->cw = p->cw + 1;
        Print(p, line_stack, all_stack);
    }else if(strcmp(p->wds[p->cw], "SET") == 0){
        p->cw = p->cw + 1;
        Set(p, line_stack, all_stack);
    }else if(strcmp(p->wds[p->cw], "ONES") == 0 || strcmp(p->wds[p->cw], "READ") == 0){
        Create(p, all_stack);
    }else if(strcmp(p->wds[p->cw], "LOOP") == 0){
        p->cw = p->cw + 1;
        Loop(p, line_stack, all_stack);
    }else{
        perror("syntax error (instruc)\n");
        exit(EXIT_FAILURE);
    }
}

void Print(prog* p, stack* line_stack, stack* all_stack)
{
    if(!line_stack || !all_stack){
        perror("invalid stack\n");
        exit(EXIT_FAILURE);
    }
    if((p->wds[p->cw][0]) == '$'){
        Varname(p);
#ifdef INTERP
        int position;
        char name = p->wds[p->cw][1];
        if(isin_stack(p, all_stack, &position)){
            for(int y=0; y<all_stack->list[position].rows; y++){
                for(int x=0; x<all_stack->list[position].cols; x++){
                    printf("%d ",all_stack->list[position].arr[y][x]);
                }
                printf("\n");
            }
        }else if(isin_stack(p, line_stack, &position)){
            for(int y=0; y<line_stack->list[position].rows; y++){
                for(int x=0; x<line_stack->list[position].cols; x++){
                    printf("%d ",line_stack->list[position].arr[y][x]);
                }
                printf("\n");
            }
        }else{
            printf("there is no $%c in the list\n",name);
            exit(EXIT_FAILURE);
        }
#endif
        p->cw = p->cw + 1;
    }else if((p->wds[p->cw][0]) == '"'){
        String(p);
#ifdef INTERP
        int i=1;
        while((p->wds[p->cw][i]) && (p->wds[p->cw][i]) != '"'){
            printf("%c",p->wds[p->cw][i]);
            i++;
        }
        printf("\n");
#endif
        p->cw = p->cw + 1;
    }else{
        perror("PRINT must be followed by VARNAME or STRING.\n");
        exit(EXIT_FAILURE);
    }
}

void Varname(prog* p)
{
    if(p->wds[p->cw][2]){
        perror("There should only be one A-Z character as variable name.\n");
        exit(EXIT_FAILURE);
    }else if((int)(p->wds[p->cw][1]) >= 'A' && (int)(p->wds[p->cw][1]) <= 'Z'){
        return;
    }else{
        perror("$ must be followed by A-Z.\n");
        exit(EXIT_FAILURE);
    }
}

void String(prog* p)
{
    unsigned long len = strlen(p->wds[p->cw]);
    if((p->wds[p->cw][0] != '"') || (p->wds[p->cw][len-1] != '"')){
        perror("It is not a string\n");
        exit(EXIT_FAILURE);
    }
    for(int i=1; i<(int)len; i++){
        if(p->wds[p->cw][i] == ' '){
            perror("strings can't have space in them.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void Set(prog* p, stack* line_stack, stack* all_stack)
{
#ifdef INTERP
    stacktype* d = (stacktype*)ncalloc(1, sizeof(stacktype));
    char name = '0';
    int position = 0;
#endif
    if((p->wds[p->cw][0]) == '$'){
        Varname(p);
#ifdef INTERP
        char curname = p->wds[p->cw][1];
        name = curname;
        if(isin_stack(p, all_stack, &position)){
            ;
        }else{
            position = all_stack->sz;
            d->name = curname;
            all_stack->sz = all_stack->sz + 1;
        }
#endif
        p->cw = p->cw + 1;
    }else{
        perror("SET must be followed by VARNAME.\n");
        exit(EXIT_FAILURE);
    }

    if(strcmp(p->wds[p->cw], ":=") == 0){
        p->cw = p->cw + 1;
        Polishlist(p, line_stack, all_stack);
#ifdef INTERP
        stack_pop(line_stack, d);
        d->name = name;
        if(d->cols == 0 && d->rows == 0){
            perror("The variable does not exsit.\n");
            exit(EXIT_FAILURE);
        }
        all_stack->list[position] = *d;
        free(d);
#endif
    }else{
        perror("VARNAME must be followed by :=\n");
        exit(EXIT_FAILURE);
    }
}

void Polishlist(prog* p, stack* line_stack, stack* all_stack)
{
    if(strcmp(p->wds[p->cw],";") == 0){
        p->cw = p->cw + 1;
        return;
    }else if(p->wds[p->cw][0] == '$' || isnum(p)){
        Pushdown(p, line_stack, all_stack);
    }else if(p->wds[p->cw][0] == 'U'){
        Unaryop(p, line_stack);
    }else if(p->wds[p->cw][0] == 'B'){
        Binaryop(p, line_stack);
    }else{
        perror("syntax error (polishlist)\n");
        exit(EXIT_FAILURE);
    }
    Polishlist(p, line_stack, all_stack);
}

void Polish(prog* p, stack* line_stack, stack* all_stack)
{
    if(((p->wds[p->cw][0]) == '$') || (isnum(p) == true)){
        Pushdown(p, line_stack, all_stack);
    }else if((p->wds[p->cw][0]) == 'U'){
        Unaryop(p, line_stack);
    }else if((p->wds[p->cw][0]) == 'B'){
        Binaryop(p, line_stack);
    }else{
        perror("POLISH should be followed by PUSHDOWN, UNARYOP or BINARYOP.\n");
        exit(EXIT_FAILURE);
    }
}

bool isnum(prog* p)
{
    if(p->wds[p->cw][0] == '-'){
        return false;
    }
    int i = 0;
    while(p->wds[p->cw][i] != '\0'){
        if((p->wds[p->cw][i]) > 57 || (p->wds[p->cw][i]) < 48){
            return false;
        }
        i++;
    }
    return true;
}
       
void Pushdown(prog* p, stack* line_stack, stack* all_stack)
{
    if(!line_stack || !all_stack){
        perror("invalid stack\n");
        exit(EXIT_FAILURE);
    }
    if((p->wds[p->cw][0]) == '$'){
        Varname(p);
#ifdef INTERP
        stacktype* d;
        d = (stacktype*)ncalloc(1, sizeof(stacktype));
        int position;
        char curname = p->wds[p->cw][1];
        d->name = curname;
        if(isin_stack(p, all_stack, &position)){
            cpyarr(all_stack, position, d);
            stack_push(line_stack, *d);
            free(d);
        }else if(isin_stack(p, line_stack, &position)){
            cpyarr(line_stack, position, d);
            stack_push(line_stack, *d);
            free(d);
        }else{
            stack_push(line_stack, *d);
            free(d);
        }
#endif
        p->cw = p->cw + 1;
        return;
    }else if(isnum(p)){
#ifdef INTERP
        stacktype* d;
        d = (stacktype*)ncalloc(1, sizeof(stacktype));
        int num = atoi(p->wds[p->cw]);
        d->arr[0][0] = num;
        d->rows = 1;
        d->cols = 1;
        stack_push(line_stack, *d);
        free(d);
#endif
        p->cw = p->cw + 1;
    }
}


void Unaryop(prog* p, stack* line_stack)
{
    if(!line_stack){
        perror("invalid stack\n");
        exit(EXIT_FAILURE);
    }
#ifdef INTERP
    stacktype* c = (stacktype*)ncalloc(1, sizeof(stacktype));
    stack_pop(line_stack, c);
    if(c->cols == 0 && c->rows == 0){
        perror("The variable does not exist.\n");
        exit(EXIT_FAILURE);
    }
#endif
    if(strcmp(p->wds[p->cw], "U-NOT") == 0){
#ifdef INTERP
        stacktype* x = (stacktype*)ncalloc(1, sizeof(stacktype));
        x->rows = c->rows;
        x->cols = c->cols;
        if(isBinary(c)){
            for(int j=0; j<c->rows; j++){
                for(int i=0; i<c->cols; i++){
                    if(c->arr[j][i] == 0){
                        x->arr[j][i] = 1;
                    }else{
                        x->arr[j][i] = 0;
                    }
                }
            }
        }
        stack_push(line_stack, *x);
        free(x);
        free(c);
#endif
        p->cw = p->cw + 1;
        return;
    }else if(strcmp(p->wds[p->cw], "U-EIGHTCOUNT") == 0){
#ifdef INTERP
        int cnt = 0;
        stacktype* x = (stacktype*)ncalloc(1, sizeof(stacktype));
        x->rows = c->rows;
        x->cols = c->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                cnt = cnt_nbr(c, j, i);
                x->arr[j][i] = cnt;
            }
        }
        stack_push(line_stack, *x);
        free(x);
        free(c);
#endif
        p->cw = p->cw + 1;
        return;
    }else{
        perror("UNARYOP should be followed by U-NOT or U-EIGHTCOUNT.\n");
        exit(EXIT_FAILURE);
    }
}

void Binaryop(prog* p, stack* line_stack)
{
    if(!line_stack){
        perror("invalid stack\n");
        exit(EXIT_FAILURE);
    }
#ifdef INTERP
    stacktype* a = (stacktype*)ncalloc(1, sizeof(stacktype));
    stacktype* b = (stacktype*)ncalloc(1, sizeof(stacktype));
    stacktype* c = (stacktype*)ncalloc(1, sizeof(stacktype));
#endif
    
    if(strcmp(p->wds[p->cw], "B-AND") == 0){
#ifdef INTERP
        if(exe_Pop(line_stack, a, b)){
            if(issamesize(a, b)){
                if(isBinary(a) && isBinary(b)){
                    B_and(a, b, c);
                }
            }
        }else{
            perror("cannot pop 2 val\n");
            exit(EXIT_FAILURE);
        }
        stack_push(line_stack, *c);
        free(a);
        free(b);
        free(c);
#endif
        p->cw = p->cw + 1;
        return;
    }else if(strcmp(p->wds[p->cw], "B-OR") == 0){
#ifdef INTERP
        if(exe_Pop(line_stack, a, b)){
            if(issamesize(a, b)){
                if(isBinary(a) && isBinary(b)){
                    B_or(a,b,c);
                }
            }
        }else{
            perror("cannot pop 2 val\n");
            exit(EXIT_FAILURE);
        }
        stack_push(line_stack, *c);
        free(a);
        free(b);
        free(c);
#endif
        p->cw = p->cw + 1;
        return;
    }else if(strcmp(p->wds[p->cw], "B-GREATER") == 0){
#ifdef INTERP
        if(exe_Pop(line_stack, a, b)){
            if(issamesize(a, b)){
                B_greater(a,b,c);
            }
        }else{
            perror("cannot pop 2 val\n");
            exit(EXIT_FAILURE);
        }
        stack_push(line_stack, *c);
        free(a);
        free(b);
        free(c);
#endif
        p->cw = p->cw + 1;
        return;
    }else if(strcmp(p->wds[p->cw], "B-LESS") == 0){
#ifdef INTERP
        if(exe_Pop(line_stack, a, b)){
            if(issamesize(a, b)){
                B_less(a, b, c);
            }
        }else{
            perror("cannot pop 2 val\n");
            exit(EXIT_FAILURE);
        }
        stack_push(line_stack, *c);
        free(a);
        free(b);
        free(c);
#endif
        p->cw = p->cw + 1;
        return;
    }else if(strcmp(p->wds[p->cw], "B-ADD") == 0){
#ifdef INTERP
        if(exe_Pop(line_stack, a, b)){
            if(issamesize(a, b)){
                B_add(a, b, c);
            }
        }else{
            perror("cannot pop 2 val\n");
            exit(EXIT_FAILURE);
        }
        stack_push(line_stack, *c);
        free(a);
        free(b);
        free(c);
#endif
        p->cw = p->cw + 1;
        return;
    }else if(strcmp(p->wds[p->cw], "B-TIMES") == 0){
#ifdef INTERP
        if(exe_Pop(line_stack, a, b)){
            if(issamesize(a, b)){
                B_times(a,b,c);
            }
        }else{
            perror("cannot pop 2 val\n");
            exit(EXIT_FAILURE);
        }
        stack_push(line_stack, *c);
        free(a);
        free(b);
        free(c);
#endif
        p->cw = p->cw + 1;
        return;
    }else if(strcmp(p->wds[p->cw], "B-EQUALS") == 0){
#ifdef INTERP
        if(exe_Pop(line_stack, a, b)){
            if(issamesize(a, b)){
                B_equals(a, b, c);
            }
        }else{
            perror("cannot pop 2 val\n");
            exit(EXIT_FAILURE);
        }
        stack_push(line_stack, *c);
        free(a);
        free(b);
        free(c);
#endif
        p->cw = p->cw + 1;
        return;
    }else{
        perror("BINARYOP should be followed by operations in the grammar.\n");
        exit(EXIT_FAILURE);
    }
}

void B_and(stacktype* a, stacktype* b, stacktype* c)
{
    if(a->rows == 1 && a->cols == 1){
        c->rows = b->rows;
        c->cols = b->cols;
        for(int j=0; j<b->rows; j++){
            for(int i=0; i<b->cols; i++){
                c->arr[j][i] =  a->arr[0][0] && b->arr[j][i];
            }
        }
    }else if(b->rows == 1 && b->cols == 1){
        c->rows = a->rows;
        c->cols = a->cols;
        for(int j=0; j<a->rows; j++){
            for(int i=0; i<a->cols; i++){
                c->arr[j][i] =  a->arr[j][i] && b->arr[0][0];
            }
        }
    }else{
        c->rows = b->rows;
        c->cols = b->cols;
        for(int j=0; j<a->rows; j++){
            for(int i=0; i<a->cols; i++){
                c->arr[j][i] =  a->arr[j][i] && b->arr[j][i];
            }
        }
    }
}

void B_or(stacktype* a, stacktype* b, stacktype* c)
{
    if(a->rows == 1 && a->cols ==1){
        c->rows = b->rows;
        c->cols = b->cols;
        for(int j=0; j<b->rows; j++){
            for(int i=0; i<b->cols; i++){
                c->arr[j][i] =  a->arr[0][0] || b->arr[j][i];
            }
        }
    }else if(b->rows == 1 && b->cols == 1){
        c->rows = a->rows;
        c->cols = a->cols;
        for(int j=0; j<a->rows; j++){
            for(int i=0; i<a->cols; i++){
                c->arr[j][i] =  a->arr[j][i] || b->arr[0][0];
            }
        }
    }else{
        c->rows = b->rows;
        c->cols = b->cols;
        for(int j=0; j<b->rows; j++){
            for(int i=0; i<b->cols; i++){
                c->arr[j][i] =  a->arr[j][i] || b->arr[j][i];
            }
        }
    }
}

void B_greater(stacktype* a, stacktype* b, stacktype* c)
{
    if(a->rows == 1 && a->cols == 1){
        c->rows = b->rows;
        c->cols = b->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                if(a->arr[0][0] < b->arr[j][i]){
                    c->arr[j][i] = 1;
                }else{
                    c->arr[j][i] = 0;
                }
            }
        }
    }else if(b->rows == 1 && b->cols == 1){
        c->rows = a->rows;
        c->cols = a->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                if(a->arr[j][i] < b->arr[0][0]){
                    c->arr[j][i] = 1;
                }else{
                    c->arr[j][i] = 0;
                }
            }
        }
    }else{
        c->rows = b->rows;
        c->cols = b->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                if(a->arr[j][i] < b->arr[j][i]){
                    c->arr[j][i] = 1;
                }else{
                    c->arr[j][i] = 0;
                }
            }
        }
    }
}

void B_less(stacktype* a, stacktype* b, stacktype* c)
{
    if(a->rows == 1 && a->cols == 1){
        c->rows = b->rows;
        c->cols = b->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                if(a->arr[0][0] > b->arr[j][i]){
                    c->arr[j][i] = 1;
                }else{
                    c->arr[j][i] = 0;
                }
            }
        }
    }else if(b->rows == 1 && b->cols == 1){
        c->rows = a->rows;
        c->cols = a->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                if(a->arr[j][i] > b->arr[0][0]){
                    c->arr[j][i] = 1;
                }else{
                    c->arr[j][i] = 0;
                }
            }
        }
    }else{
        c->rows = a->rows;
        c->cols = a->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                if(a->arr[j][i] > b->arr[j][i]){
                    c->arr[j][i] = 1;
                }else{
                    c->arr[j][i] = 0;
                }
            }
        }
    }
}

void B_add(stacktype* a, stacktype* b, stacktype* c)
{
    if(a->rows == 1 && a->cols == 1){
        c->rows = b->rows;
        c->cols = b->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                c->arr[j][i] = a->arr[0][0] + b->arr[j][i];
            }
        }
    }else if(b->rows == 1 && b->cols == 1){
        c->rows = a->rows;
        c->cols = a->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                c->arr[j][i] = a->arr[j][i] + b->arr[0][0];
            }
        }
    }else{
        c->rows = a->rows;
        c->cols = a->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                c->arr[j][i] = a->arr[j][i] + b->arr[j][i];
            }
        }
    }
}

void B_times(stacktype* a, stacktype* b, stacktype* c)
{
    if(a->rows == 1 && a->cols == 1){
        c->rows = b->rows;
        c->cols = b->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                c->arr[j][i] = a->arr[0][0] * b->arr[j][i];
            }
        }
    }else if(b->rows == 1 && b->cols == 1){
        c->rows = a->rows;
        c->cols = a->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                c->arr[j][i] = a->arr[j][i] * b->arr[0][0];
            }
        }
    }else{
        c->rows = a->rows;
        c->cols = a->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                c->arr[j][i] = a->arr[j][i] * b->arr[j][i];
            }
        }
    }
}

void B_equals(stacktype* a, stacktype* b, stacktype* c)
{
    if(a->rows == 1 && a->cols == 1){
        c->rows = b->rows;
        c->cols = b->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                if(a->arr[0][0] == b->arr[j][i]){
                    c->arr[j][i] = 1;
                }else{
                    c->arr[j][i] = 0;
                }
            }
        }
    }else if(b->rows == 1 && b->cols == 1){
        c->rows = a->rows;
        c->cols = a->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                if(a->arr[j][i] == b->arr[0][0]){
                    c->arr[j][i] = 1;
                }else{
                    c->arr[j][i] = 0;
                }
            }
        }
    }else{
        c->rows = a->rows;
        c->cols = a->cols;
        for(int j=0; j<c->rows; j++){
            for(int i=0; i<c->cols; i++){
                if(a->arr[j][i] == b->arr[j][i]){
                    c->arr[j][i] = 1;
                }else{
                    c->arr[j][i] = 0;
                }
            }
        }
    }
}



void Create(prog* p, stack* all_stack)
{
    if(!all_stack){
        perror("invalid stack\n");
        exit(EXIT_FAILURE);
    }
    if(strcmp(p->wds[p->cw], "ONES") == 0){
#ifdef INTERP
        stacktype* d = (stacktype*)ncalloc(1, sizeof(stacktype));
#endif
        p->cw = p->cw + 1;
        if(isnum(p)){
#ifdef INTERP
            int rows = 0;
            rows = atoi(p->wds[p->cw]);
            d->rows = rows;
#endif
            p->cw = p->cw + 1;
            if(isnum(p)){
#ifdef INTERP
                int cols = 0;
                cols = atoi(p->wds[p->cw]);
                d->cols = cols;
#endif
                p->cw = p->cw + 1;
                if(p->wds[p->cw][0] == '$'){
                    Varname(p);
#ifdef INTERP
                    char name = 0;
                    name = p->wds[p->cw][1];
                    d->name = name;
                    int position;
                    bool inallstack = false;
                    if(isin_stack(p, all_stack, &position)){
                        inallstack = true;
                    }
#endif
                    p->cw = p->cw + 1;
#ifdef INTERP
                    for(int j=0; j<rows; j++){
                        for(int i=0; i<cols; i++){
                            d->arr[j][i] = 1;
                        }
                    }
                    if(inallstack == true){

                        all_stack->list[position] = *d;
                    }else{
                        stack_push(all_stack, *d);
                    }
                    free(d);
#endif
                }else{
                    perror("CREATE: COL should be followed by VARNAME.\n");
                    exit(EXIT_FAILURE);
                }
            }else{
                perror("CREATE: ROW should be followed by COLS.\n");
                exit(EXIT_FAILURE);
            }
        }else{
            perror("CREATE: ONES should be followed by ROWS.\n");
            exit(EXIT_FAILURE);
        }
    }else if(strcmp(p->wds[p->cw], "READ") == 0){
#ifdef INTERP
        stacktype* d = (stacktype*)ncalloc(1, sizeof(stacktype));
#endif
        p->cw = p->cw + 1;
        
        if(p->wds[p->cw][0] == '"'){
            String(p);
#ifdef INTERP
            char* filename = (char*)ncalloc(MAXTOKENSIZE, sizeof(char));
            int i=1;
            while(p->wds[p->cw][i] != '"'){
                filename[i-1] = p->wds[p->cw][i];
                i++;
            }
#endif
            p->cw = p->cw + 1;
            if(p->wds[p->cw][0] == '$'){
                Varname(p);
#ifdef INTERP
                char name = 0;
                name = p->wds[p->cw][1];
                int rows = 0, cols = 0;
                FILE* file = fopen(filename, "r");
                if(!file){
                    perror("cannot open the file\n");
                    exit(EXIT_FAILURE);
                }
                fscanf(file, "%d",&rows);
                fscanf(file, "%d",&cols);
                int i=0;
                int* buffer = (int*)ncalloc(cols*rows+1, sizeof(int));
                while (fscanf(file, "%d",&buffer[i]) == 1 && i<rows*cols) {
                    i++;
                }
                if(rows*cols != i){
                    perror("invalid file\n");
                    exit(EXIT_FAILURE);
                }
                d->rows = rows;
                d->cols = cols;
                d->name = name;
                int l = 0;
                for(int y=0; y<rows; y++){
                    for(int x=0; x<cols; x++){
                        d->arr[y][x] = buffer[l];
                        l++;
                    }
                }
                free(filename);
                fclose(file);
                stack_push(all_stack, *d);
                free(buffer);
                free(d);
#endif
                p->cw = p->cw + 1;
            }else{
                perror("CREATE: STRING should be followed by VARNAME.\n");
                exit(EXIT_FAILURE);
            }
        }else{
            perror("CREATE: READ should be followed by FILENAME.\n");
            exit(EXIT_FAILURE);
        }
    }else{
        perror("CREATE: CREATE should be followed by ONES or READ.\n");
        exit(EXIT_FAILURE);
    }
}

void Loop(prog* p, stack* line_stack, stack* all_stack)
{
    if(p->wds[p->cw][0] == '$'){
        Varname(p);
#ifdef INTERP
        int pos;
        char name = 0;
        if(isin_stack(p, line_stack, &pos)){
            name = line_stack->list[pos].name;
        }else{
            stacktype* cnt = (stacktype*)ncalloc(1, sizeof(stacktype));
            name = p->wds[p->cw][1];
            cnt->name = name;
            cnt->arr[0][0] = 1;
            cnt->rows = 1;
            cnt->cols = 1;
            stack_push(line_stack, *cnt);
            free(cnt);
        }
#endif
        p->cw = p->cw + 1;
        if(isnum(p)){
#ifdef INTERP
            int num = atoi(p->wds[p->cw]);
#endif
            p->cw = p->cw + 1;
            if(strcmp(p->wds[p->cw], "{") == 0){
                p->cw = p->cw + 1;
                if(strcmp(p->wds[p->cw], "}") == 0 || strcmp(p->wds[p->cw], "PRINT") == 0 || strcmp(p->wds[p->cw], "SET") == 0 || strcmp(p->wds[p->cw], "CREATE") == 0 || strcmp(p->wds[p->cw], "LOOP") == 0){
#ifdef INTERP
                    int current = p->cw;
                    int times = 1;
                    bool loopends = false;
                    for(times = 1; (times<=num) && (loopends == false); times++){
                        for(int k=0; k<all_stack->sz; k++){
                            if(all_stack->list[k].name == name){
                                all_stack->list[k].arr[0][0] = times;
                            }
                        }
                        for(int k=0; k<line_stack->sz; k++){
                            if(line_stack->list[k].name == name){
                                line_stack->list[k].arr[0][0] = times;
                            }
                        }
                        p->cw = current;
#endif
                        Instrclist(p, line_stack, all_stack);
#ifdef INTERP
                        for(int k=0; k<line_stack->sz; k++){
                            if(line_stack->list[k].name == name){
                                for(int s=0; s<all_stack->sz; s++){
                                    if(all_stack->list[s].name == name){
                                        line_stack->list[k].arr[0][0] = all_stack->list[s].arr[0][0];
                                        times = line_stack->list[k].arr[0][0];
                                    }
                                }
                            }
                        }
                        if(times>num){
                            loopends = true;
                        }
#endif

#ifdef INTERP
                    }
                    stacktype* m = (stacktype*)ncalloc(1, sizeof(stacktype));
                    
                    if(loopends){
                        for(int k=0; k<line_stack->sz; k++){
                            if(line_stack->list[k].name == name){
                                *m = line_stack->list[k];
                            }
                        }
                        line_stack->sz = line_stack->sz-1;

                    }
                    
                    free(m);
#endif
                    p->cw = p->cw + 1;
                    return;
                }else{
                    perror("LOOP: { should be followed by instrclist.\n");
                    exit(EXIT_FAILURE);
                }
            }else{
                perror("LOOP: INTEGER should be followed by {.\n");
                exit(EXIT_FAILURE);
            }
        }else{
            perror("LOOP: VARNAME should be followed by INTEGER.\n");
            exit(EXIT_FAILURE);
        }
    }else{
        perror("LOOP: LOOP should be followed by VARNAME.\n");
        exit(EXIT_FAILURE);
    }
}

bool isin_stack(prog* p, stack* s, int* position)
{
    char curname = p->wds[p->cw][1];
    int i=0;
    for(i=0; i<s->sz; i++){
        if(s->list[i].name == curname){
            *position = i;
            return true;
        }
    }
    return false;
}


void stack_push(stack* s, stacktype d)
{
   if(s==NULL){
       return;
   }
   if(s->sz >= s->capacity){
       int newcap = s->capacity * FACTOR;
       s->list = realloc(s->list, sizeof(stacktype)*newcap);
       s->capacity = s->capacity * FACTOR;
   }
    s->list[s->sz] = d;
    s->sz = s->sz + 1;
}

bool stack_pop(stack* s, stacktype* d)
{
   if((s == NULL) || (s->sz < 1)){
      return false;
   }
   s->sz = s->sz - 1;
   *d = s->list[s->sz];
   return true;
}

void cpyarr(stack* s, int position, stacktype* d)
{
    if(s == NULL){
        return;
    }
    *d = s->list[position];
}

bool exe_Pop(stack* s, stacktype* a, stacktype* b)
{
    if(s->sz < 2){
        return false;
    }
    stack_pop(s, a);
    stack_pop(s, b);
    return true;
}

bool issamesize(stacktype* a, stacktype* b)
{
    if(a->cols == 0 && a->rows == 0){
        return false;
    }else if(b->cols == 0 && b->rows == 0){
        return false;
    }else if((a->rows == b->rows) && (b->rows == 1) && (a->cols == b->cols) && (b->cols == 1)){
        return true;
    }else if(a->rows == 1 && a->cols == 1){
        return true;
    }else if(b->rows == 1 && b->cols == 1){
        return true;
    }else if(a->rows == b->rows && a->cols == b->cols){
        return true;
    }else{
        return false;
    }
}

bool isBinary(stacktype* d)
{
    if(d->rows == 1 && d->cols == 1){
        if(d->arr[0][0] == 0){
            return true;
        }else if(d->arr[0][0] == 1){
            return true;
        }else{
            return false;
        }
    }else if(d->rows != 0 && d->cols != 0){
        for(int j=0;j<d->rows; j++){
            for(int i=0; i<d->cols; i++){
                if(d->arr[j][i] != 1){
                    if(d->arr[j][i] != 0){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

//count neighbor
int cnt_nbr(stacktype* c, int j, int i)
{
    int cnt = 0;
    for(int y=-1; y<2; y++){
        if(j+y>=0 && j+y<c->rows){
            for(int x=-1; x<2; x++){
                if(i+x>=0 && i+x<c->cols){
                    if(c->arr[j+y][i+x] == 1){
                        cnt++;
                    }
                }
            }
        }
    }
    if(c->arr[j][i] == 1){
        cnt--;
    }
    return cnt;
}

void test(void)
{
    prog* p = ncalloc(1, sizeof(prog));
    p->wds[0][0] = '1';
    p->wds[0][1] = '2';
    p->wds[0][2] = '8';
    p->cw = 0;
    assert(isnum(p) == true);
    p->cw = p->cw + 1;
    
    p->wds[1][0] = '2';
    p->wds[1][1] = '5';
    p->wds[1][2] = '$';
    p->cw = 1;
    assert(isnum(p) == false);
    p->cw = p->cw + 1;
    
    stacktype* a = (stacktype*)ncalloc(1, sizeof(stacktype));
    stacktype* b = (stacktype*)ncalloc(1, sizeof(stacktype));
    a->cols = 1;
    a->rows = 1;
    b->rows = 5;
    b->cols = 5;
    assert(issamesize(a, b) == true);
    a->rows = 5;
    a->cols = 6;
    assert(issamesize(a, b) == false);
    a->cols = 5;
    assert(issamesize(a, b) == true);
    
    a->arr[0][0] = 5;
    assert(isBinary(a) == false);
    a->arr[0][0] = 1;
    assert(isBinary(a) == true);
    a->arr[1][1] = 1;
    a->arr[2][2] = 1;
    a->arr[3][3] = 1;
    a->arr[4][4] = 1;
    assert(isBinary(a) == true);
    assert(cnt_nbr(a, 1, 1) == 2);
    assert(cnt_nbr(a, 1, 0) == 2);
    assert(cnt_nbr(a, 1, 3) == 1);
    
    free(p);
    free(a);
    free(b);
}
