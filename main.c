#include <stdio.h>
#include "balloc.h"


int main() {
	int* newMem = balloc(sizeof(int));
	void* newMem2 = balloc(5 * sizeof(int));
	void* newMem3 = balloc(5 * sizeof(int));
	void* newMem4 = balloc(20 * sizeof(int));
	void* newMem5 = balloc(100 * sizeof(int));
	printHeap();
	printf("\ntesting \n\n");
	bfree(newMem2);
	bfree(newMem4);
	printHeap();
	printf("\ntesting \n\n");
	bfree(newMem3);
	printHeap();
	printf("\ntesting \n\n");
	void* newMem6 = balloc(20 * sizeof(int));
	printHeap();
}
