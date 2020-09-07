#include <stdio.h>
#include <assert.h>

#define STORAGE_SIZE 16

static char memory[STORAGE_SIZE];
static short int mapping[STORAGE_SIZE];
static int refCounter = 1; 

void* sim_calloc(size_t n, size_t size);
void dumpMapping();
void sim_free(void* ptr);
void __unmap(int pos, int id);

int main() {
	dumpMapping();
	void* first = sim_calloc(3, 2);
	void* second = sim_calloc(2, 2);
	dumpMapping();
	sim_free(first);
	dumpMapping();
	void* third = sim_calloc(1, 2);
	dumpMapping();
	sim_free(second);
	sim_free(third);
	dumpMapping();
	// Store tests

	int* a = sim_calloc(2, sizeof(int));
	printf("a alloc: ");
	dumpMapping();

	a[0] = 10;
	a[1] = -121;

	char* b = sim_calloc(1, sizeof(char));

	printf("b alloc: ");
	dumpMapping();
	
	b[0] = '\0';
	assert(b[0] == '\0');

	sim_free(b);

	printf("b free: ");
	dumpMapping();

	assert(a[0] == 10);
	assert(a[1] == -121);
	
}

void dumpMapping(){
	for (unsigned i = 0; i < STORAGE_SIZE; i++){
		printf("|%2d|", mapping[i]);
	}
	printf("\n");
}

void* sim_calloc(size_t n, size_t size) {
	char* start = memory;
	char* end = memory + size * n - 1;
	while (1) {
		printf("%d => %d %d;\n", refCounter, start - memory, end - memory);
		if (end >= memory + STORAGE_SIZE)
			return NULL;
		if (end == start && mapping[end - memory] == 0) {
			start = end - size * n + 1;
			for (char* i = start; i <= end; i++){
				mapping[i - memory] = refCounter;
				printf("%d- ", i - memory);
				*(i) = 0;
			}
			refCounter++;
			printf("Final: %d\n", start - memory);
			return (void*)(start);
		}
		if (mapping[end - memory] > 0) {
			end++;
			start = end - size * n + 1;
			continue;
		}

		if (mapping[start - memory] > 0) {
			start++;
			end = start + size * n - 1;
			continue;
		}
		start++;
	}
}

void sim_free(void* ptr){
	if (ptr == NULL){
		printf("NULL pointer found\n");
		return;
	}
	char* start = ptr;
	int pos = start - memory;
	printf("Free pos: %d\n", pos);
	int refId = mapping[pos];
	if (refId == 0){
		printf("Memory at this pointer is already free!\n");
		return;
	}
	if (pos == 0){
		__unmap(0, refId);
		return;
	}

	if (mapping[pos - 1] == mapping[pos]){
		printf("Pointer is not at the beginning of the block!\n");
		return;
	}

	__unmap(pos, refId);
}

void __unmap(int pos, int id){
	while (mapping[pos] == id && pos < STORAGE_SIZE){
		mapping[pos++] = 0;
	}
}
