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
struct freeptrs {
	void* data;
	struct freeptrs *next;
}*head;
void try_going_beyond(void *p) {

	srand(time(NULL));
	int num = rand() / 1000;
	printf("num %d=", num);
	struct metadata_entry me = metadata_load(&head);
	if (spatial_check(p, me.base, me.bound, num) == 1) {
		printf("not violated");
		p = p + num;
	}else{
		printf("----------------------violated\n");
	}
	printf("display p->data=%p\n", ((struct freeptrs*) p)->next);
}
void insertel(struct freeptrs *fp) {
	struct freeptrs *temp;
	temp = head;
	if (head == NULL) {
		head = fp;
	} else {
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = fp;
	}
}
void removel(int dt) {

	struct freeptrs *temp;
	temp = head;

	//if head node
	if ((int) temp->data == dt) {
		struct freeptrs *del;
		del = temp;
		head = temp->next;
		free(del);
	} else {
		while (temp->next != NULL) {
			if (((int) temp->next->data) == dt) {
				struct freeptrs *del;
				del = temp->next;
				temp->next = temp->next->next;
				free(del);
				break;
			}

			temp = temp->next;
		}

	}
}
void printall() {
	struct freeptrs *temp;
	temp = head;
	try_going_beyond(temp);
	if (head == NULL) {
		printf("List is empty");
	} else {
		while (temp != NULL) {
			printf("value = %d\n", (int) (temp->data));
			temp = temp->next;
		}
	}
}

int main() {
	//struct freeptrs *xp;
	//xp->data=121;
init();
	//pointer allocation
	struct freeptrs *fp;
	fp = (struct freeptrs*) malloc(sizeof(struct freeptrs));
	fp->data = (int*) 23;
	fp->next = NULL;
	insertel(fp);
//	printf("xpnext:=%p", xp->next->data);
	fp = (struct freeptrs*) malloc(sizeof(struct freeptrs));

	fp->data = (int*) 22;
	fp->next = NULL;
	insertel(fp);

	fp = (struct freeptrs*) malloc(sizeof(struct freeptrs));
	fp->data = (int*) 212;
	fp->next = NULL;
	insertel(fp);

	fp = (struct freeptrs*) malloc(sizeof(struct freeptrs));
	fp->data = (int*) 23123;
	fp->next = NULL;
	insertel(fp);

	fp = (struct freeptrs*) malloc(sizeof(struct freeptrs));
	fp->data = (int*) 2123;
	fp->next = NULL;
	insertel(fp);
	printf("----------------------head+5=%p\n",head+5);
	metadata_store(&head, head,head+5 );
	int dt = 2123;
	removel(dt);

	printall();

	return 0;
}
