#ifndef COUNT_H
#define COUNT_H


/***********************************************************************************
 * LIBRARIES 
 **********************************************************************************/
#include <string.h> /* strlen */
#include <stdlib.h> /* malloc */


/***********************************************************************************
 * HASH DEFINES 
 **********************************************************************************/
#define INPUT_FILE      "love_poem_58.txt"
#define CODE_FILE       "code.txt"
#define NB_CHARACTERS   256


/***********************************************************************************
 * STRUCTURES 
 **********************************************************************************/
struct data {
    char character;
    int occurrence;
    int code[128];
    struct data* next;
};
typedef struct data data_t;

struct input_string {
    char* str;
    int str_length;
};
typedef struct input_string input_string_t;


/***********************************************************************************
 * FUNCTION PROTOTYPES 
 **********************************************************************************/
/* counts occurrences of each character appearing in a given text file */
data_t* countOccurrences(char file_name[]);

/* saves the linked list which contains all characters and their occurrences */
void saveLinkedList(data_t* linked_list);

/* loads the linked list which contains all characters and their occurrences */
data_t* loadLinkedList(char file_name[]);

/* determines the size of a linked list */
int getSize(data_t* linked_list);


#endif /* COUNT_H */