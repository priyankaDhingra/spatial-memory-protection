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
void * HEAD_BASE = 0;
void * HEAD_BOUND = 1;
struct freeptrs {
	int data;
	struct freeptrs *next;
}*head;
void try_going_beyond(void *p) {

	srand(time(NULL));
	int num = rand() / 1000;
	printf("num %d=", num);
		p = p + num;
	printf("display p->data=%p\n", ((struct freeptrs*) p)->next);
}

void insertel(struct freeptrs *fp, void *base, void *bound) {

	if (head == NULL) {
		//copy pointer
		head = fp;
		HEAD_BASE = base;
		HEAD_BOUND = bound;
	} else {
		struct freeptrs *temp;
		//copy pointer
		temp = head;
		struct metadata_entry *tmp_me;
		tmp_me->base = HEAD_BASE;
		tmp_me->bound = HEAD_BOUND;

		while ((spatial_check(&(temp),tmp_me->base,tmp_me->bound))&&(temp->next != NULL)) {
			//copying
			//check

			temp = temp->next;
			tmp_me->base = temp->next;
			tmp_me->bound = (temp->next) + sizeof(struct freeptrs);
		}
		//copy pointer
		//check
		temp->next = fp;
		//check
		tmp_me->base = fp->next;
		tmp_me->bound = (fp->next) + sizeof(struct freeptrs);
	}
}

void printall() {
	struct freeptrs *temp;
	//copy pointer
	temp = head;
	struct metadata_entry *tmp_me;
	tmp_me->base = HEAD_BASE;
	tmp_me->bound = HEAD_BOUND;
	try_going_beyond(temp,tmp_me->base,tmp_me->bound);
	if (head == NULL) {
		printf("List is empty");
	} else {
		while (temp != NULL) {
			//check
			printf("value = %d\n", (temp->data));
			//indexing/copying
			temp = temp->next;
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
	metadata_store(&fp, fp, fp + sizeof(struct freeptrs));
	insertel(fp, fp, fp + sizeof(fp));

	fp1 = (struct freeptrs*) malloc(sizeof(struct freeptrs));

	fp1->data = 22;
	fp1->next = NULL;
	metadata_store(&fp1, fp1, fp1 + sizeof(fp1));
	insertel(fp1, fp1, fp1 + sizeof(fp));

	fp2 = (struct freeptrs*) malloc(sizeof(struct freeptrs));
	fp2->data = 212;
	fp2->next = NULL;
	metadata_store(&fp2, fp2, fp2 + sizeof(fp2));
	insertel(fp2, fp2, fp2 + sizeof(fp2));

	fp3 = (struct freeptrs*) malloc(sizeof(struct freeptrs));
	fp3->data = 23123;
	fp3->next = NULL;
	metadata_store(&fp3, fp3, fp3 + sizeof(fp3));
	insertel(fp3, fp3, fp3 + sizeof(fp3));

	fp4 = (struct freeptrs*) malloc(sizeof(struct freeptrs));
	fp4->data = 2123;
	fp4->next = NULL;
	metadata_store(&fp4, fp4, fp4 + sizeof(fp4));
	insertel(fp4, fp4, fp4 + sizeof(fp4));

	int dt = 2123;

	printall();

	return 0;
}
