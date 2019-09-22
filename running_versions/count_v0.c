/* author: Oezguen Turgut 
 * date: 13/08/2019
 * description: read a user input word from the prompt and output 
 * the occurrences of each character in the word
 */

#include <stdio.h>
#include <string.h> /* strlen */
#include <stdlib.h> /* malloc */

#define FILE_NAME       "input.txt"
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
input_string_t* getInput(char file_name[]);
data_t* countOccurrences(input_string_t* input_string);
void printData(data_t* linked_list);


/* MAIN */
int main (void) {

    input_string_t* input = getInput(FILE_NAME);
    data_t* linked_list = countOccurrences(input);
    printData(linked_list);

	return 0;
}


/* FUNCTION DEFINITIONS */
input_string_t* getInput(char file_name[]) {
    /*char* str = (char*) malloc(128 * sizeof(char));*/

    FILE *fptr = fopen(file_name, "r");

    input_string_t* input_string = (input_string_t*) malloc(sizeof(input_string_t));
    input_string->str = NULL;
    size_t buffer_length = 0; 
    int nb_characters = 0;

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        /* read the line from the file */
        nb_characters = getline(&input_string->str, &buffer_length, fptr);

        /* rename the newline */
        if(input_string->str[nb_characters-1] == '\n') {
            input_string->str[nb_characters-1] = '^';
        }

        input_string->str_length = nb_characters;
    }

    return input_string;
}


data_t* countOccurrences(input_string_t* input_string) {
    /* Variables */
    char* checked_chars;        /* auxiliary array storing characters being checked */
    checked_chars = (char*) malloc(NB_CHARACTERS * sizeof(char));
    int already_checked = 0;    /* flag to see if character has been checked previously */

    /* pointers for the linked list */
    data_t* ptr = NULL;
    data_t* prev_ptr = NULL;
    char first_datapoint = 1;
    data_t* head = NULL;

    /* determine the occurrences of the characters */
    int i,j;
    /* go through the entire string */
    for(i = 0; i < input_string->str_length; i++) {
        /* check if the current character has already been checked previously */
        for(j = 0; j < input_string->str_length; j++) {
            /* if so, activate the flag */
            if (input_string->str[i] == checked_chars[j])
                already_checked = 1;
        }
        /* depending on if the character has been checked previously or not */
        if(already_checked) {
            /* deactivate the flag and go on to the next character */
            already_checked = 0;
        } else {
            /* create new data point */
            ptr = (data_t*) malloc(sizeof(data_t));

            if (first_datapoint) {
                /* store pointer pointing to the top of the linked list */
                head = ptr;
                first_datapoint = 0;
            } else {
                /* store current pointer for the next data point */
                prev_ptr->next = ptr;
            }

            prev_ptr = ptr;

            /* set the attributes of the new data point */
            ptr->character = input_string->str[i];
            ptr->next = NULL; 

            /* check the number of occurances in the string */
            for(j = i; j < input_string->str_length; j++) {
                if(input_string->str[i] == input_string->str[j]) {
                    ptr->occurrence++;        
                }
            }

            /* mark the current character as checked */ 
            checked_chars[i] = input_string->str[i];
        }
    }

    return head;
}

void printData(data_t* linked_list) {
    data_t* ptr = linked_list;

    while (ptr != NULL) {
        printf("%c %-10d\n", ptr->character, ptr->occurrence);
        ptr = ptr->next;
    }
}
