#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define TRUE 1
#define FALSE 0
#define MAX_NAME_LENGTH 255
#define INPUT_FILE_NAME "pa2_data.txt"

// COP 3502C Spring 2025
// Student Name: Mariana Guerrero
// File Name: pa2_guerrero.c
// Note: Rename to use your actual surname. File name should be in lowercase.
// You must remove any whitespaces or dashes from your surname.
// Note: **do not** modify or remove TODO comments


typedef struct card_s {
    char suit;                  // The card's suit (possible values: C, D, H, S).
    int rank;                   // The card's rank (possible values: 1-13, inclusive).
} card_t;

typedef struct LLNode_s {
    card_t card;                // Data stored in this node (a card).
    struct LLNode_s *next;      // A pointer reference to the next node, or NULL if it is the last element in the list.
} LLNode;

typedef struct LList_s {
    LLNode *head;               // A pointer to the linked list's head node, or NULL if list is empty.
} LList;

typedef struct player_s {
    char *name;                 // Dynamically allocated string of the player's name.
    LList *list_of_cards;       // Pointer to a linked list that holds all of the player's cards.
} player_t;

typedef struct GameBoard_s {
    int player_count;           // The number of players in the game.
    player_t **players;         // A dynamic array of pointers to player_t.
    LList *draw_pile;           // Pointer to a linked list for the draw pile, from which cards will be dealt.
    LList *discard_pile;        // Pointer to a linked list for the discard pile, where cards will be discarded.
} GameBoard;



// DO NOT MODIFY THIS PORTION
// function prototypes defined here
// the function definitions are located below the main function
GameBoard * load_file(FILE *, int *);
GameBoard * init_game_board(int);
LList * create_linked_list();
LLNode * create_card_node(char, int);
player_t * create_player(char *);
player_t ** create_list_of_players(int);
void destroy_linked_list(LList *);
void destroy_node(LLNode *);
void destroy_player(player_t *);
void destroy_list_of_players(player_t **, int);
void cleanup(GameBoard *);

void print_game_status(GameBoard *);
void distribute_draw_pile(GameBoard *);
void discard_all_cards(GameBoard *);
void give_card_node_to_player(player_t *, LLNode *);

int is_list_empty(LList *);
int get_linked_list_size(LList *);
void print_linked_list(LList *);
void add_node_to_list(LList *, LLNode *);
void add_node_to_list_proper(LList *, LLNode *);

LLNode * get_first_card_node_from_pile(LList *);
void print_card_detail(card_t *);
void display_player_information(player_t *);
int compare_card_nodes(LLNode *, LLNode *);
void trim_string(char *);

void reverse_card_pile(LList *);
void combine_linked_lists(LList *, LList *);



// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION
#ifndef MAIN_FUNCTION
int main(void) {
    // read the input file
    FILE *input_file = fopen(INPUT_FILE_NAME, "r");

    // check if the file was opened
    if(input_file == NULL) {
        // if there was a problem, simply return a 1
        printf("There was a problem opening the file. Abort!\n");
        return 1;
    }

    // the number of players indicated in the input file
    // this will be populated by the load_file() function
    int player_count;

    // load the file and pass the address of the player_count variable
    // for it to be populated. Additionally, this function returns a dynamically
    // allocated GameBoard (which we will call board)
    // it is possible for this function to return NULL if there
    // was a problem (e.g., cannot allocate memory)
    GameBoard *board = load_file(input_file, &player_count);

    // close the input file
    fclose(input_file);

    // check if the game board was dynamically allocated
    if(board == NULL) {
        // error; do not proceed
        printf("There was a problem setting up the game board. Abort!\n");
        return 2;
    }
    else {
        // indicate that the game board was successfully loaded
        printf("Loading Complete.\n");
    }


    // print the status of the game board before distributing the cards
    print_game_status(board);

    // distribute the cards among the players, one at a time
    // we do this in the following sequence (order): player 1, player 2, and so on
    // we keep doing this until the board's draw pile runs out of cards
    // therefore, it is possible for some players to receive fewer number of cards
    // compared to other players
    printf("Distributing Draw Pile to Players.\n");
    distribute_draw_pile(board);

    // print the status of the game board after distributing the cards
    print_game_status(board);

    // have the players return all the cards to the board's discard pile
    // we do this in the following sequence (order): player 1, player 2, and so on
    // for each player, the player returns or puts one card at a time to the discard pile
    // essentially, when the player is done returning all the cards, it will be
    // in reversed order in the discard pile
    printf("Discarding All Cards.\n");
    discard_all_cards(board);

    // print the status of the game board after all players discarded their cards
    print_game_status(board);


    // perform cleanup by deallocating and freeing all
    // the dynamically allocated spaces in this program
    cleanup(board);

    // discard the reference to the memory location of the game board
    board = NULL;


    return 0;
}
// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION
#endif


GameBoard * load_file(FILE *file_ptr, int *count_of_players) {
    // this function will build the game board based on the input file

    // only for processing the input file, we will assume that the names of the players
    // will have a maximum of 254 characters; however, when these players are later
    // created, their names must be dynamically allocated (i.e., strings)
    char name_temp[MAX_NAME_LENGTH];

    // store the number of players into the variable
    // passed to this function by reference
    fscanf(file_ptr, "%d\n", count_of_players);

    // create a dynamically allocated GameBoard
    GameBoard *board = init_game_board(*count_of_players);

    // check if the game board was successfully allocated
    if(board == NULL) {
        return NULL;
    }

    // process each player from the file
    for(int i = 0; i < *count_of_players; i++) {
        // read a line of string which corresponds
        // to the name of the player
        fgets(name_temp, sizeof(name_temp), file_ptr);

        // trim the string because it contains an extra \n
        trim_string(name_temp);

        // create a player; then store this in a dynamic array of pointers to players
        board->players[i] = create_player(name_temp);

        if(board->players[i] == NULL) {
            return NULL;
        }
    }

    // read from the file how many cards to process, usually 51 (but it may vary)
    int card_count;
    fscanf(file_ptr, "%d\n", &card_count);

    char suit_temp;
    int rank_temp;
    for(int i = 0; i < card_count; i++) {
        fscanf(file_ptr, "%c%d\n", &suit_temp, &rank_temp);

        // dynamically create a card node which will be added later
        // to the game board's draw pile
        LLNode *node = create_card_node(suit_temp, rank_temp);

        // add to the game board's draw pile
        add_node_to_list(board->draw_pile, node);
    }

    // return a pointer to the game board
    return board;
}


GameBoard * init_game_board(int count_of_players) {
    // dynamically allocate a game board
    GameBoard *board = malloc(sizeof(GameBoard));

    // check if the board was allocated successfully
    if(board == NULL) {
        return NULL;
    }

    // set the number of players
    board->player_count = count_of_players;

    // initialize the list of pointers to players
    board->players = create_list_of_players(count_of_players);

    // check if there is an issue
    if(board->players == NULL) {
        return NULL;
    }

    // initialize the draw pile list
    board->draw_pile = create_linked_list();

    if(board->draw_pile == NULL) {
        return NULL;
    }

    // initialize the discard pile list
    board->discard_pile = create_linked_list();

    if(board->discard_pile == NULL) {
        return NULL;
    }

    return board;
}


/**
 * @brief This function dynamically creates an empty linked list.
 * If it is unable to dynamically allocate the linked list at runtime, it simply returns a NULL.
 * It keeps track of the first element of the list which is referred to as the head.
 * Because the linked list is initially empty, head should be set to NULL.
 * @param None
 * @return A pointer to a LList, or NULL if unsuccessful.
 */
LList * create_linked_list() {
    // TODO 1 START
    // TODO 1: This function dynamically creates an empty linked list
    // check if the dynamic allocation was successful or not; if it was unsuccessful, return NULL
    // set the head pointer to NULL
    // afterward, return the linked list

	LList *list = malloc(sizeof(LList));
	
	if (list == NULL){
		return NULL; //if unsuccessful return null
	}
	
	list->head = NULL; //setting head pointer to null
	return list;


    // TODO 1 END
}



/**
 * @brief This function dynamically creates an LLNode which can be later be used in a LList.
 * If it is unable to dynamically allocate the node at runtime, it simply returns a NULL.
 * It sets the suit and the rank values of the card_t card of this node based on the arguments received.
 * It ensures that the suit parameter is in upper-case.
 * @param char suit
 * @param int rank
 * @return A pointer to a LLNode struct, or NULL if there is a problem.
 */
LLNode * create_card_node(char suit, int rank) {
    // dynamically allocate it
    LLNode *node = malloc(sizeof(LLNode));

    if(node == NULL) {
        return NULL;
    }

    // change to upper case the suit
    suit = toupper(suit);

    // set the attributes of the card
    (node->card).suit = suit;
    (node->card).rank = rank;

    // set the next to NULL by default
    node->next = NULL;

    return node;
}


player_t * create_player(char *name) {
    // dynamically allocate the player
    player_t *player = malloc(sizeof(player_t));

    if(player == NULL) {
        return NULL;
    }

    int len = strlen(name);
    player->name = malloc(sizeof(char)*(len+1));

    if(player->name == NULL) {
        return NULL;
    }

    // set the name of the player (dynamically allocated string)
    strcpy( player->name, name );

    // create the linked list
    player->list_of_cards = create_linked_list();

    if(player->list_of_cards == NULL) {
        return NULL;
    }

    return player;
}


player_t ** create_list_of_players(int count_of_players) {
    // TODO 2 START
    // TODO 2: This function accepts a number indicating the
    // number of slots (count of players) to dynamically allocate
    // for an array of pointers to a struct (player_t*)
    // this function returns a pointer to this dynamic array

	//allocating memory for an array of pointers to player_t
	player_t **players = malloc(sizeof(player_t)*count_of_players);
	
	if(players == NULL){
		return NULL; //return NULL if memory allocation fails
	}
	return players;



    // TODO 2 END
}



/**
 * @brief This function destroys or deallocates all dynamically allocated spaces associated with an existing LList.
 * @param LList *list
 * @return None
 */
void destroy_linked_list(LList *list) {
    // TODO 3 START
    // TODO 3: This function accepts a pointer to a linked list
    // it then deallocates or frees up all the memory spaces
    // that were allocated to all of its nodes and afterward,
    // to the linked list itself

	if (list == NULL){ //if nothing to deallocate, return NULL
		return;	
	}
	LLNode *current = list->head; //initialize current to head
	LLNode *next ;//declaring pointer next to store next node's address
	
	while (current != NULL){ //loop through linked list until current becomes NULL
		next = current -> next;
		free(current); //deallocate memory occupied by current node
		current = next;
	}
	free(list);



    // TODO 3 END
}


void destroy_node(LLNode *node) {
    // destroy current node
    free(node);
}


void destroy_player(player_t *player) {
    // deallocate name
    free(player->name);

    // destroy the list
    destroy_linked_list(player->list_of_cards);

    // deallocate player
    free(player);
}


/**
 * @brief This function destroys or deallocates all dynamically allocated spaces
 * associated with an existing array of pointers to a player_t struct.
 * @param player_t **players
 * @param int size
 * @return None
 */
void destroy_list_of_players(player_t **players, int size) {
    // TODO 4 START
    // TODO 4: This function accepts a pointer to a dynamic array of pointers to a struct (player_t)
    // and a number corresponding to the number of slots of this dynamic array of pointers
    // this function then deallocates or frees up all the memory spaces
    // that were allocated to each player (Hint: player_t struct) and afterward,
    // to the dynamic array itself

	if (players == NULL){
		return; //if players pointer NULL, return
	}
	
	for (int i=0; i<size; i++){
		destroy_player(players[i]); //deallocate each player in array of players
	}

	free(players);


    // TODO 4 END
}


void cleanup(GameBoard *board) {
    // deallocate all the players
    destroy_list_of_players(board->players, board->player_count);

    // deallocate the linked list (game)
    destroy_linked_list(board->draw_pile);

    // deallocate the linked list (discard)
    destroy_linked_list(board->discard_pile);

    // destroy the board
    free(board);
}


void print_game_status(GameBoard *board) {
    printf("Current Game Status\n");

    // print all the information about the players
    printf("There are %d Players:\n", board->player_count);
    for(int i = 0; i < board->player_count; i++) {
        // call the function that prints information about the
        // player along with listing all the cards this player has
        display_player_information(board->players[i]);
    }

    // print all the cards in the draw pile
    printf("The Draw Pile:\n");
    print_linked_list(board->draw_pile);

    // print all the cards in the discard pile
    printf("The Discard Pile:\n");
    print_linked_list(board->discard_pile);
}


void distribute_draw_pile(GameBoard *board) {
    // distribute all the cards from the game board's draw pile
    // to each player one at a time, in the following order: player 1, player 2, and so on
    int draw_pile_card_count = get_linked_list_size(board->draw_pile);

    for(int i = 0; i < draw_pile_card_count; i++) {
        // determine for whom it is based on the number of players
        int for_player = i % board->player_count;

        // remove the first card from the game board's draw pile
        LLNode *card_node = get_first_card_node_from_pile(board->draw_pile);

        // check first if there is an actual card node
        if(card_node != NULL) {
            // print the details
            printf("Giving to Player %s the Card ", board->players[for_player]->name);
            print_card_detail(&card_node->card);

            // insert; actually should not be an issue at this point but to be safe
            give_card_node_to_player(board->players[for_player], card_node);
        }
    }
}


void discard_all_cards(GameBoard *board) {
    // go through each player
    for(int i = 0; i < board->player_count; i++) {
        // print who is returning the pile to the discard pile
        printf("Player %s is Discarding Cards\n", board->players[i]->name);

        // reverse the current pile of cards of this player
        reverse_card_pile(board->players[i]->list_of_cards);

        // print what is being returned and in which order
        print_linked_list(board->players[i]->list_of_cards);

        // remove all the pile of cards from the player's list
        // and transfer them to the game board's discard pile
        combine_linked_lists(board->discard_pile, board->players[i]->list_of_cards);

        // clear this player's list of cards because all the cards
        // are going to be transferred to the game board's discard pile
        board->players[i]->list_of_cards->head = NULL;
    }
}


void give_card_node_to_player(player_t *player, LLNode *node) {
    // assume that these were allocated already
    add_node_to_list_proper(player->list_of_cards, node);
}


int is_list_empty(LList *list) {
    // check if the list is empty or not
    // based only on the head

    if(list->head == NULL) {
        return TRUE;
    }

    return FALSE;
}


int get_linked_list_size(LList *list) {
    // This function accepts a pointer to a linked list
    // and returns a number corresponding to the number of nodes
    // or elements this list currently has

    // return how many nodes there are in the linked list
    int count = 0;

    LLNode *ptr = list->head;

    while(ptr != NULL) {
        count++;
        ptr = ptr->next;
    }

    return count;
}


void print_linked_list(LList *list) {
    if( is_list_empty(list) ) {
        printf("List is Empty!\n");
        return;
    }

    // otherwise, traverse
    LLNode *ptr;
    ptr = list->head;

    // do until reached NULL for next
    while( ptr != NULL ) {
        // has to be a reference due to the fact that this
        // function expects an address to a card struct
        print_card_detail(&(ptr->card));
        ptr = ptr->next;
    }
}


void add_node_to_list(LList *list, LLNode *node) {
    // make sure to set the next to NULL
    node->next = NULL;

    // if it is empty, then simply create node
    // then set it as the head
    if( is_list_empty(list) ) {
        list->head = node;
        return;
    }

    // otherwise, traverse
    LLNode *ptr;
    ptr = list->head;

    // do until reached NULL for next
    while( ptr->next != NULL ) {
        ptr = ptr->next;
    }

    // add the next here
    ptr->next = node;
}



/**
 * @brief This function adds a LLNode to a LList. It inserts LLNode in its proper position.
 * Essentially, this function ensures that the LList will always be sorted based on a given criteria.
 * @param LList *list
 * @param LLNode *node
 * @return None
 */
void add_node_to_list_proper(LList *list, LLNode *node) {
    // TODO 5 START
    // TODO 5: This function accepts a pointer to a linked list and a pointer to a node
    // that needs to be added to the linked list
    // this function ensures that the node is going to be added
    // or inserted in its proper location; essentially, after calling this function,
    // all the nodes in the list are sorted from least to greatest based on a given criteria
    // Hint: this function can be easily implemented if you use the helper
    // function compare_card_nodes()

	if (list == NULL || node == NULL){
		return; //return if node or list is NULL
	}
	if(list->head == NULL){
		list->head = node; //if list empty, make new node the head
		node->next = NULL; //ensure new head's next pointer is NULL
		return;
	}
	
	if(compare_card_nodes(node,list->head)<0){
		node->next = list->head;
		list->head = node;
		return;
	}

	LLNode *current = list->head; //traversing from head
	
	while(current->next != NULL && compare_card_nodes(node,current->next)>=0){
		current = current->next;
	}
	
	node->next = current->next;//insert new node after current node
	current->next = node;
	
    // TODO 5 END
}




/**
 * @brief This function removes the first LLNode of a LList.
 * Afterward, it returns a reference to this LLNode.
 * @param LList *list
 * @return A pointer to a LLNode struct, or NULL if LList is empty.
 */
LLNode * get_first_card_node_from_pile(LList *list) {
    // TODO 6 START
    // TODO 6: This function accepts a pointer to a linked list from which
    // we will be removing the first element of; in the event that the list is currently empty,
    // simply return a NULL; otherwise, after removing this first element
    // this function returns a pointer to this node
    // Hint: you have to do some other operations to ensure the integrity of your linked list

	if(list == NULL || list->head == NULL){
		return NULL;
	}
	
	LLNode *first_node = list->head; //store address of head in first_node
	list->head = list->head->next; //update head to point to second node
	first_node->next = NULL;
	
	return first_node;//return a pointer to removed first node


    // TODO 6 END
}


void print_card_detail(card_t *card) {
    // this helper function prints the details of a card
    // referenced by a pointer
    // this has already been done in PA1
    printf("%c %2d\n", card->suit, card->rank);
}


void display_player_information(player_t *player) {
    // this helper function prints the details of a player
    // referenced by a pointer
    // this has already been done in PA1, but slightly modified here
    printf("Player Name: %s\n", player->name);
    printf("Card List\n");
    printf("*********\n");

    // print the list of cards of this player
    print_linked_list(player->list_of_cards);
}



/**
 * @brief Compare two card nodes.
 * For card comparisons, use card rank then card suit (alphabetically).
 * E.g. S2 < H3, because rank of 2 < rank of 3.
 * E.g. C3 < S3, because ranks are tied, and suit 'C' comes before suit 'S'.
 */
int compare_card_nodes(LLNode *node1, LLNode *node2) {
    // TODO 7 START
    // TODO 7: This is a helper function that accepts two pointers to two nodes and returns an integer
    // basically, this function determines which of the two nodes go first if we were
    // to arrange them; in short, we are trying to compare two nodes
    // this returns -1 if node1 has a card that has a lesser value compared to node2
    // it returns 1 if node1 has a card that has a higher value compared to node2
    // we first focus only on the RANKS of the two cards; a card with a lower rank
    // will go first, followed by a card with a higher rank;
    // for example S2 H3 (2 Spade goes first, followed by 3 Hearts)
    // in case of a tie in the rank, we break it based on the lexicographical order of the suit of the cards
    // for example: C3 S3 (3 Clubs goes first, followed by 3 Spades; alphabetically C goes first compared to S)
    // you can assume for this activity that all cards will be unique; for other cases, you can safely return 0

	if(node1 == NULL || node2 == NULL){
		return 0;
	}
	
	if(node1->card.rank < node2->card.rank){
		return -1;
	}else if (node1->card.rank>node2->card.rank){
		return 1;
	}else{
		//if ranks are equal, compare suits
		if(node1->card.suit<node2->card.suit){
			return -1;
		}else if (node1->card.suit>node2->card.suit){
			return 1;
		}else{
			return 0;
		}
	}




    // TODO 7 END
}


void trim_string(char *str) {
    // helper function that will remove the extra new line
    // added from reading the input file
    int len = strlen(str);
    /* OLD CODE
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
    */
    // Begin Bug Fix: 2025-02-26
    while(len > 0) {
        if(str[len-1] != '\n' && str[len-1] != '\r')
            break;
        len--;
    }
    str[len] = '\0';
    // End of Bug Fix: 2025-02-26
}


void reverse_card_pile(LList *list) {
    // TODO 8 START
    // TODO 8: This function accepts a pointer to a linked list
    // it then reverses all the elements of this linked list
    // ensure that the pointer to the head of this list is updated

	if(list == NULL || list->head == NULL){
		return;
	}
	
	LLNode *prev = NULL; //initialize prev to NULL
	LLNode *current = list->head; //initialize current to head of list
	LLNode *next = NULL; //initialize next to NULL
	
	while(current != NULL){
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	list->head = prev;



    // TODO 8 END
}


void combine_linked_lists(LList *list1, LList *list2) {
    // this is a helper function that will append list2
    // to list1

    // check if list1 is empty
    if( is_list_empty(list1) ) {
        // check if list2 is not empty
        if( !is_list_empty(list2) ) {
            list1->head = list2->head;
        }

        return;
    }

    // if list2 is empty, there is nothing to do
    if( is_list_empty(list2) ) {
        return;
    }

    // if list1 is not empty, go to the last element of list1
    // then connect list2 to it
    LLNode *ptr;
    ptr = list1->head;

    // go to the last element of list1
    while(ptr->next != NULL) {
        ptr = ptr->next;
    }

    // set the next to the first element of list2
    ptr->next = list2->head;
}
