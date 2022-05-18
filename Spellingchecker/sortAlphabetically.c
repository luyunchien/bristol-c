#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFF_SIZE 10
#define START_SIZE 1000
#define SCALE_FACTOR 3

/* A program to read the contents of a file into a dynamically allocated array */

FILE* h_open(char* filename);
char** read_file(FILE* file_pointer);
char** init_words_list(void);
char** h_realloc(char** words_list, int newsize);
void* h_malloc(int size);
void print_words(char** words_list);
void free_list(char** words_list);

void movebackward(char** words_list, int i, int storage_space);


int main(int argc, char* argv[]){
    if(argc!=2)
    {
        perror("invalid input.\n");
    }
    char* filename = argv[1];
    FILE* file_pointer = h_open(filename);
    char** words_list = read_file(file_pointer);
    print_words(words_list);
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

char** read_file(FILE* file_pointer){
    char buffer[BUFF_SIZE];
    char** words_list = init_words_list();
    int word_count = 0;
    int storage_space = START_SIZE;
    int newsize;

    while(fscanf(file_pointer, "%s", buffer) != EOF){
        
        
        if(word_count >= storage_space - 1){
            newsize = storage_space * SCALE_FACTOR;
            words_list = h_realloc(words_list, newsize);
            storage_space = newsize;
        }
        words_list[word_count] = (char*)h_malloc(sizeof(char) * (int)strlen(buffer) + 1);
  
        if(word_count == 0)
            strcpy(words_list[word_count], buffer);
        if(word_count == 1)
        {
            if(strcmp(buffer, words_list[0]) < 0)
            {
                strcpy(words_list[word_count], words_list[0]);
                strcpy(words_list[0], buffer);
            }
        }

        bool findlocation = 0;
        for(int i = 0; i < word_count && findlocation == 0; i++)
        {
            if(strcmp(buffer, words_list[i]) < 0)
            {
                movebackward(words_list, i, word_count);
                strcpy(words_list[i], buffer);
                findlocation = 1;
            }
        }
        memset(buffer, 0, sizeof(buffer));
        word_count++;
    }

    if(word_count == storage_space){
        words_list = h_realloc(words_list, ++storage_space);
    }
    return words_list;
}

char** init_words_list(void){
    char** words_list = (char**)malloc(sizeof(char*) * START_SIZE);
    if(!words_list){
        perror("Couldn't initialise the words list array.");
    }
    return words_list;
}

char** h_realloc(char** words_list, int newsize){
    char** new_words_list = realloc(words_list, sizeof(char*) * newsize);
    if(!new_words_list){
        perror("Couldn't allocate space for words list.");
    }
    return new_words_list;
}

void* h_malloc(int size){
    void* ptr = malloc(size);
    if(!ptr){
        perror("Couldn't allocate space");
    }
    return ptr;
}

void print_words(char** words_list){
    if(!words_list)
        perror("invalid pointer\n");
    int i = 0;
    while(words_list[i][0]){
        printf("%s\n", words_list[i++]);
    }
}

void free_list(char** words_list){
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
