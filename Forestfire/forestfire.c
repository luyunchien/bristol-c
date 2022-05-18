#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define MAX_COL 80
#define MAX_ROW 30
#define G 250
#define L 10*G
#define ITERATION 1000

char forest[MAX_ROW][MAX_COL];
char forest_tmp[MAX_ROW][MAX_COL];
void test(void);
char empty_to_tree(int y, int x);
char tree_to_fire(int y, int x);
void print_forest(void);
bool is_inside(int y, int x);
char cell_next_state(int y, int x);
void forest_next_state(int iter);

int main()
{
    test();
    srand((unsigned)time(NULL));
    memset(forest, '@', sizeof(forest));
    for(int i = 0; i < 80; i++)
    {
        int a = rand()%80;
        int b = rand()%30;
        forest[b][a] = '*';
    }
    for(int j = 0; j < MAX_ROW; j++)
    {
        strncpy(forest_tmp[j], forest[j], MAX_COL);
    }
    //memset(forest_tmp, '@', sizeof(forest));
    print_forest();
    forest_next_state(0);
    return 0;
}

void forest_next_state(int iter)
{
    if(iter == ITERATION)
        return;
    for(int j = 0; j < MAX_ROW; j++)
    {
        for(int i = 0; i < MAX_COL; i++)
        {
            forest_tmp[j][i] = cell_next_state(j, i);
        }
    }
    printf("This is the result of turn %d:\n",iter);
    for(int j = 0; j < MAX_ROW; j++)
    {
        //char* pf = &forest[j][0];
        //char* ptmp = &forest_tmp[j][0];
        //strcpy(pf, ptmp);
        strncpy(forest[j], forest_tmp[j], MAX_COL);
    }
    print_forest();
    memset(forest_tmp, ' ', sizeof(forest));
    forest_next_state(iter+1);
    
}

char cell_next_state(int y, int x)
{
    switch (forest[y][x]) {
        case '*':
            return ' ';
            break;
        case '@':
            return(tree_to_fire(y, x));
            break;
        case ' ':
            return(empty_to_tree(y, x));
            break;
        default:
            return 0;
            break;
    }
}

char empty_to_tree(int y, int x)
{
    //srand((unsigned)time(NULL));
    if((rand()%G) == 1)
        return '@';
    return ' ';
}

char tree_to_fire(int y, int x)
{
    bool fire_nbr = 0;
    for(int j=-1; j<=1; j++)
    {
        for(int i=-1; i<=1; i++)
        {
            if((is_inside(y+j, x+i)) == 1)
            {
                if(forest[y+j][x+i] == '*')
                    fire_nbr = 1;
            }
        }
    }
    if(fire_nbr == 1)
    {
        return '*';
    }
    //srand((unsigned)time(NULL));
    //int random = rand()%L;
    //printf("-%d", random);
    if((rand()%L) == 1)
        return '*';
    return '@';
}

bool is_inside(int y, int x)
{
    if(y < 0 || x < 0 || y > MAX_ROW || x > MAX_COL)
        return 0;
    else return 1;
}


void print_forest(void)
{
    for (int j = 0; j < MAX_ROW; j++)
    {
        for(int i = 0; i < MAX_COL; i++)
            printf("%c", forest[j][i]);
        printf("\n");
    }
    printf("\n\n");
}

void test(void)
{
    assert(is_inside(-1, 5) == 0);
    assert(is_inside(45, 5) == 0);
    assert(is_inside(5, -29) == 0);
    assert(is_inside(20, 90) == 0);
    assert(is_inside(5, 8) == 1);
}

