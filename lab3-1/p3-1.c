#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node* PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

struct Node {
	int val;
	PtrToNode next;
};

int POP(List L);
int PUSH(int val, List L);
void PrintStack(List L, FILE* output);
void DeleteAll(List L);

int main(void) {
	FILE* input = fopen("input3-1.txt", "r");
	FILE* output = fopen("output3-1.txt", "w");

	List Stack;
	Stack = (List)malloc(sizeof(struct Node));
	Stack->next = NULL;

	int optime = 0;
	fscanf(input, "%d", &optime);
	while (optime--) {
		char oper[5];
		fscanf(input, "%s", oper);

		if (strcmp(oper, "push") == 0) {
			int val;
			fscanf(input, "%d", &val);
			
			PUSH(val, Stack);

			PrintStack(Stack, output);
		}

		else {
			if (POP(Stack)==0) {
				fseek(output, 0, SEEK_END);
				fprintf(output, "Empty\n");
			}
		}
	}
	fclose(input);
	fclose(output);
	DeleteAll(Stack);
}
int POP(List L) {
	if (L->next == NULL) {
		return 0;
	}
	PtrToNode popCell = L->next;
	L->next = popCell->next;
	free(popCell);
	return 1;
}

int PUSH(int val, List L) {
	PtrToNode newCell = (PtrToNode)malloc(sizeof(struct Node));

	newCell->val = val;
	newCell->next = L->next;
	L->next = newCell;

	return 1;
}

void DeleteAll(List L) {
	if (L->next == NULL) {
		free(L);
	}
	else {
		PtrToNode P = L->next;
		while (1) {
			
			free(L);
			if (P == NULL) break;
			L = P->next;
			
			free(P);
			if (L == NULL) break;
			P = L->next;
		}
	}
}

void PrintStack(List L, FILE* output) {
	fseek(output, 0, SEEK_SET);
	Position P = L->next;

	while (P != NULL) {
		fprintf(output, "%d\n", P->val);
		P = P->next;
	}
}