/* 
 * author: Oezguen Turgut 
 * date: 23/09/2019
 * description: reads a textfile, counts the occurrences of each character 
 *              in the word, and saves the result in a seperate textfile
 */

#include <stdio.h>
#include "count.h"


/***********************************************************************************
 * MAIN 
 **********************************************************************************/
int main (int argc, char* argv[]) {

    if (argc < 2) {
        printf("Syntax error: Argument file is missing.\n");
    } else {
        char* input_file = argv[1];
        data_t* linked_list = countOccurrences(input_file);
        saveLinkedList(CODE_FILE, linked_list);

        data_t* ptr = loadLinkedList(CODE_FILE);

        int size = getSize(ptr);
        printf("Size of linked list: %d\n", size);

        while (ptr != NULL) {
            printf("%c %d\n", ptr->character, ptr->occurrence);
            ptr = ptr->next;
        }
    }

	return 0;
}


/***********************************************************************************
 * FUNCTION DEFINITIONS
 **********************************************************************************/
data_t* countOccurrences(char file_name[]) {
    /* pointers for the linked list */
    data_t* ptr = NULL;
    data_t* prev_ptr = NULL;
    data_t* head = NULL;

    char first_datapoint = 1;
    char add_new_datapoint;

    /* create new data point */
    ptr = (data_t*) malloc(sizeof(data_t));
    ptr->code = NULL;
    ptr->next = NULL;
    /* store pointer pointing to the top of the linked list */
    head = ptr;

    /* open file to read from */
    FILE *fptr = fopen(file_name, "r");

    /* variable to store characters read from the file */
    char character;

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        /* read the line from the file */
        while ((fscanf(fptr, "%c", &character)) != EOF) {
            add_new_datapoint = 1;

            /* save the newline (\n) by using special character ^ (caret) */
            if(character == '\n') {
                character = '^';
            }

            /* check if data point for the current character already exists */
            data_t* aux_ptr = head;
            while (aux_ptr != NULL) {
                /* if so, increment occurrence of the character */
                if (aux_ptr->character == character) {
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
                ptr->character = character;
                /* obviously the character occurs at least once if a new 
                 * data point is needed */
                ptr->occurrence = 1;
                ptr->code = NULL;
                ptr->next = NULL; 
            }           
        }
    }

    /* close the file after reading its content */
    fclose(fptr);

    return head;
}

void saveLinkedList(char file_name[], data_t* linked_list) {
    FILE* fptr = fopen(file_name, "w");

    data_t* ptr = linked_list;
    while (ptr != NULL) {
        fprintf(fptr, "%c%d", ptr->character, ptr->occurrence);
        ptr = ptr->next;
    }

    fclose(fptr);
}

data_t* loadLinkedList(char file_name[]) {
    FILE* fptr = fopen(file_name, "r");

    data_t* linked_list = NULL;

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        char character;
        int occurrence;

        data_t* ptr = NULL;
        data_t* prev_ptr = NULL;
        char first_datapoint = 1;

        while (fscanf(fptr, "%c%d", &character, &occurrence) != EOF) {
            ptr = (data_t*) malloc(sizeof(data_t));

            if (first_datapoint) {
                linked_list = ptr;
                first_datapoint = 0;
            } else {
                prev_ptr->next = ptr;
            }

            prev_ptr = ptr;
            ptr->character = character;
            ptr->occurrence = occurrence;
            ptr->code = NULL;
            ptr->next = NULL;
        } 

    }

    fclose(fptr);
    return linked_list;
}

int getSize(data_t* linked_list) {
    data_t* ptr = linked_list;

    int size = 0;
    while (ptr != NULL) {
        size++;
        ptr = ptr->next;
    }

    return size;
}



