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

struct spatialattr {
	void* key;
	void* base;
	void* bound;
};
struct freeptrs {
	struct spatialattr* data;
	struct freeptrs *next;
}*head;
static const size_t TABLE_ENTRIES = ((size_t) 8 * (size_t) 1024 * (size_t) 1024)*sizeof(struct freeptrs);
int spatial_check(void *p, int p_base, int p_bound, int size) {

	if (p < p_base || p + size >= p_bound) {
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
		printf("orig address--->%p\n", head);
		//	printf("table address--->%p\n", &table);
	}

}

void insertel(struct freeptrs *fp) {
	struct freeptrs *temp;
	temp = head;

	if (head == NULL) {
		create_lookup();
		table = memcpy(table, fp, sizeof(struct freeptrs))+ sizeof(fp);

		//head = fp;
	} else {
		while (temp->next != NULL) {
			temp = temp->next;
		}
		printf("key------->%p\n", fp->data->key);
		table = memcpy(table, fp, sizeof(struct freeptrs))+ sizeof(fp);
		fp=table;
		temp->next = fp;

	}
}
void removel(int dt) {
	struct freeptrs *temp;
	temp = head;

	//if head node
	if ((int *) temp->data == dt) {
		struct freeptrs *del;
		del = temp;
		head = temp->next;
		free(del);
	} else {
		while (temp->next != NULL) {
			if (((int*) temp->next->data) == dt) {
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
	struct spatialattr *arr_spa = malloc(sizeof(struct spatialattr));

	printf("head = %p\n", (head));
	 memcpy(temp, head, sizeof(struct freeptrs));
	 memcpy(arr_spa, temp->data, sizeof(struct spatialattr));
	//arr_spa=temp->data;
	printf("temp key = %p\n", (arr_spa->key));
	printf("temp next = %p\n", (temp->next));
	if (head == NULL) {
		printf("List is empty");
	} else {


		while (temp->next != NULL) {

			printf("base = %p\n", (temp->data->base));
			printf("bound = %p\n", (temp->data->bound));
			printf("key = %p\n", (temp->data->key));
			printf("next = %p\n", (temp->next));
			 memcpy(temp, temp->next, sizeof(struct freeptrs));

			 printf("arr_spa = %p\n", (arr_spa->key));
			//temp = temp->next;

		}

	}
	free(arr_spa);
}

void store_metadata(void* address, void* base, void * bound) {
	struct spatialattr *arr_spa = malloc(sizeof(struct spatialattr));
	arr_spa->key = address;
	arr_spa->base = base;
	arr_spa->bound = bound;
	printf("key------->%p\n", arr_spa->key);
	struct freeptrs *fp;

	fp = malloc(sizeof(struct freeptrs));

	fp->data = arr_spa;
	fp->next = NULL;
	insertel(fp);
	free(arr_spa);
	free(fp);
}
void load_metadata(void* address) {
	struct freeptrs *temp;
	temp = head;
	if (head == NULL) {
		printf("List is empty");
	} else {
		while (temp != NULL) {
			printf("value = %d\n", (int) (temp->data));
			temp = temp->next;
		}
	}
}

int main(int argc, char** argv) {

	int arr[4];
	store_metadata(&arr, &arr, sizeof(arr) + &arr);

	int x[10];
	int *p = x;
	//store_metadata(&p, &p, sizeof(int) + &p);

	int num, i;
	int result = 0;

	printf("printall\n");
	printall();

	for (i = 0; i < 2; i++) {
		//if (spatial_check(arr, arr_spa->base, arr_spa->bound, i) == 1) {
		arr[i] = rand();
//		} else {
//			printf("out of bound access to arr\n");
//			break;
//		}

		printf("arr[%d]=%d\n", i, arr[i]);
	}
	printf("enter a number\n");
	scanf("%d", &num);

	for (i = 0; i < num; i++) {
		//if (spatial_check(arr, arr_spa->base, arr_spa->bound, i) == 1) {
		result = result + arr[i];
//		} else {
//			printf("out of bound access to arr\n");
//			break;
//		}

	}

	printf("the result is %zd\n", result);

	return 0;
}

