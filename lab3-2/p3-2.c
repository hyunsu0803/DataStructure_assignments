#include<stdio.h>
#include<string.h>
#define MAX_QUEUE_SIZE 100
int enQ(int x);
int deQ();

int queue[MAX_QUEUE_SIZE] = { 0 };
int front = 0;
int rear = -1;
int queue_size = 0;
int main(void) {
	FILE* input = fopen("input3-2.txt", "r");
	FILE* output = fopen("output3-2.txt", "w");

	int op;
	fscanf(input, "%d", &op);
	while (op--) {
		char oper[5];
		fscanf(input, "%s", oper);

		if (strcmp(oper, "enQ") == 0) {
			int x;
			fscanf(input, "%d", &x);

			if (enQ(x)) 
				fprintf(output, "%d\n", x);
			else fprintf(output, "FULL\n");
		}
		else {
			if (deQ() == 0) fprintf(output, "Empty\n");
		}
	}

	fclose(input);
	fclose(output);
}

int enQ(int x) {
	if (queue_size == MAX_QUEUE_SIZE) return 0; //q is full

	if (rear == MAX_QUEUE_SIZE - 1) {
		rear = 0;
		queue[rear] = x;
	}
	else {
		queue[++rear] = x;
	}

	queue_size++;
	return 1; //enqueue success
}
int deQ() {
	if (queue_size == 0) return 0; //q is empty

	if (front == MAX_QUEUE_SIZE - 1)
		front = 0;
	else front++;

	queue_size--;
	return 1; //dequeue success
}