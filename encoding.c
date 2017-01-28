/* 
 * File:   encoding.c
 * Author: john
 *
 * Created on February 7, 2014, 9:44 PM
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "encoding.h"
#include "huffman_encoding.h"




node init_node(node n)
{
    //printf("Initializing node...\n");
    n = (node)malloc(sizeof(struct tree_node));
    n->parent = NULL;
    n->right_node = NULL;
    n->left_node = NULL;
    n->next = NULL;
    n->freq = NO_FREQ;
    n->value = 0;
    n->bin = (int *)malloc(K_BITS * sizeof(int));
    int i;
    for (i=0; i < K_BITS; i++) {
        n->bin[i] = EMPTY_CELL;
    }
    return n;
}

void print_node(node n)
{
    printf("Node:\n");
    printf("------\n\n");
    printf("\tValue: %d\n", n->value);
    printf("\tFrequency: %d\n", n->freq);
    printf("\tPointer: %d\n", n->pointer);
    if ((n->left_node == NULL) && (n->right_node == NULL)) {
        printf("\tNode is LEAF\n");
    }
    printf("\tNode map table: \n");
    int i = 0;
    while ((i<K_BITS) && (n->bin[i] != EMPTY_CELL)) {
        printf("\t\t-> %d\n", n->bin[i]);
        i++;
    }
    printf("\n\n");
}

huff_heap init_heap(huff_heap h_heap)
{
    //printf("Initializing heap...\n");
    h_heap = (huff_heap)malloc(sizeof(struct heap));
    h_heap->head = NULL;
    h_heap->tail = NULL;
    h_heap->counter = EMPTY_HEAP;
    return h_heap;
}

void print_heap(huff_heap h_heap)
{
    printf("\nPrinting the heap:\n");
    printf("-------------------\n\n");
    printf("Size of the heap: %d\n", h_heap->counter);
    //printf("Tail of the heap:\n");
    //print_node(h_heap->tail);
    node current;
    current = init_node(current);
    current = h_heap->head;
    //print_node(current);
    while (current != NULL){
        print_node(current);
        current = current->next;
    }
    printf("Print_heap-> Free current node.\n");
    free(current);
    printf("End of heap printing\n");
}

void free_heap(huff_heap h_heap)
{
    //printf("\nFree heap:\n");
    //printf("--------------------------------------\n");
    node current, tmp;
    current = init_node(current);
    tmp = init_node(tmp);
    current = h_heap->head;
    while (current != NULL) {
        tmp = current;
        current = current->next;
        //printf("Free node - Heap size: %d\n", h_heap->counter);
        h_heap->counter--;
        free(tmp);
    }
    //printf("Free node - Heap size: %d\n", h_heap->counter);
    free(current);
    //printf("Free heap structure\n");
    free(h_heap);
    //printf("--------------------------------------\n");
}

void insert_to_heap(node new_node, huff_heap h_heap)
{
    //printf("Inserting new node to heap...\n");
    node current, tmp_node;
    //printf("Current->");
    current = init_node(current);
    //printf("Tmp_node->");
    tmp_node = init_node(tmp_node);
    if (h_heap->head == NULL) {
        //printf("Case that heap is empty\n");
        //Heap is empty
        h_heap->head = new_node;
        h_heap->head->next = NULL;
        //h_heap->counter++;
        //printf("-------New node inserted\n");
    }
    else {
        if ((new_node->freq < h_heap->head->freq) || (new_node->freq == 
                                                            h_heap->head->freq)) {
            new_node->next = h_heap->head;
            h_heap->head = new_node;
            //h_heap->counter++;
            //printf("--------New node inserted in the head of the heap\n");
        }
        else {
            current = h_heap->head;
            while ((current != NULL) && ((new_node->freq) > (current->freq) )) {
                //printf("While loop\n");
                tmp_node = current;
                current = current->next;
                tmp_node->next = current;
            }
            //inserting the new node;
            tmp_node->next = new_node;
            new_node->next = current;
            //if (new_node->next = NULL) {
            //    h_heap->tail = new_node;
            //}
            //h_heap->counter++;
            //printf("-------New node inserted\n");
        }
    }
}

node is_in_heap(node some_node, huff_heap h_heap)
{
    node current;
    node tmp;
    tmp = init_node(tmp);
    current = init_node(current);
    if (h_heap->head == NULL) {
        //printf("Is_in_heap: Heap is empty. Return null.\n");
        return NULL;
    }
    else {
        if (h_heap->head->value == some_node->value) {
            some_node->freq = some_node->freq + h_heap->head->freq;
            h_heap->head = h_heap->head->next;
            h_heap->counter--;
            return some_node;
        }
        current = h_heap->head;
        tmp = h_heap->head;
        while ((current != NULL) && (current->value != some_node->value)) {
            tmp = current;
            current = current->next;
        }
        if (current == NULL) {
            //printf("Is_in_heap: Input node does not exist in heap\n");
            return NULL;
        }
        else {
            //printf("Is_in_heap: Input node found in the list.\n");
            tmp->next = current->next;
            current->next = NULL;
            h_heap->counter--;
            some_node->freq = some_node->freq + current->freq;
            free(current);
            return some_node;
        }
    }
}

void simple_insert_to_heap(node new_node, huff_heap h_heap)
{
    if (h_heap->head == NULL) {
        //printf("head\n");
        h_heap->head = new_node;
        h_heap->head->next = NULL;
        h_heap->counter++;
    }
    else {
        if (abs(new_node->value) <= abs(h_heap->head->value)) {
            new_node->next = h_heap->head;
            h_heap->head = new_node;
            //printf("Before heap\n");
            h_heap->counter++;
            //h_heap->counter++;
            //printf("--------New node inserted in the head of the heap\n");
        }
        else {
            //printf("Not head\n");
            node current, previous;
            current = init_node(current);
            previous = init_node(previous);
            current = h_heap->head;
            //previous = h_heap->head;
            while ((current != NULL) && (abs(current->value) < abs(new_node->value))) {
                previous = current;
                current = current->next;
                previous->next = current;
            }
            if (current == NULL) {
                previous->next = new_node;
                new_node->next = NULL;
            }
            else {
                previous->next = new_node;
                new_node->next = current;
            }
            h_heap->counter++;
        }
    }
}

int search_value_in_heap(huff_heap heap, int value){
    node temp;
    temp = init_node(temp);
    temp = heap->head;
    while(temp != NULL){
        if(temp->value == value)
            return temp->bin;

        temp = temp->next;
    }
} 

void insert_value_to_heap(int value, huff_heap h_heap)
{
    node tmp_node;
    node tmp;
    tmp = init_node(tmp);
    tmp_node = init_node(tmp_node);
    tmp_node->value = value;
    tmp_node->freq = DFLT_FREQ;

    tmp = is_in_heap(tmp_node, h_heap);

    if (tmp == NULL) {
        insert_to_heap(tmp_node, h_heap);
        //printf("\nInsert_value_to_heap: Node inserted to heap!\n");
        h_heap->counter++;
    }
    else {
        insert_to_heap(tmp, h_heap);
        //printf("\nInsert_value_to_heap: Node in heap updated!\n");
        h_heap->counter++;
    }

}

node delete_from_heap(huff_heap h_heap)
{
    node tmp;
    //printf("delete_from_heap tmp-> ");
    tmp = init_node(tmp);
    tmp = h_heap->head;
    h_heap->head = h_heap->head->next;
    //tmp->next = NULL;
    h_heap->counter--;
    return tmp;
}

huff_tree init_huff_tree(huff_tree h_tree) {
    h_tree = (huff_tree)malloc(sizeof(struct tree));
    h_tree->root = NULL;
    h_tree->depth = EMPTY_TREE;
    return h_tree;
}

huff_tree build_huff_tree(huff_tree h_tree, huff_heap h_heap) {
    if (h_tree->root == NULL) {
        //printf("Build_huff_tree root-> ");
        h_tree->root = init_node(h_tree->root);
        node left, right;
        if (h_heap->counter < 2) {
            return h_tree;
        }
        left = delete_from_heap(h_heap);
        right = delete_from_heap(h_heap);
        h_tree->root->left_node = left;
        h_tree->root->right_node = right;
        h_tree->root->freq = left->freq + right->freq;
        h_tree->depth++;
        printf("\n-----------------> Depth: %d\n\n", h_tree->depth);
        build_huff_tree(h_tree, h_heap);
    }
    else {
        node left1;
        if (h_heap->counter < 1) {
            //printf("Heap is empty\n");
            return h_tree;
        }
        left1 = delete_from_heap(h_heap);
        
        node new_root;
        //printf("build_huff_tree new_root-> ");
        new_root = init_node(new_root);
        new_root->right_node = h_tree->root;
        new_root->left_node = left1;
        new_root->freq = h_tree->root->freq + left1->freq;
        h_tree->root = new_root;
        h_tree->depth++;
        //printf("\n-----------------> Depth: %d\n\n", h_tree->depth);
        build_huff_tree(h_tree, h_heap);
    }
}

huff_tree build_wide_huff_tree(huff_tree h_tree, huff_heap h_heap)
{
    if (h_tree->root == NULL) {
        //printf("Build_huff_tree root-> ");
        h_tree->root = init_node(h_tree->root);
        node left, right;
        if (h_heap->counter < 2) {
            return h_tree;
        }
        left = delete_from_heap(h_heap);
        right = delete_from_heap(h_heap);
        h_tree->root->left_node = left;
        h_tree->root->right_node = right;
        h_tree->root->freq = left->freq + right->freq;
        h_tree->depth++;
        //printf("\n-----------------> Depth: %d\n\n", h_tree->depth);
        build_wide_huff_tree(h_tree, h_heap);
    }
    else {
	if (h_heap->counter < 1) {
            //printf("Heap is empty\n");
            return h_tree;
        }
        node tmp_1;
        tmp_1 = delete_from_heap(h_heap);
        //printf("\nTree root->");
        //print_node(h_tree->root);
        //printf("\nHeap head->");
        //print_node(h_heap->head);
        if ((h_heap->head != NULL) && (h_heap->head->freq <= h_tree->root->freq)){
            if (h_heap->counter < 1) {
                //printf("Heap is empty\n");
                return h_tree;
            }
            node tmp_2;
            tmp_2 = delete_from_heap(h_heap);
            node sub_root;
            sub_root = init_node(sub_root);
            sub_root->freq = tmp_1->freq + tmp_2->freq;
            sub_root->left_node = tmp_1;
            sub_root->right_node = tmp_2;

            node new_root;
            new_root = init_node(new_root);
            new_root->left_node = h_tree->root;
            new_root->right_node = sub_root;

            
            h_tree->root = new_root;
            //printf("\nEdw tha ftiaksoume sub tree. Pisteoume.\n");
            //printf("\n-----------------> Depth: %d\n\n", h_tree->depth);
            build_wide_huff_tree(h_tree, h_heap);
        }
        else {
            
            node new_root;
            new_root = init_node(new_root);
            new_root->left_node = h_tree->root;
            new_root->right_node = tmp_1;
            new_root->freq = new_root->left_node->freq + new_root->right_node->freq;

            h_tree->depth++;
            h_tree->root = new_root;
            //printf("\n-----------------> Depth: %d\n\n", h_tree->depth);
            if (h_heap->counter < 1) {
                //printf("Heap is empty\n");
                return h_tree;
            }
            build_wide_huff_tree(h_tree, h_heap);
        }
    }
}


void print_huff_tree_preorder(node root)
{
    //printf("\nPrinting the huffman tree:\n");
    //printf("-----------------------------\n\n");
    //print_node(root);
    if ((root->left_node == NULL) && (root->right_node == NULL)) {
        print_node(root);
        return;
    }
    else {
        if ((root->left_node != NULL) && (root->right_node == NULL)) {
            print_huff_tree_preorder(root->left_node);
            print_node(root);
            //print_node(root->right_node);
            return;
        }
        else if ((root->left_node == NULL) && (root->right_node != NULL)) {
            //print_node(root->right_node);
            print_node(root);
            print_huff_tree_preorder(root->right_node);
            return;
        }
        else {
            //print_node(root);
            print_huff_tree_preorder(root->left_node);
            print_node(root);
            print_huff_tree_preorder(root->right_node);
            return;
        }
    }
    //printf("End of huffman tree printing\n");
    //printf("-----------------------------\n\n");
}


void print_huff_tree_codes(node root)
{
    //printf("\nPrinting the huffman tree:\n");
    //printf("-----------------------------\n\n");
    //print_node(root);
    if ((root->left_node == NULL) && (root->right_node == NULL)) {
        //print_node(root);
        printf("\t%d\t", root->value);
        int y = 0;
        while(root->bin[y] != -1) {
            printf("%d", root->bin[y]);
            y++;
        }
        printf("\n");
        return;
    }
    else {
        if ((root->left_node != NULL) && (root->right_node == NULL)) {
            print_huff_tree_codes(root->left_node);
            //print_node(root);
            //print_node(root->right_node);
            return;
        }
        else if ((root->left_node == NULL) && (root->right_node != NULL)) {
            //print_node(root->right_node);
            //print_node(root);
            print_huff_tree_codes(root->right_node);
            return;
        }
        else {
            //print_node(root);
            print_huff_tree_codes(root->left_node);
            //print_node(root);
            print_huff_tree_codes(root->right_node);
            return;
        }
    }
    //printf("End of huffman tree printing\n");
    //printf("-----------------------------\n\n");
}


void update_huff_tree(node root, huff_tree h_tree, int *bin, int p, int bit)
{
    if (root == h_tree->root) {
        //printf("CASE 1\n");
        
        update_huff_tree(root->left_node, h_tree, root->bin, 
                                                    root->pointer, BIT_0);
        
        update_huff_tree(root->right_node, h_tree, root->bin, 
                                                    root->pointer, BIT_1);
    }
    else {
        if ((root->left_node == NULL) && (root->right_node == NULL)) {
            //if (root->flag == UNSET) {
                int i = 0;
                while ((i < K_BITS) && (bin[i] != EMPTY_CELL)) {
                    root->bin[i] = bin[i];
                    i++;
                }
                root->bin[p] = bit;
                root->pointer = p+1;
                //print_node(root);
                //printf("CASE 2\n");
            //}
            
        }
        else {
            int i = 0;
            while ((i < K_BITS) && (bin[i] != EMPTY_CELL)) {
                root->bin[i] = bin[i];
                i++;
            }
            root->bin[p] = bit;
            root->pointer = p+1;
            //print_node(root);
            //printf("CASE 5\n");
                
            if (root->left_node != NULL) {      
                update_huff_tree(root->left_node, h_tree, root->bin, 
                                                        root->pointer, BIT_0);
            }
            if (root->right_node != NULL) {
                update_huff_tree(root->right_node, h_tree, root->bin, 
                                                       root->pointer, BIT_1);
            }
        }
    }
}


void search_huff_tree(node current, int value)
{
    if ((current->left_node == NULL) && (current->right_node == NULL)) {
        //printf("Leaf\n");
        if (current->value == value) {
            //printf("Found\n");
            found = 1;
            found_node = current;
        }
    }
    else {
        if (current->left_node != NULL) {
            //printf("left\n");
            search_huff_tree(current->left_node, value);
        }
        if (found == 1){
            return;
        }
        if (current->right_node !=NULL) {
            //printf("right\n");
            search_huff_tree(current->right_node, value);
        }
    }
}


void free_huff_tree(node root)
{
    if ((root->left_node == NULL) && (root->right_node == NULL)) {
        free(root);
        //printf("Node is free.\n");
        return;
    }
    else {
        if ((root->left_node != NULL) && (root->right_node == NULL)) {
            free_huff_tree(root->left_node);
            free(root);
        }
        else if ((root->left_node == NULL) && (root->right_node != NULL)) {
            node tmp;
            tmp = init_node(tmp);
            tmp = root;
            free(root);
            free_huff_tree(root->right_node);
        }
        /*else {
            free_huff_tree(root->left_node);
            node tmp;
            tmp = init_node(tmp);
            tmp = root;
            free(root);
            free_huff_tree(root->right_node);
        }*/
    }
    //printf("\nFree_huff_tree has finished successfully\n");
}

void build_map_table(int elems, int mode, huff_heap h_heap) 
{
    if (mode == INS_SMBLS_MODE) {
        node current;
        current = init_node(current);
        current = h_heap->head;
        int i = 0;
        while (current != NULL){ 
            map_table[i][0] = current->value;
            current = current->next;
        }
    }
}

int main_program()
{
    
    
    return 0;
}
