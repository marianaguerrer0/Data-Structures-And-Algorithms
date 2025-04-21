#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define LETTER_COUNT 26
#define INPUT_FILE_NAME "pa5_data.txt"

// COP 3502C Spring 2025
// Student Name: Mariana Guerrero
// File Name: pa5_yoursurname.c
// Note: Rename to use your actual surname. File name should be in lowercase.
// You must remove any whitespaces or dashes from your surname.
// Note: **do not** modify or remove TODO comments


typedef struct TrieNode_s {
    struct TrieNode_s *children[LETTER_COUNT];
    int is_terminal;
} TrieNode;



// DO NOT MODIFY THIS PORTION
// function prototypes defined here
int max_num_prefix_words(TrieNode *);
TrieNode * create_node();
void destroy_node(TrieNode *);
int get_char_index(char);
void insert(TrieNode *, char *);
void trim_string(char *);



// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION
int main(void) {
    FILE *input_file = fopen(INPUT_FILE_NAME, "r");

    // check if the file was opened
    if(input_file == NULL) {
        // if there was a problem, simply return a 1
        printf("There was a problem opening the file. Abort!\n");
        return 1;
    }

    // dynamically allocate the root node
    // of a trie
    TrieNode *root = create_node();

    char word[256];

    // read all the words from the file
    while( fgets(word, 256, input_file) ) {
        // trim the string
        trim_string(word);

        // insert the word to the trie
        insert(root, word);
    }

    // close the file
    fclose(input_file);

    // output the answer
    printf("%d", max_num_prefix_words(root));

    // deallocate trie
    destroy_node(root);

    return 0;
}
// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION



int max_num_prefix_words(TrieNode *node) {
    // TODO START
    // TODO: Given the root of a trie that
    // stores a dictionary of words, return
    // the maximum number of words that are
    // prefixes of a single word.

	if (node == NULL) {
        return 0; //if trie empty, no prefixes exist
    }

    int max_prefixes = 0;
    int current_prefixes = 0;

    char current_word[256] = {0}; //initialize all to null
    int current_word_index = 0; //index to keep track of current character

	//recursive function to traverse trie and count prefixes
    void traverse_trie(TrieNode *current_node) {
        if (current_node->is_terminal) {
            int prefix_count = 0;
            TrieNode *temp = node;
            for (int i = 0; i < strlen(current_word); i++) {
                temp = temp->children[get_char_index(current_word[i])];
                if (temp->is_terminal) {
                    prefix_count++;
                }
            }
            if (prefix_count > max_prefixes) {
                max_prefixes = prefix_count;
            }
        }
		//recursively traverse the children of the current node
        for (int i = 0; i < LETTER_COUNT; i++) {
            if (current_node->children[i] != NULL) {
                current_word[current_word_index] = 'a' + i;
                current_word[current_word_index + 1] = '\0';
                current_word_index++;
                traverse_trie(current_node->children[i]);
                current_word_index--;
                current_word[current_word_index] = '\0';
            }
        }
    }

    traverse_trie(node); //start trie traversal from root

    return max_prefixes;

    // TODO END
}


TrieNode * create_node() {
    // dynamically create a new node
    TrieNode *node = malloc(sizeof(TrieNode));

    // set all children pointers to NULL
    for(int i = 0; i < LETTER_COUNT; i++) {
        node->children[i] = NULL;
    }

    // set the flag to false
    // indicating that this is not
    // a terminal node for a certain string
    node->is_terminal = 0;

    return node;
}


void destroy_node(TrieNode *node) {
    // this is a postorder traversal
    // visit all the children first
    // then visit the current node
    // base case
    if(node == NULL)
        return;

    // postorder visit all the children
    // and individually destroy them
    for(int i = 0; i < LETTER_COUNT; i++) {
        destroy_node(node->children[i]);
    }

    // deallocate this current node
    free(node);
}


int get_char_index(char alpha) {
    // change to lowercase first
    alpha = tolower(alpha);

    return alpha-'a';
}


void insert(TrieNode *root, char *str) {
    // create a pointer to the root
    TrieNode *ptr = root;

    // traverse through each character
    // of the string
    int len = strlen(str);

    for(int i = 0; i < len; i++) {
        int idx = get_char_index(str[i]);

        // check if there is no existing
        // node for this letter
        if(ptr->children[idx] == NULL) {
            // create a new node for that
            // letter
            ptr->children[idx] = create_node();
        }

        // traverse to the next node
        ptr = ptr->children[idx];
    }

    // once we reached the last letter
    // ptr is pointing at the last node
    // therefore, set the flag of this
    // node to 1 or true
    ptr->is_terminal = 1;
}


void trim_string(char *str) {
    // helper function that will remove the extra new line
    // begin at location right before
    // the null terminator of the string
    int len = strlen(str) - 1;

    // from the right, keep moving to the left;
    // continue doing so until what you see right
    // now is not a newline character anymore
    while(len > 0) {
        // stop moving to the left once you don't
        // see a newline character anymore
        if(str[len] != '\n' && str[len] != '\r')
            break;

        len--;
    }

    // len is currently pointing at the
    // last character right before a newline
    // therefore, it should be immediately
    // followed by the null character
    str[len+1] = '\0';
}
