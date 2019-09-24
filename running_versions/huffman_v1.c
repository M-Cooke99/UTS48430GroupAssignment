/* 
 * author: Oezguen Turgut 
 * date: 23/09/2019
 * description: read a textfile, compress it, and decompress it again
 */

#include <stdio.h>
#include "huffman.h"


/***********************************************************************************
 * MAIN 
 **********************************************************************************/
int main (int argc, char* argv[]) {

	data_t* linked_list = loadLinkedList(CODE_FILE);
    HuffmanCompression(linked_list); 

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

min_heap_t* createAndBuildMinHeap (data_t* linked_list) {
	int size = getSize(linked_list);

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
        top = createNode('$', (*left).frequency + (*right).frequency);
        top->left = left;
        top->right = right;

        /* insert the new internal node into the min heap tree */
        insertNode(min_heap, top);
    }

    /* remaining node is the root of the huffman tree */
    return extractMin(min_heap);
}

void createCode (data_t* linked_list, min_heap_node_t* node, 
	int* code, int current_node) {
    if (node->left) {
        /* assign 0 to the left node */
        code[current_node] = 0;
        createCode(linked_list, node->left, code, (current_node+1));
    }

    if (node->right) {
        /* assign 1 to the right node */
        code[current_node] = 1;
        createCode(linked_list, node->right, code, (current_node+1));
    }    

    if (node->left == NULL && node->right == NULL) {
    	data_t* ptr = linked_list;
    	while (ptr->character != node->data) {
    		ptr = ptr->next;
    	}

    	ptr->code = code;

        printf("%c: ", ptr->character); 
        int i;
        for(i = 0; i < current_node; i++) {
            printf("%d", ptr->code[i]);
        }
        printf("\n");
    }
}

void HuffmanCompression(data_t* linked_list) {
    min_heap_node_t* root = buildHuffmanTree(linked_list);

    int* code = (int*) malloc(CODE_SIZE * sizeof(int));
    int current_node = 0;

    createCode(linked_list, root, code, current_node);
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

        while (fscanf(fptr, "%c %d ", &character, &occurrence) != EOF) {
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


