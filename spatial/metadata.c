/*
 * metadata.c
 *
 *  Created on: Jun 28, 2015
 *      Author: priyanka
 */
#include<stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#define TRIE_PRIMARY_TABLE_ENTRIES (size_t)(8*1024*1024)
#define TRIE_SEC_TABLE_ENTRIES (size_t)(8*1024*1024)
int secondary_table_exists = 0;

struct metadata_entry {
	void *base;
	void *bound;
};

struct metadata_entry** trie_root;
void init() {
	size_t length_trie = TRIE_PRIMARY_TABLE_ENTRIES
			* sizeof(struct metadata_entry);
	trie_root = mmap(0, length_trie, PROT_READ | PROT_WRITE,
	MAP_ANON | MAP_NORESERVE | MAP_PRIVATE, -1, 0);
	if (trie_root == MAP_FAILED) {
		secondary_table_exists = 0;
	} else {
		secondary_table_exists = 1;
	}
	printf("secondary_table_exists:%d\n", secondary_table_exists);
}
struct metadata_entry *trie_allocate() {
	struct metadata_entry* secondary_entry;
	size_t length = (TRIE_SEC_TABLE_ENTRIES ) * sizeof(struct metadata_entry);
	secondary_entry = mmap(0, length, PROT_READ | PROT_WRITE,
	MAP_ANON | MAP_NORESERVE | MAP_PRIVATE, -1, 0);
	//assert(secondary_entry != (void*)-1);
	printf("snd trie table %p %lx\n", secondary_entry, length);
	return secondary_entry;
}
struct metadata_entry metadata_load(void* address_of_ptr) {
	struct metadata_entry me;
	struct metadata_entry *secondary_table;
	size_t ptr = (size_t) (address_of_ptr);
//	size_t primary_index = ptr >> 25;
//	size_t secondary_index = ((ptr >> 3) & 0x3fffff);

	size_t primary_index = ptr >> 26;
			size_t secondary_index = ((ptr >> 3) & 0x7fffff);
	secondary_table = trie_root[primary_index];
	me = secondary_table[secondary_index];
	return me;
}
int spatial_check(void *p, void* p_base, void* p_bound, size_t size) {
	printf("pointer:%p\n",(void*) ((char*) p + size));
	if ((p < p_base) || ((void*) ((char*) p + size) >= p_bound)) {
		return 0;
	}
	return 1;
}
void metadata_store(void* address_of_ptr, void* base,void* bound) {
	struct metadata_entry *me;
	struct metadata_entry *secondary_table;
	size_t ptr = (size_t) (address_of_ptr);
//	size_t primary_index = ptr >> 25;
//	size_t secondary_index = ((ptr >> 3) & 0x3fffff);
	size_t primary_index = ptr >> 26;
		size_t secondary_index = ((ptr >> 3) & 0x7fffff);
	secondary_table = trie_root[primary_index];
	if (secondary_table == NULL) {
		secondary_table = trie_allocate();
		trie_root[primary_index] = secondary_table;
	}
	me = &secondary_table[secondary_index];

	me->base = base;
	me->bound = bound;
}
//int main() {
//	init();
//	int x[4];
//	int *p = x;
//	printf("pointer:%p\naddress:%p\nbase:%p\nbound:%p\n", p, &p, p, p + 4);
//	metadata_store(&p, p, p + 4);
//	struct metadata_entry me;
//	me=metadata_load(&p);
//	printf("base:%p\nbound:%p\n", me.base, me.bound);
//	return 0;
//}
