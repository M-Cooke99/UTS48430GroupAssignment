/* 
 * author:      Oezguen Turgut 
 * date:        15/10/2019
 * description: read a textfile, compress it, and decompress it again via 
 *              huffman coding
 */


#include "huffman.h"


/*****************************************************************************
 * MAIN 
 *****************************************************************************/
int main (int argc, char* argv[]) {

    /* check if file to compress is passed to the program */
    if (argc < 2) {
       printf("Syntax error\n");
       printf("Correct syntax: %s option argument_file\n", argv[0]);
       printf("The following options are available:\n");
       printf("%8s %-6s %-12s %s\n", " ", "-c", "Compress", 
        "(argument_file required)");
       printf("%8s %-6s %-12s %s\n", " ", "-d", "Decompress", 
        "(argument_file required)");
    } else {
        if (strcmp(argv[1], "-c") == 0 && argc == 3) {
            char* plain_file = argv[2];
            HuffmanCompression(plain_file); 
        } else if (strcmp(argv[1], "-d") == 0 && argc == 3) {
            char* compressed_file = argv[2];
            HuffmanDecompression(compressed_file, CODE_FILE);
        } else {
            printf("The following options are available:\n");
            printf("%8s %-6s %-12s %s\n", " ", "-c", "Compress", 
                "(argument_file required)");
            printf("%8s %-6s %-12s %s\n", " ", "-d", "Decompress", 
                "(argument_file required)");
        }
    }

    return 0;
}


/*****************************************************************************
 * FUNCTION DEFINITION 
 *****************************************************************************/
min_heap_node_t* createNode (char character, int occurrence) {
    /* create a new node */
    min_heap_node_t* node = (min_heap_node_t*) malloc(sizeof(min_heap_node_t));
    
    /* set all attributes */
    (*node).character = character;
    (*node).occurrence = occurrence;
    (*node).left = NULL;
    (*node).right = NULL;

    return node;
}

min_heap_t* createMinHeap (int capacity) {
    /* create a new min heap tree */
    min_heap_t* min_heap = (min_heap_t*) malloc(sizeof(min_heap_t));

    /* set all attributes */
    (*min_heap).size = 0;
    (*min_heap).capacity = capacity;
    (*min_heap).array = (min_heap_node_t**) malloc(((*min_heap).capacity) * 
        sizeof(min_heap_node_t*));

    return min_heap;
}

void swapNodes (min_heap_node_t** node_a, min_heap_node_t** node_b) {
    min_heap_node_t* temp = *node_a;
    *node_a = *node_b;
    *node_b = temp;
}

void minHeapify (min_heap_t* min_heap, int index) {
    /* array index of node with the lowest freq */
    int smallest = index;
    /* array index of its left child node */
    int left = 2 * index + 1;
    /* array index of its right child node */
    int right = 2 * index + 2;

    /* check if there is a left child at all */
    if (left < min_heap->size) {
        /* check if freq of left child is lower than freq of current node */
        if (min_heap->array[left]->occurrence < 
            min_heap->array[smallest]->occurrence) 
            smallest = left;
    }

    /* check if there is a right child at all */
    if (right < min_heap->size) {
        /* check if freq of right child is lower than freq of current node */
        if (min_heap->array[right]->occurrence < 
            min_heap->array[smallest]->occurrence) 
           smallest = right;
    }

    /* check if there was a node with a lower freq */
    if(smallest != index) {
        swapNodes(&(*min_heap).array[smallest], &(*min_heap).array[index]); 
        /* apply min heapify on subtree of the swapped node */
        minHeapify(min_heap, smallest);
    }
}

min_heap_node_t* extractMin (min_heap_t* min_heap) {
    /* node with lowest freq is ALWAYS at index 0 (given a min heap tree) */
    min_heap_node_t* min_node = min_heap->array[0];

    /* get the last element of the array and put it at index 0 
     * now the min heap tree is not sorted correctly anymore    */
    min_heap->array[0] = min_heap->array[(min_heap->size) - 1];
    /* decrease the size of the array since a node with lowest freq 
     * was extracted */
    (min_heap->size)--;
    /* rearrange the min heap tree starting from the root */
    minHeapify(min_heap, 0);

    return min_node;
}

void insertNode (min_heap_t* min_heap, min_heap_node_t* node) {
    /* increase size of the min heap tree */
    (min_heap->size)++;
    /* set index of the new node */
    int i = (min_heap->size)-1;

    /* while freq of new node is lower than freq of node at index (i-1)/2 */
    while (i > 0 && 
        node->occurrence < min_heap->array[(i - 1) / 2]->occurrence) {
        /* node in the middle of the array is replaced to the end of the 
         * array */
        min_heap->array[i] = min_heap->array[(i - 1) / 2];
        /* index is reset */
        i = (i - 1) / 2;
    }

    min_heap->array[i] = node;
}

void buildMinHeap (min_heap_t* min_heap) {
    int n = min_heap->size - 1;
    int i;

    for (i = (n-1)/2; i >= 0; i--)
        minHeapify(min_heap, i);
}

min_heap_t* createAndBuildMinHeap (data_t* linked_list) {
	int size = getListSize(linked_list);

    /* create the min heap */
    min_heap_t* min_heap = createMinHeap(size);
    /* set size of the min heap */
    (*min_heap).size = size;

    /* create a node for each character in the linked list */
    data_t* ptr = linked_list;
    int i = 0;
    while (ptr != NULL) {
    	(*min_heap).array[i] = createNode(ptr->character, ptr->occurrence);
    	ptr = ptr->next; 
    	i++;
    }

    /* build the min heap */
    buildMinHeap(min_heap);

    return min_heap;
}

min_heap_node_t* buildHuffmanTree (data_t* linked_list) {
    /* define node and children */
    min_heap_node_t *left, *right, *top;

    /* create and build a min heap tree */
    min_heap_t* min_heap = createAndBuildMinHeap(linked_list);

    while (min_heap->size != 1) {
        /* node with lowest freq is left child */
        left = extractMin(min_heap);
        /* node with next higher freq is right child */
        right = extractMin(min_heap);

        /* create an internal node with frequency equal to the sum of
         * the frequencies of the left and right child, furthermore
         * make it parent of left and right node */
        top = createNode('$', (*left).occurrence + (*right).occurrence);
        top->left = left;
        top->right = right;

        /* insert the new internal node into the min heap tree */
        insertNode(min_heap, top);
    }

    /* remaining node is the root of the huffman tree */
    return extractMin(min_heap);
}

void createCode (char file_name[], min_heap_node_t* node, 
    int* code, int current_node) {
    if (node->left) {
        /* assign 0 to the left node */
        code[current_node] = 0;
        createCode(file_name, node->left, code, (current_node+1));
    }

    if (node->right) {
        /* assign 1 to the right node */
        code[current_node] = 1;
        createCode(file_name, node->right, code, (current_node+1));
    }    

    /* the node is a leaf of the tree */
    if (node->left == NULL && node->right == NULL) {
        /* open the file to store the <character, code> pairs in */
        FILE* fptr = fopen(file_name, "a");

        /* store the current <character, code> pair into the file */
        fprintf(fptr, "< %c , ", node->character); 
        int i;
        for(i = 0; i < current_node; i++) {
            fprintf(fptr, "%d", code[i]); 
        }
        fprintf(fptr, " >\n");

        fclose(fptr);
    }
}

data_t* countOccurrences(char file_name[]) {
    /* pointers for the linked list */
    data_t* ptr = NULL;
    data_t* prev_ptr = NULL;
    data_t* head = NULL;

    char first_datapoint = 1;
    char add_new_datapoint;

    /* create new data point */
    ptr = (data_t*) malloc(sizeof(data_t));
    /*ptr->code = " ";*/
    ptr->next = NULL;
    /* store pointer pointing to the top of the linked list */
    head = ptr;

    /* open text file to read from */
    FILE *fptr = fopen(file_name, "r");

    /* variable to store characters read from the file */
    char character;

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        /* read the line from the file */
        while ((fscanf(fptr, "%c", &character)) != EOF) {
            add_new_datapoint = 1;

            
            if(character == '\n') {
                /* save the newline (\n) by using 
                 * special character ^ (caret) */
                character = '^';
            } else if (character == ' ') {
                /* save the whitespace by using 
                 * special character * (asterisk) */
                character = '*';
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
                    /* don't need to allocate new memory for first 
                     * data point */
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
                /*ptr->code = " ";*/
                ptr->next = NULL; 
            }           
        }
    }

    /* close the text file after reading its content */
    fclose(fptr);

    return head;
}

data_t* loadCode(char file_name[]) {
    /* open the file to read the <character, code> pairs from */
    FILE* fptr = fopen(file_name, "r");

    data_t* linked_list = NULL;

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        char character;
        char code[MAX_CODE_SIZE];

        data_t* ptr = NULL;
        data_t* prev_ptr = NULL;
        char first_datapoint = 1;

        /* while the end of file (EOF) is not reached */
        while (fscanf(fptr, "< %c , %s >\n", &character, code) != EOF) {
            /* create a data point for each <character, code> pair in the 
             * file */
            ptr = (data_t*) malloc(sizeof(data_t));

            if (first_datapoint) {
                linked_list = ptr;
                first_datapoint = 0;
            } else {
                prev_ptr->next = ptr;
            }

            prev_ptr = ptr;
            /* store the character */
            ptr->character = character;
            ptr->occurrence = 0;
            /* store the code */
            int i;
            for (i = 0; i < strlen(code); i++) {
                ptr->code[i] = code[i];
            }
            ptr->code[strlen(code)+1] = '\0';
            ptr->next = NULL;
        } 

    }

    fclose(fptr);
    return linked_list;
}

int getListSize(data_t* linked_list) {
    data_t* ptr = linked_list;

    int size = 0;
    while (ptr != NULL) {
        size++;
        ptr = ptr->next;
    }

    return size;
}

int getFileLength(char file_name[]) {
    int file_length = 0;

    /* open the file to determine its length */
    FILE *fptr = fopen(file_name, "r");

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        /* count number of characters until EOF is reached */
        char character;
        while (fscanf(fptr, "%c", &character) != EOF) {
            file_length++;
        }
    }

    /* close the file after determining its length */
    fclose(fptr);
    return file_length;
}

char* bufferFileContent(char file_name[]) {
    char *buffer = NULL;

    /* open the file to buffer its content */
    FILE *fptr = fopen(file_name, "r");

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        /* create a buffer with the respective file length */
        int file_length = getFileLength(file_name);
        buffer = (char *) malloc(file_length * sizeof(char));

        /* store the file content (character by character) into buffer */
        char character;
        int i = 0;
        while (fscanf(fptr, "%c", &character) != EOF) {
            buffer[i] = character;
            i++;
        }
    }

    /* close file after storing its content into buffer */
    fclose(fptr);

    return buffer;
}

void deleteFileContent(char file_name[]) {
    /* open the file in write mode */
    FILE *fptr = fopen(file_name, "w");
    /* close it directly after opening it */
    fclose(fptr);
}

void compressText(char file_name[], data_t* linked_list){
    /* open the file in read mode to check if it exists at all */
    FILE *fptr = fopen(file_name, "r");

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        /* close file after checking if it exists */
        fclose(fptr);

        /* determine length of the plain file, buffer and then delete 
         * its content */
        int file_length = getFileLength(file_name);
        char* buffer = bufferFileContent(file_name);
        deleteFileContent(file_name);

        /* open the file in write mode to store compressed text into it */
        fptr = fopen(file_name, "w");

        /* read the text file character by character and transform the 
         * plain text into the compressed text */
        char character;
        int i = 0;
        for (i = 0; i < file_length; i++) {
            character = buffer[i];

            if(character == '\n') {
                /* save the newline (\n) by using 
                 * special character ^ (caret) */
                character = '^';
            } else if (character == ' ') {
                /* save the whitespace by using 
                 * special character * (asterisk) */
                character = '*';
            }

            /* look for the current character in the linked list containing 
             * all the <character, code> pairs */
            data_t* ptr = linked_list;
            while (ptr->character != character) {
                ptr = ptr->next;
            }

            /* write the corresponding code of the character into the 
             * text file */
            fprintf(fptr, "%s", ptr->code);
        } 

    }

    /* close text file after modifying (plain text into code) its content */
    fclose(fptr); 
}

void HuffmanCompression(char file_name[]) {
    /* read the text file and determine the occurrences of each character 
     * existing in the file */
    data_t* linked_list = countOccurrences(file_name);

    /* build a huffman tree given the <character, occurrence> pairs */
    min_heap_node_t* root = buildHuffmanTree(linked_list);

    /* make sure that the file storing the huffman code is empty beforehand */
    deleteFileContent(CODE_FILE);

    /* create variables required by the following function */
    int* code = (int*) malloc(MAX_CODE_SIZE * sizeof(int));
    int current_node = 0;

    /* create the huffman code and store the <character, code> pairs in a 
     * seperate text file */
    createCode(CODE_FILE, root, code, current_node);

    /* AUXILIARY FUNCTION: required to transform the code from integer to 
     * string read the text file containing the <character, code> pairs and  
     * load the pairs into a linked list */
    linked_list = loadCode(CODE_FILE);

    /* given the linked list, compress the text file */
    compressText(file_name, linked_list);
}

void HuffmanDecompression(char file_name[], char code_file[]) {
    /* open the file containing the compressed text in read mode to check if  
     * it exists*/
    FILE* fptr = fopen(file_name, "r");

    if (fptr == NULL) {
        printf("Error reading the file!\n");
    } else {
        /* close file after checking if it exists */
        fclose(fptr);

        /* get the <character, code> pairs from the code file */
        data_t* linked_list = loadCode(code_file);

        /* determine length of the compressed file, buffer and then delete 
         * its content */
        int file_length = getFileLength(file_name);
        char* buffer = bufferFileContent(file_name);
        deleteFileContent(file_name);

        /* open the file in write mode to store compressed text into it */
        fptr = fopen(file_name, "w");

        /* a single character */
        char character;
        /* a string */
        char* code = (char*) malloc(MAX_CODE_SIZE * sizeof(char));
        /* make sure the string is empty */
        code[0] = '\0';

        /* flag to see if the code read from the file matched a code saved in 
         * the <character, code> pairs */
        char match = 0;

        data_t* ptr = NULL;
        int j = 0;

        int i = 0;
        /* read character by character */
        for (i = 0; i < file_length; i++) {
            /* get the character */
            character = buffer[i];

            /* concatenate the characters to a string */
            code[j] = character;
            code[j+1] = '\0';
            
            /* check if the string matches any code from the <character, code> 
             * pairs */
            ptr = linked_list;
            while (ptr != NULL) {
                /* if there is a match, store the corresponding character into 
                 * the new file */
                if (strcmp(ptr->code, code) == 0) {
                    if(ptr->character == '^') {
                        /* save the newline (\n) by using 
                         * special character ^ (caret) */
                        fprintf(fptr, "%c", '\n');
                    } else if (ptr->character == '*') {
                        /* save the whitespace by using 
                         * special character * (asterisk) */
                        fprintf(fptr, "%c", ' ');
                    } else {
                        fprintf(fptr, "%c", ptr->character);
                    }
                    match = 1;
                }
                ptr = ptr->next;
            }

            if (match) {
                /* if there was a match, make sure the code string is empty */
                j = 0;
                code[0] = '\0';
                match = 0;
            } else {
                /* if there was not a match, read the next character from the 
                 * file and append it to the existing string */
                j++;
            }
        }
    }

    /* close file after modifying (code into plain text) its content */
    fclose(fptr);
}
