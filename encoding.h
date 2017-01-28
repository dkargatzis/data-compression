#ifndef _encoding_h
#define _encoding_h

#define NUM_OF_VALUES 20
#define NO_FREQ 0
#define DFLT_FREQ 1
#define EMPTY_HEAP 0
#define EMPTY_TREE 0
#define K_BITS (int)ceil(log2(block_size)+1)
#define EMPTY_CELL -1
#define BIT_0 0
#define BIT_1 1
#define SET 1
#define UNSET 0
#define INS_SMBLS_MODE 0
 

typedef struct tree_node {
    struct tree_node *left_node, *right_node, *parent;
    struct tree_node *next;
    int value;
    int freq;
    int *bin;
    int pointer; 
}*node;


typedef struct heap {
    node head, tail;
    int counter;
}*huff_heap;


typedef struct tree {
    node root;
    int depth;
}*huff_tree;

int **map_table;
int map_t_pnt;
int found;
node found_node;


node init_node(node n);


void print_node(node n);


huff_heap init_heap(huff_heap h_heap);


void print_heap(huff_heap h_heap);



void free_heap(huff_heap h_heap);


void insert_to_heap(node new_node, huff_heap h_heap);

node is_in_heap(node some_node, huff_heap h_heap);

void simple_insert_to_heap(node new_node, huff_heap h_heap);


int search_value_in_heap(huff_heap heap, int value);


void insert_value_to_heap(int value, huff_heap h_heap);

node delete_from_heap(huff_heap h_heap);


huff_tree init_huff_tree(huff_tree h_tree);


huff_tree build_huff_tree(huff_tree h_tree, huff_heap h_heap); 

huff_tree build_wide_huff_tree(huff_tree h_tree, huff_heap h_heap);

void print_huff_tree_preorder(node root);

void print_huff_tree_codes(node root);

void update_huff_tree(node root, huff_tree h_tree, int *bin, int p, int bit);

void search_huff_tree(node current, int value);

int main_program();






#endif
