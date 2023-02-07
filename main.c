#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"
#include <stdbool.h>

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {
	// stack stores the lines to print out
	stack_t *s = initstack();
	char* searchTerm = NULL;
	int depth = 0;
	char* filename = ".";

	if (argc >= 2) {
		filename = argv[1];
	}
	if (argc >= 3) {
		searchTerm = argv[2];
	}
	//fileSearch(s, filename, searchTerm, depth);
	
	bool containsTargetFile = ls2(s, filename, searchTerm, depth);

	/*
	if (containsTargetFile) {
        printf("The desired file was found\n");
    } else {
		printf("The desired file was not found\n");
	}
	*/

	// print stack
	printstack(s);

	// free up stack
	freestack(s);
	return 0;
}
