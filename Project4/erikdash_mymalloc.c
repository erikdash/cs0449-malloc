#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mymalloc.h"


typedef struct dataNode
{
	int sizeofchunk;
	int isfree;
	struct dataNode *next;
	struct dataNode *prev; 
}dataNode;

typedef struct dataNodeList
{
	struct dataNode *listHead;
	struct dataNode *listTail;
}dataNodeList;

//dataNodeList dataList;
//initializeList(dataList);

static dataNode *listHead = 0;
static dataNode *listTail = 0;
//static dataNode *curr = 0;

void initializeNode(dataNode *newNode, int size)
{
	if(listHead == NULL)
	{
		listHead = newNode;
	}
	if(listTail == NULL)
	{
		listTail = newNode;
	}
	newNode->sizeofchunk = size;
	newNode->isfree = 1;
	listTail->next = newNode;
	//newNode->next->prev = newNode;
	newNode->next = NULL;
	//newNode->prev = listTail;
	listTail = newNode;
}

void *initList(int size)
{
	listHead = sbrk(sizeof(dataNode));

	listHead->sizeofchunk = size;
	listHead->isfree = 1;
	listHead->next = NULL;
	listHead->prev = NULL;

	listTail = listHead;
	//cur = listHead;
	//void *newbrk = sbrk(size);
	return sbrk(size);
}

void *extendheap(int blockSize)
{
	struct dataNode *newNode = sbrk(sizeof(struct dataNode));
	newNode->sizeofchunk = blockSize;
	newNode->isfree = 1;
	newNode->prev = listTail;
	newNode->next = NULL;
	listTail->next = newNode;

	listTail = newNode;
	return sbrk(blockSize);
}

void *my_worstfit_malloc(int size)
{
	if(listHead == 0)
	{
		return (initList(size));
	}
	else
	{
		dataNode *ptr = worst_fit(size);
		if(ptr == NULL) //no open space that will fit
		{
			return extendheap(size);
		} 
		else
		{
			return ptr; //will be NULL
		}
	}

	
	//else if(worst_fit(size) == NULL)
	//{
	//	return extendheap(size);
	//}
	//else if(worst_fit(size) != NULL)
	//{
	//	return worst_fit(size);
	//}
	//return ptr;
}


void my_free(void *ptr)
{
	dataNode *thisBlock = ptr - (sizeof(dataNode));
	if(thisBlock->isfree == 0) //make sure the block isn't already free
	{
		return;
	}
	thisBlock->isfree = 0; //mark this space as free
	dataNode *nextBlock = thisBlock->next;
	if((nextBlock != NULL) && (nextBlock->isfree == 0)) //coalesce with next block if free
	{
		thisBlock->sizeofchunk += ((sizeof(dataNode)) + (nextBlock->sizeofchunk));
		thisBlock->next = nextBlock->next; //adjust list pointers
		//nextBlock->next->prev = thisBlock; //to remove nextBlock from list
	}
	dataNode *prevBlock = thisBlock->prev;
	if((prevBlock != NULL) && (prevBlock->isfree == 0)) //coalesce with the previous block if free
	{
		prevBlock->sizeofchunk += ((sizeof(dataNode)) + (thisBlock->sizeofchunk));
		prevBlock->next = thisBlock->next; //remove prevBlock from linked list
		//if(thisBlock->next != NULL)
		//{
		//	thisBlock->next->prev = thisBlock->prev;
		//}
		thisBlock = prevBlock;
	}
	//listTail->next = thisBlock;
	//listTail = thisBlock;
	if(thisBlock == listTail) //if this block is at the end, remove it
	{
		listTail = thisBlock->prev;
		thisBlock->prev->next = NULL;
		sbrk(-1 * ((thisBlock->sizeofchunk) + sizeof(dataNode)));
	}
}



void unfragment(dataNode *ptr, int blockSize)
{
	dataNode *newChunk = ptr + sizeof(dataNode) + ptr->sizeofchunk;
	newChunk->sizeofchunk = blockSize;
	newChunk->isfree = 0;
	newChunk->next = ptr->next;
	newChunk->prev = ptr;
	ptr->next = newChunk;
	ptr->next->prev = newChunk;
}





void *worst_fit(int blockSize)
{
	dataNode *thisNode = listHead->next;

	dataNode *largestBlock = NULL;
	
	if((listHead->isfree == 0) && (listHead->sizeofchunk >= blockSize))//if the listHead is free and large enough
	{
		largestBlock = listHead;
	}
	while((thisNode != NULL) && (thisNode != listTail))
	{
		if((largestBlock != NULL) && (thisNode->isfree == 0) && (thisNode->sizeofchunk >= blockSize) && (thisNode->sizeofchunk > largestBlock->sizeofchunk))
		{
			largestBlock = thisNode;
		}
		else if((thisNode->isfree == 0) && (thisNode->sizeofchunk >= blockSize))
		{
			largestBlock = thisNode;
		}
		thisNode = thisNode->next;
	}
	if((largestBlock != NULL) && (listTail->isfree == 0) && (listTail->sizeofchunk > largestBlock->sizeofchunk) && (listTail->sizeofchunk >= blockSize))
	{
		largestBlock = listTail;
	}
	else if((listTail->isfree == 0) && (listTail->sizeofchunk >= blockSize))
	{
		largestBlock = listTail;
	}
	
	if(largestBlock != NULL)
	{
		largestBlock->isfree = 1;
		if(largestBlock->sizeofchunk > (blockSize + sizeof(dataNode)))
		{
			unfragment(largestBlock, ((largestBlock->sizeofchunk)-blockSize-(sizeof(dataNode))));
		}
		return largestBlock + sizeof(dataNode);
	}
	else
	{
		return NULL;
	}
}


