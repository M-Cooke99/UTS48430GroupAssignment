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
        saveData(linked_list);
    }

    data_t* ptr = loadData(CODE_FILE);
    while (ptr != NULL) {
        printf("%c %d\n", ptr->character, ptr->occurrence);
        ptr = ptr->next;
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
    char first_datapoint = 1;
    char add_new_datapoint = 1;
    data_t* head = NULL;

    /* create new data point */
    ptr = (data_t*) malloc(sizeof(data_t));

    /* store pointer pointing to the top of the linked list */
    head = ptr;

    /* open file to read from */
    FILE *fptr = fopen(file_name, "r");

    /* variables for reading the file */
    input_string_t* input_string = (input_string_t*) malloc(sizeof(input_string_t));
    input_string->str = NULL;
    char* buffer = NULL;
    size_t buffer_length = 0; 
    int nb_characters = 0;

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        /* for the for loop below */
        int i;

        /* read the line from the file */
        while ((nb_characters = getline(&buffer, &buffer_length, fptr)) != -1) {
            input_string->str = buffer;

            /* save the newline (\n) by using special character ^ (caret) */
            if(input_string->str[nb_characters-1] == '\n') {
                input_string->str[nb_characters-1] = '^';
            }

            input_string->str_length = nb_characters;

            /* determine the occurrences of the characters */
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
        }
    }

    /* free the pointers used to read the file*/
    free(buffer);
    buffer = NULL;
    free(input_string);
    input_string = NULL;

    /* close the file after reading its content */
    fclose(fptr);

    return head;
}

void saveData(data_t* linked_list) {
    FILE* fptr = fopen(CODE_FILE, "w");

    /*int size = 0;*/
    data_t* ptr = linked_list;
   /* while (ptr != NULL) {
        size++;
        ptr = ptr->next;
    }
    fprintf(fptr, "%d\n", size);

    ptr = linked_list;*/
    while (ptr != NULL) {
        fprintf(fptr, "%c%d", ptr->character, ptr->occurrence);
        ptr = ptr->next;
    }

    fclose(fptr);
}

data_t* loadData(char file_name[]) {
    FILE* fptr = fopen(file_name, "r");



    data_t* linked_list = NULL;

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        char character;
        int occurrence;

        /*int size;

        fscanf(fptr, "%d", &size);*/

        data_t* ptr = NULL;
        data_t* prev_ptr = NULL;
        char first_datapoint = 1;

        /*int i;
        for (i = 0; i<size; i++) {
            fscanf(fptr, "%c %d", &character, &occurrence);

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
            ptr->next = NULL;
        }*/

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
            ptr->next = NULL;
        } 

    }

    fclose(fptr);
    return linked_list;
}





