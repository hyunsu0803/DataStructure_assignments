#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

typedef struct HashTbl* HashTable;
struct HashTbl {
	int TableSize;
	int * list;
};

HashTable makeTable(int size);
void Insert(HashTable h, int value, int solution, FILE* output);
void Delete(HashTable h, int value, int solution, FILE* output);
int Find(HashTable h, int value, int solution);
void Print(HashTable h, FILE* output);
int Hash(int value, int size, int i, int solution);

int main(void) {
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");

	int T;
	fscanf(input, "%d", &T);

	while (T--) {
		int solution;
		char sol[20] = { 0 };
		fgetc(input);
		fscanf(input, "%s", sol);
		
		if (!strcmp(sol, "Linear")) solution = 1;
		else if (!strcmp(sol, "Quadratic")) solution = 2;
		else if (!strcmp(sol, "Double")) solution = 3;

		fprintf(output, "%s\n", sol);
		int tableSize;
		fscanf(input, "%d", &tableSize);
		HashTable h = makeTable(tableSize);

		char command = 0;
		while (1) {
			fscanf(input, " %c", &command);
			if (command == 'i') {
				int x;
				fscanf(input, "%d", &x);
				Insert(h, x, solution, output);
			}
			else if (command == 'd') {
				int x;
				fscanf(input, "%d", &x);
				Delete(h, x, solution, output);
			}
			else if (command == 'f') {
				int x; 
				fscanf(input, "%d", &x);

				int idx = Find(h, x, solution);
				if (idx != -1) fprintf(output, "%d\n", idx);
				else fprintf(output, "Not found\n");
			}
			else if (command == 'p') {
				Print(h, output);
			}
			else if (command == 'q') {
				fprintf(output, "\n");
				break;
			}
		}
	}

	fclose(input);
	fclose(output);
}

HashTable makeTable(int size) {
	HashTable h = (HashTable)malloc(sizeof(struct HashTbl));
	h->TableSize = size;
	h->list = (int*)malloc(sizeof(int)*size);
	
	for (int i = 0; i < size; i++) {
		h->list[i] = 0;
	}
	return h;
}
void Insert(HashTable h, int value, int solution, FILE* output) {
	if (Find(h, value, solution) != -1) {
		fprintf(output, "Already exists\n");
		return;
	}
	int i = 0;
	while(h->list[Hash(value, h->TableSize, i, solution)] > 0) {
		i++;
		if (i >= h->TableSize) return;
	}
	h->list[Hash(value, h->TableSize, i, solution)] = value;
	fprintf(output, "Inserted %d\n", value);
}
void Delete(HashTable h, int value, int solution, FILE* output) {
	if (Find(h, value, solution) == -1) {
		fprintf(output, "%d not exists\n", value);
		return;
	}
 
	h->list[Find(h, value, solution)] = -1;
	fprintf(output, "Deleted %d\n", value);
}
int Find(HashTable h, int value, int solution) {
	int i = 0; 
	while(h->list[Hash(value, h->TableSize, i, solution)] != value
		&& h->list[Hash(value, h->TableSize, i, solution)] != 0) {
		i++;
		if (i >= h->TableSize) return -1;
	}

	if (h->list[Hash(value, h->TableSize, i, solution)] == value) {
		return Hash(value, h->TableSize, i, solution);
	}
	else return -1;
}
void Print(HashTable h, FILE* output) {
	for (int i = 0; i < h->TableSize; i++) {
		if (h->list[i] > 0) {
			fprintf(output, "%d ", h->list[i]);
		}
		else {
			fprintf(output, "0 ");
		}
	}
	fprintf(output, "\n");
}
int Hash(int value, int size, int i, int solution) {
	if (solution == 1) {
		return value % size + i % size;
	}
	else if (solution == 2) {
		return (value % size + i * i) % size;
	}
	else if (solution == 3) {
		return (value % size + (7 - value % 7) * i) % size;
	}
}