/*
 * dmll.c
 *
 *  Created on: Jun 16, 2015
 *      Author: priyanka
 */

#include<stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include<string.h>
void * table = 0;

struct freeptrs {
	void* key;
	void* base;
	void* bound;
	struct freeptrs *next;
}*head;
static const size_t TABLE_ENTRIES = 4 * sizeof(struct freeptrs);

int spatial_check(int *p, int* p_base, int* p_bound,int size) {
	if (p < p_base || p + size >= p_bound) {
		//violation
		return 0;
	}
	return 1;
}

void create_lookup() {
	table = mmap(0, TABLE_ENTRIES, PROT_READ | PROT_WRITE,
	MAP_PRIVATE | MAP_ANON, -1, 0);

	if (table == MAP_FAILED) {
		perror("Couldn't mmap");
	} else {
		head = table;
	}

}

void insertel(struct freeptrs *fp) {
	struct freeptrs *temp;
	temp = head;

	if (head == NULL) {
		create_lookup();
		table = memcpy(table, fp, sizeof(struct freeptrs));
		table = table + sizeof(struct freeptrs);
		//head = fp;
	} else {
		while (temp->next != NULL) {
			temp = temp->next;

		}
		table = memcpy(table, fp, sizeof(struct freeptrs));
		fp = table;
		table = table + sizeof(struct freeptrs);
		temp->next = fp;

	}
}

void printall() {
	struct freeptrs *temp, *temp2;
	temp = head;

	if (head == NULL) {
		printf("List is empty");

	} else {
		while (temp->next != NULL) {
			temp = temp->next;

		}

	}

}

void store_metadata(void* address, void* base, void * bound) {
	struct freeptrs *fp = malloc(sizeof(struct freeptrs));

	fp->key = address;
	fp->base = base;
	fp->bound = bound;
	fp->next = NULL;
	insertel(fp);
	free(fp);
}
struct freeptrs* load_metadata(void* address) {
	struct freeptrs *temp, *temp2;
	temp = head;
	if (head == NULL) {
		printf("List is empty");

	} else {
		if (temp->key == address) {
			return temp;
		} else {
			while (temp->next != NULL || temp->key == address) {
				temp = temp->next;
			}
			if (temp->key == address) {
				return temp;
			}

		}
	}

	return temp;
}

int main(int argc, char** argv) {


	int x[4];
	int *p = x;
	printf("size:%d\n",sizeof(x));
	store_metadata(&p, p, p+4);

	int *q;
	q = p;
	store_metadata(&q, q, 4+p);
	int num, i;
	int result = 0;

	printall();
	struct freeptrs *arr_spa = malloc(sizeof(struct freeptrs));
	arr_spa = load_metadata(&p);
	for (i = 0; i < 6; i++) {


		if (spatial_check(p, arr_spa->base, arr_spa->bound, i) == 1) {
			p[i] = rand();
		} else {
			printf("out of bound access to arr\n");
			break;
		}

		printf("arr[%d]=%d\n", i, p[i]);
	}

	printf("enter a number\n");
	scanf("%d", &num);
	struct freeptrs *arr_spa1 = malloc(sizeof(struct freeptrs));
	for (i = 0; i < num; i++) {
		arr_spa = load_metadata(&p);
		if (spatial_check(p, arr_spa->base, arr_spa->bound,  i) == 1) {
			result = result + p[i];
		} else {
			printf("out of bound access to arr\n");
			break;

		}

	}
	printf("the result is %zd\n", result);
	return 0;

}
