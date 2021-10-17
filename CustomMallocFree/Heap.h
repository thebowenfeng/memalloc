#pragma once

// Type typedefs for heap malloc
typedef struct MemoryBlock block_t;
typedef int intptr_t;
typedef char heap_t[64000];
typedef block_t* memblocks_t[10000];

intptr_t heap_malloc(char byteArray[], int size);
void heap_free(intptr_t address);
void dump_heap(int size);
int read_mem(intptr_t dst, char buffer[], int size);
int write_mem(intptr_t dst, char buffer[], int srcSize);