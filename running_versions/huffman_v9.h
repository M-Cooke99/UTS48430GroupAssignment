/* 
 * author: Oezguen Turgut 
 * date: 24/09/2019
 * description: header file of huffman.c
 */


#ifndef HUFFMAN_H
#define HUFFMAN_H


/***********************************************************************************
 * LIBRARIES 
 **********************************************************************************/
#include <stdlib.h> /* malloc */
#include <string.h> /* strlen */


/***********************************************************************************
 * HASH DEFINES 
 **********************************************************************************/
#define PLAIN_TEXT		"input.txt"
#define CODE_FILE		"code.txt"
#define COMPR_TEXT      "compressed.txt"
#define DECOMPR_TEXT	"decompressed.txt"

#define MAX_CODE_SIZE	32


/***********************************************************************************
 * STRUCTURES 
 **********************************************************************************/
/* a node of the huffman tree */
struct min_heap_node {
    char character; 
    int occurrence;                /* occurrence of the character in the text file */
    struct min_heap_node *left;    /* left child node */
    struct min_heap_node *right;   /* right child node */
};
typedef struct min_heap_node min_heap_node_t;

/* huffman tree */
struct min_heap {
    int size;                   /* current size */
    int capacity;               /* maximum size */
    min_heap_node_t** array;    /* array of min_heap_node pointers */
};
typedef struct min_heap min_heap_t;

/* a datapoint */
struct data {
    char character;
    int occurrence;             /* occurrence of the character in the text file */
    char code[MAX_CODE_SIZE];   /* huffman code corresponding to the character */
    struct data* next;
};
typedef struct data data_t;


/***********************************************************************************
 * FUNCTION PROTOTYPES 
 **********************************************************************************/
/* create a node for a given character */
min_heap_node_t* createNode (char character, int occurrence);

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
void createCode (char file_name[], min_heap_node_t* node, int* code, int current_node);

/* counts occurrences of each character appearing in a given text file */
data_t* countOccurrences(char file_name[]);

/* loads the <character : code> pair */
data_t* loadCode(char file_name[]);

/* determines the size of a linked list */
int getSize(data_t* linked_list);

/* compresses the plain text */
void compressText(char file_name[], data_t* linked_list);

/* THIS IS WHERE THE ACTION IS: apply the huffman coding on the text file */
void HuffmanCompression(char file_name[]);

/* decompress the text file given the <character, code> pairs stored in 
 * the code file*/
void HuffmanDecompression(char compressed_file[], char code_file[]);


#endif /* HUFFMAN_H */

