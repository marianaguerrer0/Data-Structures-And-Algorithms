#include <stdio.h>
#include <stdlib.h>
#define INPUT_FILE_NAME "pa3_data.txt"

// COP 3502C Spring 2025
// Student Name: Mariana Guerrero
// File Name: pa3_guerrero.c
// Note: Rename to use your actual surname. File name should be in lowercase.
// You must remove any whitespaces or dashes from your surname.
// Note: **do not** modify or remove TODO comments


typedef struct BSTNode_s {
    int data; 
    struct BSTNode_s *left;
    struct BSTNode_s *right;
} BSTNode;

typedef struct BSTree_s {
    BSTNode *root;
} BSTree;



// DO NOT MODIFY THIS PORTION
// function prototypes defined here
int sum_at_level(BSTNode *, int);
void insert(BSTree *, int);
BSTNode * create_node(int);
void destroy_node(BSTNode *);
BSTree * create_tree(void);
void destroy_tree(BSTree *);
void postorder_destroy_nodes(BSTNode *);



// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION
int main(void) {
    FILE *input_file = fopen(INPUT_FILE_NAME, "r");

    // check if the file was opened
    if(input_file == NULL) {
        // if there was a problem, simply return a 1
        printf("There was a problem opening the file. Abort!\n");
        return 1;
    }

    // create an empty BST
    BSTree *tree = create_tree();

    // the first line of the input file indicates
    // the level in which we will get the sum of
    int level;
    fscanf(input_file, "%d", &level);

    // read the remaining numbers are to be
    // inserted in the BST
    int num;
    while( fscanf(input_file, "%d", &num) != EOF ) {
        // insert this value into the BST
        insert(tree, num);
    }

    // output the sum at level
    printf("%d\n", sum_at_level(tree->root, level));


    // deallocate
    destroy_tree(tree);

    return 0;
}
// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION



int sum_at_level(BSTNode *root, int level) {
    // TODO START
    // TODO: Complete the recursive function that takes a pointer to the root
    // of a binary search tree (root) and non-negative integer (level).
    // This function returns the sum of all the values in the nodes at
    // that level below the root.

	if (root == NULL){
		return 0;
	}
	
	if(level == 0){
		return root->data; 
	}
	
	int leftSum = sum_at_level(root->left, level-1);
	int rightSum = sum_at_level(root->right, level-1);
	return leftSum + rightSum;

  
    // TODO END
}


void insert(BSTree *tree, int val) {
    // check if the tree is empty, set root
    if(tree->root == NULL) {
        tree->root = create_node(val);
        return;
    }

    BSTNode *ptr = tree->root;
    BSTNode *parent = NULL; // trailing pointer

    while(ptr != NULL) {
        parent = ptr;

        if(val < ptr->data)
            ptr = ptr->left;
        else
            ptr = ptr->right;
    }

    if(val < parent->data)
        parent->left = create_node(val);
    else
        parent->right = create_node(val);
}


BSTNode * create_node(int val) {
    BSTNode *node = malloc(sizeof(BSTNode));

    // initialize the appropriate attributes
    node->data = val;
    node->left = NULL;
    node->right = NULL;

    return node;
}


void destroy_node(BSTNode *node) {
    free(node);
}


BSTree * create_tree(void) {
    BSTree *tree = malloc(sizeof(BSTree));

    tree->root = NULL;

    return tree;
}


void destroy_tree(BSTree *tree) {
    // deallocate all the nodes (postorder)
    postorder_destroy_nodes(tree->root);

    // deallocate the tree
    free(tree);
}


void postorder_destroy_nodes(BSTNode *node) {
    // this is the base case
    if(node == NULL)
        return;

    // left
    postorder_destroy_nodes(node->left);

    // right
    postorder_destroy_nodes(node->right);

    // visit the current node
    destroy_node(node);
}
