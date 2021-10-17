#include <stdio.h>
#include "Heap.h"

void print_buffer(char buffer[], int size) {
	printf("Buffer: ");

	for (int i = 0; i < size; i++) {
		printf(" %c |", buffer[i]);
	}

	printf("\n");
}

int main() {
	char data[5] = { 'a', 'b', 'c', 'd', 'e' };
	char data2[7] = { 'f', 'g', 'h', 'i', 'j', 'k', 'l' };
	char data3[3] = { 'm', 'n', 'o' };

	printf("Address: %d\n", heap_malloc(data, 5));
	dump_heap(30);

	printf("Address: %d\n", heap_malloc(data, 5));
	dump_heap(30);

	heap_free(0);

	printf("Address: %d\n", heap_malloc(data3, 3));
	dump_heap(30);

	printf("Address: %d\n", heap_malloc(data2, 7));
	dump_heap(30);

	heap_free(0);
	heap_free(3);

	printf("Address: %d\n", heap_malloc(data2, 7));
	dump_heap(30);

	printf("Address: %d\n", heap_malloc(data3, 3));
	dump_heap(30);

	heap_free(0);

	printf("Address: %d\n", heap_malloc(data3, 3));
	dump_heap(30);

	printf("Address: %d\n", heap_malloc(data3, 3));
	dump_heap(30);

	printf("Address: %d\n", heap_malloc(data, 5));
	dump_heap(30);

	heap_free(3);
	heap_free(12);

	printf("Address: %d\n", heap_malloc(data2, 7));
	dump_heap(30);

	char buffer[7];
	read_mem(11, buffer, 7);
	print_buffer(buffer, 7);

	char buffer2[3] = { 'z', 'x', 'y' };
	write_mem(11, buffer2, 3);

	read_mem(11, buffer, 7);
	print_buffer(buffer, 7);

	char buffer3[5] = { 'b', 'r', 'u', 'h', 'h' };
	write_mem(11, buffer3, 5);

	read_mem(11, buffer, 7);
	print_buffer(buffer, 7);

	return 0;
}