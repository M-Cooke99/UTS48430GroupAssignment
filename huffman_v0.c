#include <stdio.h>
#include "huffman.h"


/***********************************************************************************
 * STRUCTURES 
 **********************************************************************************/
/* a node of the huffman tree*/
struct huff_node {
    char data; 
    int frequency; /* occurance of the data in the input text */
    struct huff_node *left;     /* left child node */
    struct huff_node *right;    /* right child node */
};
typedef struct huff_node huff_node_t;

/* huffman tree */
struct min_heap {
    int size;       /* current size */
    int capacity;   /* maximum size */
    huff_node_t** array; /* array of huff_node pointers */
};
typedef struct min_heap min_heap_t;


/***********************************************************************************
 * FUNCTION PROTOTYPES 
 **********************************************************************************/
/* create a node for a given character */
huff_node_t* createNode (char data, int frequency);

/* create a huffman tree (which is a min heap) */
min_heap_t* createMinHeap (int capacity);

/* swap two nodes */
void swapNodes (huff_node_t* node_a, huff_node_t* node_b);

/* apply min heapify on a subtree starting from a given index */
void minHeapify (min_heap_t* min_heap, int index);

/* extract node with lowest freq from heap */
huff_node_t* extractMin (min_heap_t* min_heap);

/***********************************************************************************
 * MAIN 
 **********************************************************************************/
int main (void) {
    puts("hi");
    return 0;
}


/***********************************************************************************
 * FUNCTION DEFINITION 
 **********************************************************************************/
huff_node_t* createNode (char data, int frequency) {
    /* create a new node */
    huff_node_t* node = (huff_node_t*) malloc(sizeof(huff_node_t));
    
    /* set all attributes */
    (*node).data = data;
    (*node).frequency = frequency;
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
    (*min_heap).array = (huff_node_t**) malloc(((*min_heap).capacity) * 
        sizeof(huff_node_t*));

    return min_heap;
}

void swapNodes (huff_node_t* node_a, huff_node_t* node_b) {
    huff_node_t* temp = node_a;
    node_a = node_b;
    node_b = temp;
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
        if (min_heap->array[left]->frequency < min_heap->array[smallest]->frequency) 
            smallest = left;
    }

    /* check if there is a right child at all */
    if (right < min_heap->size) {
        /* check if freq of right child is lower than freq of current node */
        if (min_heap->array[right]->frequency < min_heap->array[smallest]->frequency) 
           smallest = right;
    }

    /* check if there was a node with a lower freq */
    if(smallest != index) {
        swapNodes((*min_heap).array[smallest], (*min_heap).array[index]); 
        /* apply min heapify on subtree of the swapped node */
        minHeapify(min_heap, smallest);
    }
}

huff_node_t* extractMin (min_heap_t* min_heap) {
    /* node with lowest freq is ALWAYS at index 0 (given a min heap tree) */
    huff_node_t* min_huff_node = min_heap->array[0];

    /* get the last element of the array and put it at index 0 
     * now the min heap tree is not sorted correctly anymore    */
    min_heap->array[0] = min_heap->array[(min_heap->size) - 1];
    /* decrease the size of the array since a node with lowest freq was extracted */
    (min_heap->size)--;
    /* rearrange the min heap tree starting from the root */
    minHeapify(min_heap, 0);

    return min_huff_node;
}





