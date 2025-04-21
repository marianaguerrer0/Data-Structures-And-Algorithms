#include <stdio.h>
#include <stdlib.h>
#define INPUT_FILE_NAME "pa4_data.txt"

// COP 3502C Spring 2025
// Student Name: Mariana Guerrero
// File Name: pa4_yoursurname.c
// Note: Rename to use your actual surname. File name should be in lowercase.
// You must remove any whitespaces or dashes from your surname.
// Note: **do not** modify or remove TODO comments


// DO NOT MODIFY THIS PORTION
// function prototypes defined here
unsigned long get_the_element(unsigned long *, unsigned long);



// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION
int main(void) {
    FILE *input_file = fopen(INPUT_FILE_NAME, "r");

    // check if the file was opened
    if(input_file == NULL) {
        // if there was a problem, simply return a 1
        printf("There was a problem opening the file. Abort!\n");
        return 1;
    }

    // the first line of the input file indicates
    // how many numbers to read from the file
    // you can assume that this will be an odd
    // number; note that this can be a very big
    // number to represent a very big list
    unsigned long count;
    fscanf(input_file, "%lu", &count);

    // dynamically allocate this array of this size
    unsigned long *list = malloc(sizeof(unsigned long) * count);

    unsigned long i = 0;
    // read the remaining numbers and
    // store them in the array
    unsigned long num;
    while( fscanf(input_file, "%lu", &num) != EOF ) {
        list[i++] = num;
    }

    // close the file
    fclose(input_file);

    // output the element we are looking for
    printf("%lu\n", get_the_element(list, count));


    return 0;
}
// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION

//Creating the following helper functions will make our solution run
//in O(n lg n). Implementing bubble sorting would have not managed to get the desired
//running time solution.


//new helper function to manage the array of the next helper function
unsigned long p(unsigned long *arr, unsigned long low, unsigned long high){
	unsigned long x = arr[high];
	unsigned long y = low - 1;
	
	for (unsigned long i = low; i < high; i++){
		if(arr[i] <= x){
			y++;
			unsigned long z = arr[y];
			arr[y] = arr[i];
			arr[i] = z;
		}
	}
	unsigned long z = arr[y+1];
	arr[y+1] = arr[high];
	arr[high] = z;
	
	return y + 1;
}

//new helper selection function
unsigned long q(unsigned long *arr, unsigned long low, unsigned long high, unsigned long n){
	if (low <= high){
		unsigned long xIndex = p(arr, low, high); //partition index
		
		if (xIndex == n){
			return arr[xIndex]; //finds the nth element
		}
		else if(xIndex < n){
			return q(arr, xIndex + 1, high, n); //recursive function
		}
		else{
			return q(arr, low, xIndex - 1, n); //recursive function
		}
	}
	return 0;
}

unsigned long get_the_element(unsigned long *arr, unsigned long size) {
    // TODO START
    // TODO: Return the ((n+1)/2)-th highest element
    // of an odd-length list. Assume that all the numbers
    // in the list are distinct.
   
	unsigned long n = (size + 1)/2-1; //gets the ((n+1)/2)-th element
	return q(arr, 0, size - 1, n); // helper function call

    // TODO END
}

// You may define other helper functions if you want
