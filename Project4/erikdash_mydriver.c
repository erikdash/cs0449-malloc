#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "mymalloc.h"
//include your code


//replace malloc here with the appropriate version of mymalloc
#define MALLOC my_worstfit_malloc
//replace free here with the appropriate version of myfree
#define FREE my_free

struct student
{
	int age;
	int ID;
	int score;
}student;

struct employee
{
	int ID;
}emplyee;

int main()
{
	int i;
	printf("size of student: %d\n", sizeof(struct student));

	printf("original val brk: %p\n", sbrk(0));

	struct student *student1 = (struct student*)MALLOC(sizeof(struct student));
	student1->age = 19;
	student1->ID = 12345;
	student1->score = 85;
	printf("val1 brk: %p\n", sbrk(0));

	struct student *student2 = (struct student*)MALLOC(sizeof(struct student));
	student2->age = 21;
	student2->ID = 678910;
	student2->score = 88;
	printf("val2 brk: %p\n", sbrk(0));
	
	struct student *student3 = (struct student*)MALLOC(sizeof(struct student));
	student3->age = 20;
	student3->ID = 111213;
	student3->score = 75;
	printf("val3 brk: %p\n", sbrk(0));

	FREE(student2);

	struct employee *employee1 = (struct employee*)MALLOC(sizeof(struct employee));
	employee1->ID = 246810;
	printf("val4 brk: %p\n", sbrk(0)); //this allocation should fit in newly allocated space in heap, so brk value shouldn't change from previous value

	struct student *student4 = (struct student*)MALLOC(sizeof(struct student));
	student4->age = 18;
	student4->ID = 141516;
	student4->score = 77;
	printf("val5 brk: %p\n", sbrk(0)); //there is leftover space from the free, but not large enough for this allocation, so brk will be incremented

	for(i = 0; i < 10; i++)
	{
		struct student *new = (struct student*)MALLOC(sizeof(struct student));
		new->age = i;
		new->ID = i*i*i;
		new->score = 80 + i;
	}//this loop allocates ten students
	
	printf("val6 brk: %p\n", sbrk(0));

	struct employee *employee2 = (struct employee*)MALLOC(sizeof(struct employee));
	employee2->ID = 121416;
	printf("val7 brk: %p\n", sbrk(0));

	FREE(employee2);
	printf("val8 brk: %p\n", sbrk(0)); //should be equal to val6, as we are freeing from end	

	printf("final val brk: %p\n", sbrk(0));

	return 0;
}
