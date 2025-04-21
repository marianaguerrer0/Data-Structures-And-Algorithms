#include <stdio.h>
#include <stdlib.h>
#define INPUT_FILE_NAME "pa6_data.txt"

// COP 3502C Spring 2025
// Student Name: Mariana Guerrero
// File Name: pa6_yoursurname.c
// Note: Rename to use your actual surname. File name should be in lowercase.
// You must remove any whitespaces or dashes from your surname.
// Note: **do not** modify or remove TODO comments


typedef struct Node_s {
    int x;
    int y;
    struct Node_s *next;
} Node;



// DO NOT MODIFY THIS PORTION
// function prototypes defined here
Node * combine(Node *, Node *);
Node * create_node(int, int);
void destroy_nodes(Node *);
void print_list(Node *);
void add_to_tail(Node *, Node *);



// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION
int main(void) {
    FILE *input_file = fopen(INPUT_FILE_NAME, "r");

    // check if the file was opened
    if(input_file == NULL) {
        // if there was a problem, simply return a 1
        printf("There was a problem opening the file. Abort!\n");
        return 1;
    }

    // read the size of the first list
    int N;
    fscanf(input_file, "%d", &N);

    // temporary variables
    int x, y;

    // pointer to the head of the first linked list
    Node *ptr1 = NULL;

    // process the first list with N elements
    for(int i = 0; i < N; i++) {
        // read two numbers
        fscanf(input_file, "%d%d", &x, &y);

        // if this list is currently empty, create
        // the head of the linked list; otherwise
        // we add to tail the new node
        if(ptr1 == NULL)
            ptr1 = create_node(x, y);
        else
            add_to_tail(ptr1, create_node(x, y));
    }


    // read the size of the second list
    int M;
    fscanf(input_file, "%d", &M);

    // pointer to the head of the second linked list
    Node *ptr2 = NULL;

    // process the second list with M elements
    for(int i = 0; i < M; i++) {
        // read two numbers
        fscanf(input_file, "%d%d", &x, &y);

        // if this list is currently empty, create
        // the head of the linked list; otherwise
        // we add to tail the new node
        if(ptr2 == NULL)
            ptr2 = create_node(x, y);
        else
            add_to_tail(ptr2, create_node(x, y));
    }

    // close the file
    fclose(input_file);


    // combine the two sorted linked lists
    // into a single list
    Node *combined_list = combine(ptr1, ptr2);

    // print the contents of the linked list
    print_list(combined_list);


    // destroy the combined linked list
    destroy_nodes(combined_list);


    return 0;
}
// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION



Node * combine(Node *ptr1, Node *ptr2) {
    // TODO START
    // TODO: Given two sorted singly linked lists,
    // combine them into one sorted linked list.
    // Each node contains information about a
    // 2D coordinate (x, y). The lists are sorted
    // by the x-coordinate (increasing order) first
    // and then by the y-coordinate (increasing order).
    // You will not receive credit if your code
    // involves allocating OR deallocating any
    // memory. In short, there should be no malloc()
    // or free() call in this function. Instead, your
    // code should simply change where some of the
    // existing pointers are pointing. This function
    // should return a pointer to the front (head)
    // of this updated list.

	if (ptr1 == NULL){ //if first list is empty, return the second list.
		return ptr2;
	}
	if(ptr2 == NULL){ //if second list is empty, return the first list.
		return ptr1;
	}
	
	Node *head = NULL; //pointer to head of combined list.
	Node *tail = NULL; //pointer to tail of combined list.
	
	//determine head of the combined list
	if(ptr1->x < ptr2->x || (ptr1->x == ptr2->x && ptr1->y < ptr2->y)){
		head = ptr1;
		ptr1 = ptr1->next;
	}else{
		head = ptr2;
		ptr2 = ptr2->next;
	}
	
	tail = head;
	
	//iterate through both lists while comparing nodes.
	while (ptr1 != NULL && ptr2 != NULL){
		if(ptr1 -> x < ptr2->x || (ptr1->x == ptr2->x && ptr1->y < ptr2->y)){
			tail->next = ptr1;
			ptr1 = ptr1->next;
		} else {
			tail->next = ptr2;
			ptr2 = ptr2->next;
		}
		tail = tail->next;
	}
	//add any remaining nodes from either list
	if (ptr1 != NULL){
		tail->next = ptr1;
	} else if (ptr2 != NULL){
		tail->next = ptr2;
	}
	
	return head; //return the head of the combined linked list.

    // TODO END
}


Node * create_node(int x, int y) {
    // dynamically create node
    Node *node = malloc(sizeof(Node));

    // set the attributes of this node
    node->x = x;
    node->y = y;
    node->next = NULL;

    return node;
}


void destroy_nodes(Node *node) {
    // given the head of a linked list,
    // this function deallocates all the
    // nodes in the linked list

    // this is a trailing pointer
    Node *tmp = NULL;

    while(node != NULL) {
        // update the trailing pointer
        tmp = node;

        // move to the next node
        node = node->next;

        // deallocate the previous
        // node that is pointed by
        // the trailing pointer
        free(tmp);
    }
}


void print_list(Node *node) {
    // this is a helper function that simply
    // prints all the values of the nodes
    // of a linked list; it requires a pointer
    // to the head of the linked list
    Node *ptr = node;

    while(ptr != NULL) {
        // print the current node
        printf("(%d, %d)\n", ptr->x, ptr->y);

        // go to the next node
        ptr = ptr->next;
    }
}


void add_to_tail(Node *ptr, Node *node) {
    // this function adds a new node to the
    // tail of a linked list; it requires
    // a pointer to the head of a linked
    // list and the node that needs to be
    // appended or added to the tail

    // base case where the linked list
    // is empty; but this should not be
    // executed here
    if(ptr == NULL)
        return;

    // base case where we are currently
    // pointing at the tail of the linked
    // list
    if(ptr->next == NULL) {
        ptr->next = node;
        return;
    }

    // go to the next node so that we will
    // eventually reach the end of the
    // the linked list
    add_to_tail(ptr->next, node);
}
