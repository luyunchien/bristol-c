//
//  main.c
//  SpellingChecker7.1
//
//  Created by mac on 10/11/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define BUFF_SIZE 10
#define START_SIZE 1000
#define SCALE_FACTOR 3

/* A program to read the contents of a file into a dynamically allocated array */

FILE* h_open(char* filename);
char** read_file(FILE* file_pointer, int* word_count);
char** init_words_list(void);
char** h_realloc(char** words_list, int newsize);
void* h_malloc(int size);
void print_words(char** words_list);
void free_list(char** words_list);
void movebackward(char** words_list, int i, int storage_space);
bool binary_search(char* goal_word, char** wordlist, int smallest, int largest);


int main(int argc, char* argv[]){
    if(argc!=3)
    {
        perror("invalid input.\n");
    }
    char* filename = argv[1];
    char* goal_word = argv[2];
    FILE* file_pointer = h_open(filename);
    int word_count = 0;
    char** words_list = read_file(file_pointer, &word_count);
    //printf("wordcount = %d\n", word_count);
    if(binary_search(goal_word, words_list, 0, word_count-1) == 1)
        printf("The word %s is in the list.\n", goal_word);
    else
        printf("The word %s is NOT in the list.\n", goal_word);
    
    //print_words(words_list);
    free_list(words_list);
    return 0;
}

FILE* h_open(char* filename){
    FILE* file_pointer = fopen(filename, "r");
    if(!file_pointer){
        perror("Couldn't open file\n");
    }
    return file_pointer;
}

char** read_file(FILE* file_pointer, int* word_count)
{
    char buffer[BUFF_SIZE];
    char** words_list = init_words_list();
    //int word_count = 0;
    int storage_space = START_SIZE;
    int newsize;

    int word_cnt = 0;
    
    while(fscanf(file_pointer, "%s", buffer) != EOF){
        
        if(word_cnt >= storage_space - 1){
            newsize = storage_space * SCALE_FACTOR;
            words_list = h_realloc(words_list, newsize);
            storage_space = newsize;
        }
        words_list[word_cnt] = (char*)h_malloc(sizeof(char) * (int)strlen(buffer) + 1);
  
        if(word_cnt == 0)
            strcpy(words_list[word_cnt], buffer);
        if(word_cnt == 1)
        {
            if(strcmp(buffer, words_list[0]) < 0)
            {
                strcpy(words_list[word_cnt], words_list[0]);
                strcpy(words_list[0], buffer);
            }
        }

        bool findlocation = 0;
        for(int i = 0; i < word_cnt && findlocation == 0; i++)
        {
            if(strcmp(buffer, words_list[i]) < 0)
            {
                movebackward(words_list, i, word_cnt);
                strcpy(words_list[i], buffer);
                findlocation = 1;
            }
        }
        memset(buffer, 0, sizeof(buffer));
        word_cnt++;
        
        //printf("wordcount = %d\n", word_count);
    }

    //printf("word_cnt = %d\n", word_cnt);
    *word_count = word_cnt;
    
    if(word_cnt == storage_space){
        words_list = h_realloc(words_list, ++storage_space);
    }
    return words_list;
}

char** init_words_list(void)
{
    char** words_list = (char**)malloc(sizeof(char*) * START_SIZE);
    if(!words_list){
        perror("Couldn't initialise the words list array.");
    }
    return words_list;
}

char** h_realloc(char** words_list, int newsize)
{
    char** new_words_list = realloc(words_list, sizeof(char*) * newsize);
    if(!new_words_list){
        perror("Couldn't allocate space for words list.");
    }
    return new_words_list;
}

void* h_malloc(int size)
{
    void* ptr = malloc(size);
    if(!ptr){
        perror("Couldn't allocate space");
    }
    return ptr;
}

void print_words(char** words_list)
{
    if(!words_list)
        perror("invalid pointer\n");
    int i = 0;
    while(words_list[i][0]){
        printf("%s\n", words_list[i++]);
    }
}

void free_list(char** words_list)
{
    int i=0;
    while(words_list[i]){
        free(words_list[i++]);
    }
    free(words_list);
}

void movebackward(char** words_list, int i, int word_count)
{
    for(int j = word_count; j > i; j-- )
    {
        strcpy(words_list[j], words_list[j-1]);
    }
}

bool binary_search(char* goal_word, char** wordlist, int smallest, int largest)
{
    while(smallest <= largest)
    {
        int m = (smallest+largest)/2;
        
        if(strcmp(goal_word, wordlist[m]) == 0 || strcmp(goal_word, wordlist[smallest]) == 0 || strcmp(goal_word, wordlist[largest]) == 0)
        {
            return 1;
        }
        else{
            if(strcmp(goal_word, wordlist[m]) > 0){
                smallest = m + 1;
            }
            else{
                largest = m - 1;
            }
        }
    }
    return 0;
}
