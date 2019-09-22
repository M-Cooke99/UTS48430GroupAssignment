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
    /* open file to read from it */
    FILE *fptr = fopen(file_name, "r");

    input_string_t* input_string = (input_string_t*) malloc(sizeof(input_string_t));
    input_string->str = NULL;
    char* buffer = NULL;
    size_t buffer_length = 0; 
    int nb_characters = 0;

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        /* read the line from the file */
        nb_characters = getline(&buffer, &buffer_length, fptr);

        if (nb_characters != -1) {
            input_string->str = buffer;

            /* save the newline by using special character ^ (caret) */
            if(input_string->str[nb_characters-1] == '\n') {
                input_string->str[nb_characters-1] = '^';
            }

            input_string->str_length = nb_characters;
        }
    }

    /* free the allocated buffer */
    free(buffer);
    buffer = NULL;

    /* close the file after reading its content */
    fclose(fptr);

    return input_string;
}

data_t* countOccurrences(input_string_t* input_string) {
    /* pointers for the linked list */
    data_t* ptr = NULL;
    data_t* prev_ptr = NULL;
    char first_datapoint = 1;
    char add_new_datapoint = 1;
    data_t* head = NULL;

    /* create new data point */
    ptr = (data_t*) malloc(sizeof(data_t));

    /* store pointer pointing to the top of the linked list */
    head = ptr;

    /* determine the occurrences of the characters */
    int i;
    for(i = 0; i < input_string->str_length; i++) {
        add_new_datapoint = 1;

        /* check if data point for the current character already exists */
        data_t* aux_ptr = head;
        while (aux_ptr != NULL) {
            /* if so, increment occurrence of the character */
            if (aux_ptr->character == input_string->str[i]) {
                aux_ptr->occurrence++;
                add_new_datapoint = 0;
                break;
            }

            aux_ptr = aux_ptr->next;
        }

        /* if no data point for the current character exists */
        if (add_new_datapoint) {
            if (first_datapoint) {
                /* don't need to allocate new memory for first data point */
                first_datapoint = 0;
            } else {
                ptr = (data_t*) malloc(sizeof(data_t));
                prev_ptr->next = ptr;
            }

            prev_ptr = ptr;
            ptr->character = input_string->str[i];
            /* obviously the character occurs at least once if a new 
             * data point is needed */
            ptr->occurrence = 1;
            ptr->next = NULL; 
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
