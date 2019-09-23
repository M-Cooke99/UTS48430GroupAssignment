#ifndef COUNT_H
#define COUNT_H

/* LIBRARIES */
#include <string.h> /* strlen */
#include <stdlib.h> /* malloc */

/* HASH DEFINES */
#define INPUT_FILE      "love_poem_58.txt"
#define CODE_FILE       "code.txt"
#define NB_CHARACTERS   256

/* STRUCTURES */
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

/* FUNCTION PROTOTYPES */
data_t* countOccurrences(char file_name[]);
void saveLinkedList(data_t* linked_list);
data_t* loadLinkedList(char file_name[]);
int getSize(data_t* linked_list);

#endif /* COUNT_H */