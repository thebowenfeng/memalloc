#include "Heap.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define HEAP_SIZE 64000
#define MAX_BLOCK 10000

struct MemoryBlock {
	char free;
	intptr_t address;
	int size;
	block_t* next;
};

char heap[HEAP_SIZE] = { 0 };
block_t allocMemBlocks[MAX_BLOCK];

int blockSize = 0;

intptr_t get_max_addr() {
	assert(blockSize < MAX_BLOCK);

	intptr_t maxAddr = 0;

	for (int i = 0; i < blockSize; i++) {
		// Check if memory block and its width larger than previous
		if (allocMemBlocks[i].address + allocMemBlocks[i].size > maxAddr) {
			// Set max address to next available address
			maxAddr = allocMemBlocks[i].address + allocMemBlocks[i].size;
		}
	}

	return maxAddr;
}

int get_block_size(intptr_t addr) {
	int size = 0;

	for (int i = 0; i < blockSize; i++) {
		block_t* currBlock = &(allocMemBlocks[i]);

		if (currBlock->address == addr) {
			// Traverse the memory block chain
			do {
				size += currBlock->size;
				currBlock = currBlock->next;
			} while (currBlock != NULL);

			return size;
		}
	}

	return 0;
}

intptr_t heap_malloc(char byteArray[], int size) {
	assert(blockSize < MAX_BLOCK);

	block_t* startBlock = NULL;
	block_t* prevBlock = NULL;
	int arrayPtr = 0;
	int hasStarted = 0;

	// Iterate through all existing memory blocks
	for (int i = 0; i < blockSize; i++) {
		block_t* currBlock = &(allocMemBlocks[i]);

		// If current mem block is free
		if (currBlock->free) {
			// If current memory block has smaller size
			if (currBlock->size < size) {
				currBlock->free = 0; // Set current block to not free
				memcpy_s(heap + currBlock->address, currBlock->size, byteArray + arrayPtr, currBlock->size); // Write first N bytes to heap
				size -= currBlock->size; // Change left-over byte size
				arrayPtr += currBlock->size; // Increment array position

				// Check if first block in loop
				if (!hasStarted) {
					startBlock = currBlock;
					hasStarted = 1;
				}
				else {
					prevBlock->next = currBlock; // Link previous block to current block
				}

				prevBlock = currBlock; // Set prev block to current block
			}
			// If current memory block has bigger size
			else if (currBlock->size > size) {
				currBlock->free = 0; // Set current block to not free

				// Check if start block is null
				if (startBlock == NULL) {
					startBlock = currBlock;
				}
				// Check if there is a previous block
				else if (prevBlock != NULL) {
					prevBlock->next = currBlock; // Link prev block to current block
				}

				memcpy_s(heap + currBlock->address, size, byteArray + arrayPtr, size); // Write remaining bytes

				// "Split" original block into a new block
				block_t newBlock;
				newBlock.free = 1; // Set new block to free
				newBlock.address = currBlock->address + size; // Address set to first byte of leftover space
				newBlock.size = currBlock->size - size; // Size set to leftover space size
				newBlock.next = NULL;
				allocMemBlocks[blockSize++] = newBlock; // Append new block to all mem blocks

				currBlock->size = size; // Reduce size
				currBlock->next = NULL; // Set next to NULL

				return startBlock->address;
			}
			// If current memory block is equal in size
			else {
				currBlock->free = 0; // Set current block to not free
				memcpy_s(heap + currBlock->address, size, byteArray, size); // Write memory to heap
				currBlock->next = NULL; // Nullify chain

				return currBlock->address; // Return address
			}
		}
	}

	block_t newBlock;
	newBlock.free = 0; // Set block to not free
	newBlock.address = get_max_addr();
	newBlock.size = size;
	newBlock.next = NULL;
	allocMemBlocks[blockSize++] = newBlock;

	memcpy_s(heap + newBlock.address, size, byteArray + arrayPtr, size); // Write remaining bytes to new block

	// Check if start block is null
	if (startBlock == NULL) {
		startBlock = &(allocMemBlocks[blockSize - 1]);
	}
	// Check if there is a previous block
	else if (prevBlock != NULL) {
		prevBlock->next = &(allocMemBlocks[blockSize - 1]); // Link prev block to current block
	}

	return startBlock->address;
}

void heap_free(intptr_t address) {
	assert(blockSize < MAX_BLOCK);

	for (int i = 0; i < blockSize; i++) {
		block_t* currBlock = &(allocMemBlocks[i]);

		if (currBlock->address == address && currBlock->free == 0) {
			while (currBlock != NULL) {
				currBlock->free = 1; // Set memory block to free
				memset(heap + currBlock->address, 0, currBlock->size); // Nullify the bytes
				currBlock = currBlock->next; // Set current to next mem block in chain
			} 
			return;
		}
	}

	return;
}

void dump_heap(int size) {
	/*
	Just prints out the heap nothing special
	*/
	for (int i = 0; i < size; i++) {
		printf(" %c |", heap[i]);
	}
	printf("\n");
}

int read_mem(intptr_t dst, char buffer[], int size) {
	assert(get_block_size(dst) !=0);
	assert(size >= get_block_size(dst));
	
	int posPtr = 0;

	for (int i = 0; i < blockSize; i++) {
		block_t* currBlock = &(allocMemBlocks[i]);

		if (currBlock->address == dst) {
			// Traverse the memory block chain
			do {
				memcpy_s(buffer + posPtr, currBlock->size, heap + currBlock->address, currBlock->size); // Copies memory from heap to buffer
				posPtr += currBlock->size;
				currBlock = currBlock->next;
			} while (currBlock != NULL);

			return 1;
		}
	}

	return 0;
}

int write_mem(intptr_t dst, char buffer[], int srcSize) {
	assert(get_block_size(dst) != 0);
	assert(srcSize <= get_block_size(dst));

	int posPtr = 0;

	for (int i = 0; i < blockSize; i++) {
		block_t* currBlock = &(allocMemBlocks[i]);

		if (currBlock->address == dst) {
			// Traverse the memory block chain
			do {
				memcpy_s(heap + currBlock->address, currBlock->size, buffer + posPtr, min(currBlock->size, srcSize)); // Write mem from buffer to heap
				posPtr += currBlock->size;
				srcSize -= currBlock->size;
				currBlock = currBlock->next;
			} while (currBlock != NULL && srcSize > 0);

			return 1;
		}
	}

	return 0;
}
