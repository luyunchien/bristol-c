#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define ITER 10
#define MAXGRID 40
char board[MAXGRID+2][MAXGRID+2];
char boardtmp[MAXGRID+2][MAXGRID+2];
int Row = 0;                                    //Row of input file
void Wireworld(char* filename, int iter);
bool FileToBoard(char* filename);
void PrintBoard(void);
char CellNextState(int cellY, int cellX);
void BoardNextState(int iter);
bool CopperToHead(int copperY, int copperX);

int main(int argc, char** argv)
{
    if (argc == 2)
        Wireworld(argv[1], ITER);
    else
        printf("Please input a valid file.\n");
}

void Wireworld(char* filename, int iter)        //Wireworld function
{
    //initialisation
    memset(board, 0, sizeof(board));
    memset(boardtmp, 0, sizeof(boardtmp));
    //Input the file to board
    bool read_success = FileToBoard(filename);
    if (!read_success)
    {
        printf("Invalid file. Please try again.\n");
        return;
    }
    PrintBoard();
    BoardNextState(0);
}

bool FileToBoard(char* filename)
{
    FILE* myfile = fopen(filename, "r");
    if (myfile == NULL)
        return 0;
    char str[MAXGRID];
    for ( Row = 1; fgets(str, MAXGRID, myfile) != NULL; ++Row )
    {
        unsigned long len = strlen(str);
        if ( str[len-1] == '\n' )
            str[len-1] = '\0';
        char *p = &board[Row][1];
        strcpy(p, str);
    }
    return 1;
}

void PrintBoard(void)
{
    for ( int i = 1; i < Row; ++i )
    {
        char* p = &board[i][1];
        printf("%s\n", p);
    }
    //printf("Row = %d\n", Row);
    return;
}

void BoardNextState(int iter)
{
    if ( iter == ITER )    return;
    for ( int j = 1; j < Row; ++j )
        for ( int i = 1; board[j][i] != '\0'; ++i )
        {
            boardtmp[j][i] = CellNextState(j, i);
            //printf("Now (%d, %d) is in calculation.\n", j, i);
            //printf("Next is: %c. Current is %c. \n", boardtmp[j][i], board[j][i]);
        }
    printf("This is result of turn %d.\n", iter);
    for ( int j = 1; j < Row; ++j )
    {
        char* ptmp = &boardtmp[j][1];
        char* p = &board[j][1];
        strcpy(p, ptmp);
    }
    PrintBoard();
    memset (boardtmp, 0, sizeof(boardtmp));
    BoardNextState(iter+1);
}

char CellNextState(int cellY, int cellX)
{
    switch (board[cellY][cellX])
    {
        case ' ':    return ' '; break;
        case 'H':    return 't'; break;
        case 't':    return 'c'; break;
        case 'c':
            if (CopperToHead(cellY, cellX))
                return 'H';
            else
                return 'c';
            break;
        default:    return 0; break;
    }
}

bool CopperToHead(int copperY, int copperX)
{
    int HeadNeighbour = 0;
    //printf("Now check (%d,%d)\n", copperY, copperX);
    for ( int j = -1; j <= 1; ++j )
        for ( int i = -1; i <= 1; ++i )
        {
            //printf ("Neighbour is %c\n", board[copperY+j][copperX+i]);
            if (board[copperY+j][copperX+i] == 'H')
                ++HeadNeighbour;
        }
    if ( HeadNeighbour == 1 || HeadNeighbour == 2 )
        return 1;
    return 0;
}
