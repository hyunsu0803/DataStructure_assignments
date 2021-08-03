#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Type Definition
typedef struct Node* PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

typedef struct {
	int studentID;
	char* studentName;
}ElementType;

struct Node {
	ElementType element;
	PtrToNode next;
};

//Function Declaration
int Insert(ElementType x, List L);
int Delete(int id, List L);
Position Find(int id, List L);
void PrintList(List L, FILE* output);
void PrintAll(List L, FILE* output);
int IsEmpty(List L);
void DeleteList(List L);

int main() {
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");
	List L = (List)malloc(sizeof(struct Node)); 
	L->next = NULL; 

	char mode = 0;

	while (fscanf(input, "%c", &mode) != EOF) {
		if(mode == 'i'){
			//variable setting for Insert
			int ID;
			char name[30];
			ElementType student;
			student.studentName = (char*)malloc(sizeof(char)*30);

			//reading from the input file
			fscanf(input, "%d", &ID);
			fgets(name, 30, input);
			name[strlen(name) - 1] = 0;

			//make a student
			student.studentID = ID;
			strcpy(student.studentName, name);

			//insert student to the list
			if (Insert(student, L) == 1) {
				fprintf(output, "Insertion Success : %d\n", ID);
				PrintList(L, output);
			}
			else {
				fprintf(output, "Insertion Failed. ID %d already exists.\n", ID);
			}
		}
		else if (mode == 'd') {
			int ID;
			fscanf(input, "%d", &ID);
			if (Delete(ID, L)) {
				fprintf(output, "Deletion Success : %d\n", ID);
				PrintList(L, output);
			}
			else {
				fprintf(output, "Deletion Failed : Student ID %d is not in the list.\n", ID);
			}
		}
		else if (mode == 'f') {
			int ID;
			fscanf(input, "%d", &ID);

			Position P = Find(ID, L);
			if (P) {
				fprintf(output, "Find Success : %d %s\n", P->element.studentID, P->element.studentName);
			}
			else {
				fprintf(output, "Find %d Failed. There is no student ID\n", ID);
			}
		}
		else if (mode == 'p') {
			PrintAll(L, output);
		}
	}
	DeleteList(L);
	fclose(input);
	fclose(output);
}
int Insert(ElementType x, List L) {
	PtrToNode node = (PtrToNode)malloc(sizeof(struct Node));
	node->element.studentID = x.studentID;
	node->element.studentName = (char*)malloc(sizeof(char) * 30);
	strcpy(node->element.studentName, x.studentName);
	node->next = NULL;

	if (IsEmpty(L)) {
		L->next = node;
		return 1;
	}
	else {
		Position P = L; //actually we see P->next
		while (P->next != NULL) {
			if (x.studentID == P->next->element.studentID) { //duplicate
				free(x.studentName);
				return 0;
			}
			else if (x.studentID < P->next->element.studentID) { //insert
				node->next = P->next;
				P->next = node;
				return 1;
			}
			else {
				P = P->next;
			}
		}
		//if there is no studentID bigger than new node : 
		P->next = node; //new node be the last node
		return 1;
	}
	
}
int IsEmpty(List L) {
	if (L->next == NULL) return 1;
	else return 0;
}
void PrintList(List L, FILE* output) {
	Position P = L;
	fprintf(output, "Current List > ");
	while (P->next != NULL) {
		int id = P->next->element.studentID;
		char* name = P->next->element.studentName;

		fprintf(output , "%d%s ", id, name);
		P = P->next;
	}
	fprintf(output, "\n");
}
int Delete(int id, List L) {
	Position P = L;
	PtrToNode del;
	while (P->next != NULL) {
		if (P->next->element.studentID == id) {
			del = P->next;

			free(del->element.studentName);
			P->next = del->next;
			del->next = NULL;
			free(del);
			return 1;
		}
		P = P->next;
	}
	return 0;
}
Position Find(int id, List L) {
	Position P = L->next;

	while (P != NULL) {
		if (P->element.studentID == id) {
			return P;
		}
		P = P->next;
	}
	return NULL;
}
void PrintAll(List L, FILE* output) {
	Position P = L->next;

	fprintf(output, "-------List-------\n");
	while (P != NULL) {
		fprintf(output, "%d %s\n", P->element.studentID, P->element.studentName);
		P = P->next;
	}
	fprintf(output, "------------------\n");
}
void DeleteList(List L) {
	Position P = L->next;
	while (1) {
		free(L);
		if (P == NULL) break;
		L = P->next;
		free(P);
		if (L == NULL) break;
		P = L->next;
	}
}