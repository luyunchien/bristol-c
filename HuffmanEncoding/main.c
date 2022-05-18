#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAXSIZE 500

typedef struct unit{
    char character;
    int times;
    bool isUsed;
    bool value;
    int parent_num;
}unit;

void read_file(char* filename, unit* list, int* length);
void addtolist(unit* list, char tmp, int* length);
//void printlist(unit* list, int length, int node_num);
void sort(unit* list, int length, int node_num);
bool issorted(unit* list, int length);
int times_exist(unit* list, int length, int node_num, int add);
void find_minimum(int* tmp1, int* tmp2, unit* list, int length, int node_num);
bool all_used(unit* list, int length, int node_num);
void gettree(unit* list, int length, int* node_num);
bool** getrelatives(unit* list, int length, bool** allcode, int* count1);
//void printresult(unit* list, bool** allcode, int length, int* count1);



int main(int argc, char * argv[]) {
    if(argc!=2){
        printf("invalid input\n");
        exit(EXIT_FAILURE);
    }
    char* filename = argv[1];
    unit* list = calloc(MAXSIZE, sizeof(unit*));
    int length = 0;
    int node_num = 0;
    int* count1 = NULL;
    
    read_file(filename, list, &length);
    sort(list, length, 0);
    
    gettree(list, length, &node_num);
    
    //printlist(list, length, node_num);

    bool** allcode = malloc(sizeof(bool**)*length);
    allcode = getrelatives(list, length, allcode, count1);
    
    
    //printresult(list, allcode, length, count1);
    
//    for(int j=0; j<length; j++){
//        for(int i=0; allcode[j][i]; i++){
//            printf("%d",allcode[j][i]);
//        }
//        printf("\n");
//    }
    
    
    free(allcode);
    free(list);
    return 0;
}

void read_file(char* filename, unit* list, int* length)
{
    FILE* file = fopen(filename, "r");
    if(!file){
        printf("cannot open the file\n");
        exit(EXIT_FAILURE);
    }
    char tmp;
    while(fscanf(file, "%c",&tmp) != EOF){
        if(tmp == '\n'){
            ;
        }else{
            //printf("%c %d\n",tmp, tmp);
            addtolist(list, tmp, length);
        }
    }
    //printf("length = %d\n",*length);
    fclose(file);
}


void addtolist(unit* list, char tmp, int* length)
{
    bool isNew = true;
    for(int j=0; j<=*length; j++){
        if(list[j].character == tmp){
            list[j].times++;
            isNew = false;
        }
    }
    if(isNew){
        list[*length].character = tmp;
        list[*length].times++;
        *length = *length + 1;
    }
    return;
}

//void printlist(unit* list, int length, int node_num)
//{
//    for(int i=0; i< length + node_num; i++){
//        printf("i = %d char '%c' %d times\n", i, list[i].character, list[i].times);
//    }
//}

void sort(unit* list, int length, int node_num){
    for(int i=0; i<length-1 + node_num; i++){
        for(int j=0; j<length-1 + node_num-i; j++){
            if(list[j].times > list[j+1].times){
                //printf("********\n");
                int tmp_time;
                int tmp_char;
                tmp_time = list[j].times;
                tmp_char = list[j].character;
                list[j].times = list[j+1].times;
                list[j].character = list[j+1].character;
                list[j+1].times = tmp_time;
                list[j+1].character = tmp_char;
            }
        }
        if(issorted(list, length-1)){
            return;
        }
    }
}

bool issorted(unit* list, int length){
    for(int i=0; i<length; i++){
        if(list[i].times > list[i+1].times)
            return false;
    }
    return true;
}

void gettree(unit* list, int length, int* node_num)
{
    length = length -1;
    *node_num = 0;
    int tmp1,tmp2;
    int add;

    while(!all_used(list, length, *node_num)){
    
        if(list[length + *node_num].times > list[length].times){
            tmp1 = length + *node_num;
            tmp2 = length + *node_num;
        }else{
            tmp1 = length;
            tmp2 = length;
        }
    
        find_minimum(&tmp1, &tmp2, list, length, *node_num);
        add = list[tmp1].times + list[tmp2].times;
        //printf("%d = %d + %d\n",add, list[tmp1].times,list[tmp2].times);
        *node_num = *node_num + 1;
        list[length + *node_num].times = add;
        list[length + *node_num].isUsed = false;
        list[tmp1].parent_num = length + *node_num;
        list[tmp2].parent_num = length + *node_num;
    
        if(list[tmp1].times <= list[tmp2].times){
            list[tmp1].value = 0;
            list[tmp2].value = 1;
        }else{
            list[tmp1].value = 1;
            list[tmp2].value = 0;
        }
    
    }
}

int times_exist(unit* list, int length, int node_num, int add)
{
    for(int i=0; i<length+1 + node_num; i++){
        if(list[i].isUsed == false && list[i].times == add){
            return i;
        }
    }
    return 0;
}

void find_minimum(int* tmp1, int* tmp2, unit* list, int length, int node_num)
{
    for(int i=0; i<length+node_num; i++){
        if(!list[i].isUsed && list[i].times<list[*tmp1].times){
            *tmp1 = i;
        }
    }
    list[*tmp1].isUsed = true;
    for(int j=0; j<length+node_num; j++){
        if(!list[j].isUsed && list[j].times<list[*tmp2].times){
            *tmp2 = j;
        }
    }
    list[*tmp2].isUsed = true;
}


bool all_used(unit* list, int length, int node_num)
{
    for(int j=0; j<length+node_num; j++){
        if(!list[j].isUsed){
            return false;
        }
    }
    return true;
}

bool** getrelatives(unit* list, int length, bool** allcode, int* count1)
{
    int relatives[MAXSIZE];
    int parent = 0;
    int cnt = 0;
    int k = 0;
    bool* code = malloc(sizeof(bool*)*MAXSIZE);
    int count[length];
    count1 = count;
    

    for(k=0; k<length; k++){
        parent = k;
        while(list[parent].parent_num){
            if(cnt == 0){
                relatives[cnt] = parent;
                cnt++;
            }else{
                parent = list[parent].parent_num;
                relatives[cnt] = parent;
                cnt++;
            }
        }
        
        count[k] = cnt;
        
        if(cnt<MAXSIZE){
            code = realloc(code, sizeof(bool*)*cnt);
        }

        allcode[k] = malloc(sizeof(bool*)*cnt);
        
        int n=0;
        for(int m=cnt-1; m>=0; m--){
            code[n] = list[relatives[m]].value;
            //printf("%d",code[n]);
            n++;
        }
        //printf("\n");
        

        for(int a=0; a<cnt; a++){
            allcode[k][a] = code[a];
            //printf("%d",allcode[k][a]);
        }
        //printf("\n");
        
        
        cnt = 0;
        for(int n=0; relatives[n]; n++){
            relatives[n] = 0;
            code[n] = 0;
        }

    }

    int len;
    for(int y=0; y<length; y++){
        len = count[y];
        printf("'%c': ",list[y].character);
        for(int z=0; z<len; z++){
            printf("%d",allcode[y][z]);
        }
        printf("( %d * %d )\n",len, list[y].times);
    }
    free(code);
    return allcode;
}
    

//void printresult(unit* list, bool** allcode, int length, int* count1)
//{
//    int cnt=0;
//    int len;
//    for(int i=0; i<length; i++){
//        len = count1[i];
//        printf("'%c': ",list[i].character);
//        for(int j=0; allcode[i][j]; j++){
//            printf("%d",allcode[i][j]);
//            cnt++;
//        }
//        printf("( %d * %d )\n",cnt, list[i].times);
//    }
//}
