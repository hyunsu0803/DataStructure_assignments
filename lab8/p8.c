#include <stdio.h>
#include <stdlib.h>
struct HeapStruct {
	int capacity;
	int size;
	int * Elements;
};
typedef struct HeapStruct* Heap;

Heap makeNode(int capacity);
void Insert(int x, Heap h, FILE* output);
int Find(int x, Heap h);
void Print(Heap h, FILE* output);
int main() {
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");

	int capacity;
	fscanf(input, "%d", &capacity);

	Heap h = makeNode(capacity);

	char command;
	while (fscanf(input, " %c", &command) != EOF) {
		if (command == 'i') {
			int x;
			fscanf(input, "%d", &x);
			Insert(x, h, output);
		}
		else if (command == 'f') {
			int x; 
			fscanf(input, "%d", &x);
			if (Find(x, h)) 
				fprintf(output, "%d is in the heap\n", x);
			else fprintf(output, "%d is not in the heap\n", x);
		}
		else if (command == 'p') {
			Print(h, output);
		}
	}
}
Heap makeNode(int capacity) {
	Heap h = (Heap)malloc(sizeof(struct HeapStruct));
	h->capacity = capacity;
	h->Elements = (int*)malloc(sizeof(int)*(capacity + 1));
	h->size = 0;
	return h;
}
void Insert(int x, Heap h, FILE* output) {
	if (h->size == h->capacity)
		fprintf(output, "heap is full\n");
	else if (Find(x, h))
		fprintf(output, "%d is already in the heap\n", x);
	else {
		int i;
		h->size++; //일단 여기에 x를 넣어두고
		// i는 x의 인덱스
		// i가 root노드가 아니고, i가 부모보다 크면 자리바꾸기
		for (i = h->size; i / 2 > 0 && h->Elements[i / 2] < x; i /= 2) {
			h->Elements[i] = h->Elements[i / 2];
		}
		h->Elements[i] = x;
		fprintf(output, "insert %d\n", x);
	}
}
int Find(int x, Heap h) {
	for (int i = 1; i <= h->size; i++) {
		if (h->Elements[i] == x) {
			return 1;
		}
	}
	return 0;
}
void Print(Heap h, FILE* output) {
	for (int i = 1; i <= h->size; i++) {
		fprintf(output, "%d ", h->Elements[i]);
	}
	fprintf(output, "\n");
}
