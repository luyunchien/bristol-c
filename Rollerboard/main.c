#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#define MAXSIZE 6
#define MAXLIST 2000000

typedef struct board{
    int cur_board[MAXSIZE][MAXSIZE];
    int parent_num;
    int level;
}board;

void test(void);
void initial_board(char* filename, board* list, int* cnt, int* rows, int* cols);
void char_to_int(board* list, char initial[MAXSIZE][MAXSIZE], int rows, int cols);
void printlist(board* list, int cnt, int rows, int cols);
bool validboard(char initial[MAXSIZE][MAXSIZE], int rows, int cols, FILE* file);
bool isNew(board* list, int rows, int cols, board* new_board, int cnt);
void addtolist(board* list, int cnt, int rows, int cols, board* new_board, int parent_num);
bool getchild(board* list, int rows, int cols, int* cnt);
void cpy_parent2new(board* list, board* new_board, int rows, int cols, int parent_num);
bool rollleft(board* list, int rows, int cols, int* cnt, board* new_board, int parent_num, bool* child_exist);
bool rollright(board* list, int rows, int cols, int* cnt, board* new_board, int parent_num, bool* child_exist);
bool rollup(board* list, int rows, int cols, int* cnt, board* new_board, int parent_num, bool* child_exist);
bool rolldown(board* list, int rows, int cols, int* cnt, board* new_board, int parent_num, bool* child_exist);
bool isEqual(board* list, int rows, int cols, board* new_board, int num);
bool isAnswer(int cols, board* new_board);


int main(int argc, char * argv[]) {
    bool mode_print = false;
    char* filename = 0;
    if(argc == 2){
        filename = argv[1];
    }else if(argc == 3){
        if(strcmp(argv[1], "-v") == 0)
        {
            mode_print = true;
            filename = argv[2];
        }
    }else{
        printf("invalid input.\n");
        exit(EXIT_FAILURE);
    }
    
    test();
    board* list = (board*)malloc(sizeof(board*)*MAXLIST);
    if(!list){
        printf("cannot allocate space for the list.\n");
        exit(EXIT_FAILURE);
    }
    
    int cnt = 0, rows, cols;
    initial_board(filename, list, &cnt, &rows, &cols);
    //find the children boards
    //current child is the answer?
    if(getchild(list, rows, cols, &cnt)){
        if(mode_print == false){
            printf("%d moves\n",list[cnt].level);
        }else{
            printlist(list, cnt, rows, cols);
        }
    }else{
        printf("No solution?\n");
    }
    free(list);
    return 0;
}

void initial_board(char* filename, board* list, int* cnt, int* rows, int* cols)
{
    FILE* file = fopen(filename, "r");
    if(!file){
        printf("cannot open the file.\n");
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%d %d", rows, cols);
    char initial[MAXSIZE][MAXSIZE];
    //if the input is valid, initialize the board
    validboard(initial, *rows, *cols, file);
    //change type from char to bool
    char_to_int(list, initial, *rows, *cols);
    *cnt = 0;
    list[0].level = 0;
    fclose(file);
}

void char_to_int(board* list, char initial[MAXSIZE][MAXSIZE], int rows, int cols)
{
    for(int j=0; j<rows; j++){
        for(int i=0; i<cols; i++){
            if(initial[j][i] == '0'){
                list[0].cur_board[j][i] = 0;
            }
            if(initial[j][i] == '1'){
                list[0].cur_board[j][i] = 1;
            }
        }
    }
}

void printlist(board* list, int cnt, int rows, int cols)
{
    int steps = list[cnt].level;
    int* relatives = (int*)malloc((sizeof(int*))*(cnt+1));
    
    relatives[steps] = cnt;
    int father = cnt;
    int count = steps-1;
    
    while(count>=0){
        father = list[father].parent_num;
        relatives[count] = father;
        count--;
    }
    
    for(int i=0; i<steps+1; i++){
        int num = relatives[i];
        printf("%d:\n",i);
        for(int n=0; n<rows; n++){
            for(int m=0; m<cols; m++){
                printf("%d",list[num].cur_board[n][m]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

bool validboard(char initial[MAXSIZE][MAXSIZE], int rows, int cols, FILE* file)
{
    if(rows<=0 || rows>6 || cols<=0 || cols>6){
        printf("invalid size of board.\n");
        exit(EXIT_FAILURE);
    }
    int cnt_1 = 0, cnt_num = 0;
    for(int j=0; j<rows; j++){
        for(int i=0; i<cols; i++){
            char tmp;
            fscanf(file, "%c", &tmp);
            if(tmp == EOF){
                printf("There are fewer numbers than board size.\n");
            }else if(tmp == '0'){
                initial[j][i] = tmp;
                cnt_num++;
            }else if(tmp == '1'){
                initial[j][i] = tmp;
                cnt_1++;
                cnt_num++;
            }
            else if(tmp == '\n'){
                i = i-1;
            }else{
                printf("The numbers in the board should only be 1s or 0s.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    char more;
    int cnt_more = 0;
    while(fscanf(file, "%c", &more) != EOF){
        if(cnt_more != 0){
            if(more != ' ' && more != '\n'){
                printf("%d\n",(int)more);
                printf("There are more numbers than expected size.\n");
                exit(EXIT_FAILURE);
            }
        }
        cnt_more++;
    }
    if(cnt_1 != cols){
        printf("The number of 1s should be the same as that of cols.\n");
        exit(EXIT_FAILURE);
    }
    return 1;
}

bool getchild(board* list, int rows, int cols, int* cnt)
{
    int i = 0;
    int parent_num = 0;
    bool child_exist = 0;
    while (i <= *cnt) {
        child_exist = 0;
        board new_board;
        parent_num = i;
        cpy_parent2new(list, &new_board, rows, cols, parent_num);
        //see if answer comes from rolling left
        if(rollleft(list, rows, cols, cnt, &new_board, parent_num, &child_exist)){
            return 1;
        }
        //see if answer comes from rolling right
        if(rollright(list, rows, cols, cnt, &new_board, parent_num, &child_exist)){
            return 1;
        }
        //see if answer comes from rolling up
        if(rollup(list, rows, cols, cnt, &new_board, parent_num, &child_exist)){
            return 1;
        }
        //see if answer comes from rolling down
        if(rolldown(list, rows, cols, cnt, &new_board, parent_num, &child_exist)){
            return 1;
        }
        i++;
    }
    if(!child_exist){
        return 0;
    }
    return 1;

}

bool rollleft(board* list, int rows, int cols, int* cnt, board* new_board, int parent_num, bool* child_exist)
{
    int i, j;
    //roll the board left for one row
    for(j=0; j<rows; j++){
        new_board->cur_board[j][cols-1] = list[parent_num].cur_board[j][0];
        for(i=0; i<cols-1; i++){
            new_board->cur_board[j][i] = list[parent_num].cur_board[j][i+1];
        }
        //if the new board is unique, add it to the list
        if(isNew(list, rows, cols, new_board, *cnt)){
            *cnt = *cnt + 1;
            addtolist(list, *cnt, rows, cols, new_board, parent_num);
            *child_exist = 1;
        }
        //if the new board is the answer, ends the loop
        if(isAnswer(cols, new_board)){
            return 1;
        }
        //move the rolled row back
        cpy_parent2new(list, new_board, rows, cols, parent_num);
    }
    return 0;
}


bool rollright(board* list, int rows, int cols, int* cnt, board* new_board, int parent_num, bool* child_exist)
{
    int i, j;
    for(j=0; j<rows; j++){
        new_board->cur_board[j][0] = list[parent_num].cur_board[j][rows-1];
        for(i=cols-2; i>=0; i--){
            new_board->cur_board[j][i+1] = list[parent_num].cur_board[j][i];
        }
        if(isNew(list, rows, cols, new_board, *cnt)){
            *cnt = *cnt + 1;
            addtolist(list, *cnt, rows, cols, new_board, parent_num);
            *child_exist = 1;
        }
        if(isAnswer(cols, new_board)){
            return 1;
        }
        cpy_parent2new(list, new_board, rows, cols, parent_num);
    }
    return 0;
}

bool rollup(board* list, int rows, int cols, int* cnt, board* new_board, int parent_num, bool* child_exist)
{
    int i, j;
    for(i=0; i<cols; i++){
        new_board->cur_board[rows-1][i] = list[parent_num].cur_board[0][i];
        for(j=0; j<rows-1; j++){
            new_board->cur_board[j][i] = list[parent_num].cur_board[j+1][i];
        }
        if(isNew(list, rows, cols, new_board, *cnt)){
            *cnt = *cnt + 1;
            addtolist(list, *cnt, rows, cols, new_board, parent_num);
            *child_exist = 1;
        }
        if(isAnswer(cols, new_board)){
            return 1;
        }
        cpy_parent2new(list, new_board, rows, cols, parent_num);
    }
    return 0;
}

bool rolldown(board* list, int rows, int cols, int* cnt, board* new_board, int parent_num, bool* child_exist)
{
    int i, j;
    for(i=0; i<cols; i++){
        new_board->cur_board[0][i] = list[parent_num].cur_board[rows-1][i];
        for(j=rows-2; j>=0; j--){
            new_board->cur_board[j+1][i] = list[parent_num].cur_board[j][i];
        }
        if(isNew(list, rows, cols, new_board, *cnt)){
            *cnt = *cnt + 1;
            addtolist(list, *cnt, rows, cols, new_board, parent_num);
            *child_exist = 1;
        }
        if(isAnswer(cols, new_board)){
            return 1;
        }
        cpy_parent2new(list, new_board, rows, cols, parent_num);
    }
    return 0;
}

void cpy_parent2new(board* list, board* new_board, int rows, int cols, int parent_num)
{
    for(int l=0; l<rows; l++){
        for(int k=0; k<cols; k++){
            new_board->cur_board[l][k] = list[parent_num].cur_board[l][k];
        }
    }
}

bool isNew(board* list, int rows, int cols, board* new_board, int cnt)
{
    for(int k=0; k<=cnt; k++){
        if(isEqual(list, rows, cols, new_board, k)){
            return 0;
        }
    }
    return 1;
}

bool isEqual(board* list, int rows, int cols, board* new_board, int num)
{
    for(int j=0; j<rows; j++){
        for(int i=0; i<cols; i++){
            if(new_board->cur_board[j][i] != list[num].cur_board[j][i]){
                return 0;
            }
        }
    }
    return 1;
}

void addtolist(board* list, int cnt, int rows, int cols, board* new_board, int parent_num)
{
    for(int j=0; j<rows; j++){
        for(int i=0; i<cols; i++){
            list[cnt].cur_board[j][i] = new_board->cur_board[j][i];
        }
    }
    list[cnt].level = list[parent_num].level+1;
    list[cnt].parent_num = parent_num;
}

bool isAnswer(int cols, board* new_board)
{
    for(int i=0; i<cols; i++){
        if(new_board->cur_board[0][i] != 1){
            return 0;
        }
    }
    return 1;
}


void test(void)
{
    int cnt = 0, rows = 4, cols = 4;
    board* testlist = (board*)malloc((sizeof(board*))* 2000);
    assert(testlist);
    for(int j=0; j<rows; j++){
        for(int i=0; i<cols; i++){
            testlist[0].cur_board[j][i] = 0;
        }
    }
    testlist[0].cur_board[0][0] = 1;
    testlist[0].cur_board[1][1] = 1;
    testlist[0].cur_board[2][2] = 1;
    testlist[0].cur_board[3][3] = 1;

    board new;
    
    for(int n=0; n<rows; n++){
        for(int m=0; m<cols; m++){
            new.cur_board[n][m] = 0;
        }
    }
    new.cur_board[0][0] = 1;
    new.cur_board[1][1] = 1;
    new.cur_board[2][2] = 1;
    new.cur_board[3][3] = 1;
    assert(isEqual(testlist, rows, cols, &new, 0));
    assert(isNew(testlist, rows, cols, &new, cnt) == 0);
    assert(!isAnswer(rows, &new));
    
    
    for(int n=0; n<rows; n++){
        for(int m=0; m<cols; m++){
            new.cur_board[n][m] = 0;
        }
    }
    new.cur_board[0][0] = 1;
    new.cur_board[1][3] = 1;
    new.cur_board[2][2] = 1;
    new.cur_board[3][3] = 1;
    assert(!isEqual(testlist, rows, cols, &new, 0));
    assert(isNew(testlist, rows, cols, &new, cnt) == 1);
    assert(!isAnswer(rows, &new));
    
    for(int n=0; n<rows; n++){
        for(int m=0; m<cols; m++){
            new.cur_board[n][m] = 0;
        }
    }
    new.cur_board[0][1] = 1;
    new.cur_board[1][0] = 1;
    new.cur_board[2][2] = 1;
    new.cur_board[3][3] = 1;
    assert(!isEqual(testlist, rows, cols, &new, 0));
    assert(isNew(testlist, rows, cols, &new, cnt) == 1);
    assert(!isAnswer(rows, &new));
    
    board ans_board;
    for(int n=0; n<rows; n++){
        for(int m=0; m<cols; m++){
            if(n == 0){
            ans_board.cur_board[n][m] = 1;
            }else{
            ans_board.cur_board[n][m] = 0;
            }
        }
    }
    assert(!isEqual(testlist, rows, cols, &ans_board, 0));
    assert(isNew(testlist, rows, cols, &ans_board, cnt) == 1);
    assert(isAnswer(rows, &ans_board));
    
    free(testlist);
}
