#ifndef HUFFMAN_H
#define HUFFMAN_H


/***********************************************************************************
 * LIBRARIES 
 **********************************************************************************/
#include <stdlib.h>


/***********************************************************************************
 * HASH DEFINES 
 **********************************************************************************/
#define PLAIN_TEXT		"input.txt"
#define COMPR_TEXT		"compressed.txt"
#define DECOMPR_TEXT	"decompressed.txt"
#define CODE_FILE       "code.txt"


/***********************************************************************************
 * STRUCTURES 
 **********************************************************************************/
/* a node of the huffman tree */
struct min_heap_node {
    char data; 
    int frequency; /* occurance of the data in the input text */
    struct min_heap_node *left;     /* left child node */
    struct min_heap_node *right;    /* right child node */
};
typedef struct min_heap_node min_heap_node_t;

/* huffman tree */
struct min_heap {
    int size;       /* current size */
    int capacity;   /* maximum size */
    min_heap_node_t** array; /* array of min_heap_node pointers */
};
typedef struct min_heap min_heap_t;

/* a line read from the input textfile */
/*struct input_string {
    char* str;
    int str_length;
};
typedef struct input_string input_string_t;*/

/* a datapoint */
struct data {
    char character;
    int occurrence;
    int* code;
    struct data* next;
};
typedef struct data data_t;


/***********************************************************************************
 * FUNCTION PROTOTYPES 
 **********************************************************************************/
/* create a node for a given character */
min_heap_node_t* createNode (char data, int frequency);

/* create a min heap */
min_heap_t* createMinHeap (int capacity);

/* swap two nodes */
void swapNodes (min_heap_node_t** node_a, min_heap_node_t** node_b);

/* apply min heapify on a subtree starting from a given index */
void minHeapify (min_heap_t* min_heap, int index);

/* extract node with lowest freq from heap */
min_heap_node_t* extractMin (min_heap_t* min_heap);

/* insert a new node to the min heap tree */
void insertNode (min_heap_t* min_heap, min_heap_node_t* node);

/* build a min heap tree */
void buildMinHeap (min_heap_t* min_heap);

/* create a min heap tree of capacity equal to size and insert all characters 
 * of the linked list in the tree */
min_heap_t* createAndBuildMinHeap (data_t* linked_list);

/* build a huffman tree */
min_heap_node_t* buildHuffmanTree (data_t* linked_list);

/* assign the huffman code to the characters */
void createCode (data_t* linked_list, min_heap_node_t* node, 
	int* code, int current_node);

/* apply the huffman coding on data */
void HuffmanCompression(data_t* linked_list);

/* count occurrences of all characters existent in the file */
/*data_t* countOccurrences(char file_name[]);*/

/* save the linked list which contains all characters and their frequencies */
/*void saveLinkedList(data_t* linked_list);*/

/* load the linked list which contains all characters and their frequencies */
data_t* loadLinkedList(char file_name[]);

/* determine the size of a linked list */
int getSize(data_t* linked_list);


#endif /* HUFFMAN_H */

