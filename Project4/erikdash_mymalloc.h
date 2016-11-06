/*
	Interface for mymalloc.c
*/
 
void *my_worstfit_malloc(int size);

void my_free(void *ptr);

void *worst_fit(int blockSize);

void *worst_fit2(int size);

//void initializeNode(dataNode *newNode, int size);

//void initializeList();
