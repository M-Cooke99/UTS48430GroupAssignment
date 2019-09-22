#include <stdio.h>
#include "huffman.h"


/***********************************************************************************
 * STRUCTURES 
 **********************************************************************************/
/* a node of the huffman tree*/
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
 * of the array data[] in the tree */
min_heap_t* createAndBuildMinHeap (char data[], int frequency[], int size);

/* build a huffman tree */
min_heap_node_t* buildHuffmanTree (char data[], int frequency[], int size);

/* assign the huffman code to the characters */
void createCode (min_heap_node_t* node, int array[], int current_node);

/* apply the huffman coding on data */
void HuffmanCompression(char data[], int freq[], int size);


/***********************************************************************************
 * MAIN 
 **********************************************************************************/
int main (void) {
    
    char array[] = { 'a', 'b', 'c', 'd', 'e', 'f' }; 
    int frequency[] = { 5, 9, 12, 13, 16, 45 }; 
  
    int size = sizeof(array) / sizeof(array[0]); 
  
    HuffmanCompression(array, frequency, size); 

    return 0;
}


/***********************************************************************************
 * FUNCTION DEFINITION 
 **********************************************************************************/
min_heap_node_t* createNode (char data, int frequency) {
    /* create a new node */
    min_heap_node_t* node = (min_heap_node_t*) malloc(sizeof(min_heap_node_t));
    
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
    /* decrease the size of the array since a node with lowest freq was extracted */
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
    while (i > 0 && node->frequency < min_heap->array[(i - 1) / 2]->frequency) {
        /* node in the middle of the array is replaced to the end of the array */
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

min_heap_t* createAndBuildMinHeap (char data[], int frequency[], int size) {
    /* create the min heap */
    min_heap_t* min_heap = createMinHeap(size);
    /* set size of the min heap */
    (*min_heap).size = size;

    /* create a node for each character in the array data[] */
    int i;
    for (i = 0; i < size; i++)
        (*min_heap).array[i] = createNode(data[i], frequency[i]);

    /* build the min heap */
    buildMinHeap(min_heap);

    return min_heap;
}

min_heap_node_t* buildHuffmanTree (char data[], int frequency[], int size) {
    /* define node and children */
    min_heap_node_t *left, *right, *top;

    /* create and build a min heap tree */
    min_heap_t* min_heap = createAndBuildMinHeap(data, frequency, size);

    while (min_heap->size != 1) {
        /* node with lowest freq is left child */
        left = extractMin(min_heap);
        /* node with next higher freq is right child */
        right = extractMin(min_heap);

        /* create an internal node with frequency equal to the sum of
         * the frequencies of the left and right child, furthermore
         * make it parent of left and right node */
        top = createNode('$', (*left).frequency + (*right).frequency);
        top->left = left;
        top->right = right;

        /* insert the new internal node into the min heap tree */
        insertNode(min_heap, top);
    }

    /* remaining node is the root of the huffman tree */
    return extractMin(min_heap);
}

void createCode (min_heap_node_t* node, int array[], int current_node) {
    if (node->left) {
        /* assign 0 to the left node */
        array[current_node] = 0;
        createCode(node->left, array, (current_node+1));
    }

    if (node->right) {
        /* assign 1 to the right node */
        array[current_node] = 1;
        createCode(node->right, array, (current_node+1));
    }    

    if (node->left == NULL && node->right == NULL) {
        printf("%c: ", node->data); 
        int i;
        for(i = 0; i < current_node; i++) {
            printf("%d", array[i]);
        }
        printf("\n");
    }
}

void HuffmanCompression(char data[], int freq[], int size) {
    min_heap_node_t* root = buildHuffmanTree(data, freq, size);

    int array[100];
    int current_node = 0;

    createCode(root, array, current_node);
}




