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

//struct spatialattr {
//	void* key;
//	void* base;
//	void* bound;
//};
struct freeptrs {
	void* key;
	void* base;
	void* bound;
	struct freeptrs *next;
}*head;
static const size_t TABLE_ENTRIES = 4 * sizeof(struct freeptrs);
int spatial_check(void *p, void *p_base, void* p_bound, int size) {
	printf("p = %p\n", (p));
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
		printf("base = %p\n", (temp->base));
		printf("bound = %p\n", (temp->bound));
		printf("key = %p\n", (temp->key));
		printf("next = %p\n", (temp->next));
		printf("\n-----------------------------\n");
		while (temp->next != NULL) {
			temp = temp->next;
			printf("base = %p\n", (temp->base));
			printf("bound = %p\n", (temp->bound));
			printf("key = %p\n", (temp->key));
			printf("next = %p\n", (temp->next));
			printf("\n-----------------------------\n");

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
	printf("\n--------111$---------\n");
	if (head == NULL) {
		printf("List is empty");

	} else {
		if (temp->key == address) {
			printf("\n--------$$222$$$---------\n");
			printf("base = %p\n", (temp->base));
			printf("bound = %p\n", (temp->bound));
			printf("key = %p\n", (temp->key));
			printf("next = %p\n", (temp->next));
			return temp;
		} else {
			printf("\n--------$$$$$$$$$$$---------\n");
			while (temp->next != NULL || temp->key == address) {
				temp = temp->next;
				printf("base = %p\n", (temp->base));
				printf("bound = %p\n", (temp->bound));
				printf("key = %p\n", (temp->key));
				printf("next = %p\n", (temp->next));
				printf("\n--------$$$$$$$$$$$---------\n");

			}
			if (temp->key == address) {
				return temp;
			}

		}
	}
	return temp;
}

int main(int argc, char** argv) {

	int arr[4];
	store_metadata(&arr, &arr, sizeof(arr) + &arr);

	int x[10];
	int *p = x;
	store_metadata(&p, &p, sizeof(x) + &p);

	int *q;
	q = p;
	store_metadata(&q, &q, sizeof(q) + &q);
	int num, i;
	int result = 0;

	printf("printall\n");
	printall();
	struct freeptrs *arr_spa = malloc(sizeof(struct freeptrs));
	for (i = 0; i < 4; i++) {

		arr_spa = load_metadata(&arr);
		if (spatial_check(&arr, arr_spa->base, arr_spa->bound, i) == 1) {
			arr[i] = rand();
		} else {
			printf("out of bound access to arr\n");
			break;
		}

		printf("arr[%d]=%d\n", i, arr[i]);
	}

	printf("enter a number\n");
	scanf("%d", &num);
	struct freeptrs *arr_spa1 = malloc(sizeof(struct freeptrs));
	for (i = 0; i < num; i++) {
		arr_spa = load_metadata(&arr);
		if (spatial_check(&arr, arr_spa->base, arr_spa->bound, i) == 1) {
			result = result + arr[i];
		} else {
			printf("out of bound access to arr\n");
			break;

		}

	}
	printf("the result is %zd\n", result);
	return 0;

}
