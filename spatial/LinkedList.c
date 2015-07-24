/*
 * LinkedList.c
 *
 *  Created on: May 28, 2015
 *      Author: priyanka
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "metadata.c"
void* HEAD_BASE = 0;
void* HEAD_BOUND = 0;
struct freeptrs {
	int data;
	struct freeptrs *next;
}*head;
void try_going_beyond(void *temp) {
	struct metadata_entry tmp_me;
	tmp_me = metadata_load(temp);
	srand(time(NULL));
	int num = rand() ;
	printf("num %d=", num);
	if (spatial_check(temp, tmp_me.base, tmp_me.bound, num) == 1) {
		temp = temp + num;
		printf("display p->data=%d\n", ((struct freeptrs*) temp)->data);
	} else {
		printf("spatial violation");
	}
}

void insertel(struct freeptrs *fp, void *base, void *bound) {
	if (head == NULL) {
		//copy pointer
		head = fp;
		HEAD_BASE = base;
		HEAD_BOUND = bound;
		metadata_store(fp, fp, fp + sizeof(struct freeptrs));
	} else {
		struct freeptrs *temp;
		//copy pointer
		temp = head;
		struct metadata_entry tmp_me;
		tmp_me.base = HEAD_BASE;
		tmp_me.bound = HEAD_BOUND;
		try_going_beyond(temp);
		while (temp->next != NULL) {
			//copying
			//check

			if (spatial_check(temp->next, tmp_me.base, tmp_me.bound, 0) == 1) {
				tmp_me = metadata_load(temp->next);

				temp = temp->next;
				try_going_beyond(temp);
			} else {
				break;
			}
		}
		//copy pointer
		//check
		temp->next = fp;
		try_going_beyond(temp);
		//check
		metadata_store(fp, fp, fp + sizeof(struct freeptrs));
	}
}

void printall() {
	struct freeptrs *temp;
	//copy pointer
	temp = head;
	struct metadata_entry tmp_me;
	tmp_me.base = HEAD_BASE;
	tmp_me.bound = HEAD_BOUND;
	try_going_beyond(temp);
	if (head == NULL) {
		printf("List is empty");
	} else {
		while (temp != NULL) {
			//check

			printf("value = %d\n", (temp->data));
			if (spatial_check(temp->next, tmp_me.base, tmp_me.bound, 0) == 1) {
				tmp_me = metadata_load(temp->next);

				temp = temp->next;
				try_going_beyond(temp);
			} else {
				break;
			}
			//indexing/copying

//		temp = (temp+2)->next;
//		printf("value = %d\n", (temp->data));
		}
	}
}
int main() {
	init();
	//pointer allocation
	struct freeptrs *fp, *fp1, *fp2, *fp3, *fp4;
	fp = (struct freeptrs*) malloc(sizeof(struct freeptrs));
	fp->data = 23;
	fp->next = NULL;
	insertel(fp, fp, fp + sizeof(fp));
	try_going_beyond(fp);
	fp1 = (struct freeptrs*) malloc(sizeof(struct freeptrs));

	fp1->data = 22;
	fp1->next = NULL;
	insertel(fp1, fp1, fp1 + sizeof(fp));

	fp2 = (struct freeptrs*) malloc(sizeof(struct freeptrs));
	fp2->data = 212;
	fp2->next = NULL;
	insertel(fp2, fp2, fp2 + sizeof(fp2));

	fp3 = (struct freeptrs*) malloc(sizeof(struct freeptrs));
	fp3->data = 23123;
	fp3->next = NULL;
	insertel(fp3, fp3, fp3 + sizeof(fp3));

	fp4 = (struct freeptrs*) malloc(sizeof(struct freeptrs));
	fp4->data = 2123;
	fp4->next = NULL;

	insertel(fp4, fp4, fp4 + sizeof(fp4));

	int dt = 2123;

	printall();

	return 0;
}
