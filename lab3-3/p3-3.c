#define _CRT_SECURE_NO_WARNINGS 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_EXP 100

typedef struct Node* PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
struct Node {
	int val;
	PtrToNode next;
};
int POP(List L);
int PUSH(int val, List L);
void DeleteAll(List L);

char infix[MAX_EXP];
int operate(FILE* output);
char Top(List L);
int is_empty(List L);
int is_number(char x);
int is_operator(char x);
int calc(int a, int b, char x);
int p = -1; //operation process (index)

int main() {
	FILE* input = fopen("input3-3.txt", "r");
	FILE* output = fopen("output3-3.txt", "w");
	int result = 0; 

	//read input file
	fgets(infix, MAX_EXP, input);
	//print infix expression except #
	fprintf(output, "Infix Form : %s", infix);
	fseek(output, -1, SEEK_END);
	//print postfix expression
	fprintf(output, "\nPostfix Form : ");
	result = operate(output);
	//print result
	fprintf(output, "\nEvaluation Result : %d", result);

	fclose(input);
	fclose(output);
}

int operate(FILE* output) {
	List Operand = (List)malloc(sizeof(struct Node));
	Operand->next = NULL;
	List Operator = (List)malloc(sizeof(struct Node));
	Operator->next = NULL;
	int result = 0; 
	
	while (1) {
		++p;
		char x = infix[p];

		//if infix[p] is number
		if (is_number(x)) { 
			int a = x - '0';
			PUSH(a, Operand); //push num x
			fprintf(output, "%d", a); //print num x
			continue;
		}

		//if infix[p] is operator
		else if (is_operator(x)) { 

			//if no operator is in stack
			if (is_empty(Operator)) {
				PUSH(x, Operator); //push op x
				continue;
			}

			//if there is an operator in stack
			else {
				// now do op x
				if (is_operator(Top(Operator)) < is_operator(x)) { 

					int a = Top(Operand); POP(Operand);
					int b;
					//if infix[p] is number
					if (is_number(infix[++p])) {
						b = infix[p] - '0';
						fprintf(output, "%d", b); 
					}
					else {//infix[p] is '(' 
						b = operate(output);
					}

					PUSH(calc(a, b, x), Operand);
					fprintf(output, "%c", x);
					continue;
				}
				// do top op first and push op x
				else {
					int b = Top(Operand); POP(Operand);
					int a = Top(Operand); POP(Operand);
					char c = Top(Operator); POP(Operator);

					PUSH(x, Operator);
					PUSH(calc(a, b, c), Operand);
					fprintf(output, "%c", c);
					continue;
				}
			}
		}
		else if (x == '(') {
			PUSH(operate(output), Operand);
			continue;
		}
		else if (x == ')' || x == '#') {
			int b = Top(Operand); POP(Operand);
			int a = Top(Operand); POP(Operand);
			char c = Top(Operator); POP(Operator);

			result = calc(a, b, c);
			fprintf(output, "%c", c);
			break;
		}
	}

	DeleteAll(Operator);
	DeleteAll(Operand);

	return result;	
}
int is_empty(List L) {
	return L->next == NULL;
}
int is_number(char x) {
	if ('1' <= x && x <= '9') 
		return 1;
	return 0;
}
int is_operator(char x) {
	if (x == '+' || x == '-')
		return 1;
	if (x == '*' || x == '/' || x == '%')
		return 2;
	return 0;
}
char Top(List L) {
	return L->next->val;
}
int calc(int a, int b, char x) {
	switch (x)
	{
	case '*':
		return a * b;
	case '/':
		return a / b;
	case '%':
		return a % b;
	case '+':
		return a + b;
	case '-':
		return a - b;
	default:
		return -100;
		break;
	}
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