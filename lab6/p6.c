#define _CRT_SECURE_NO_WARNINGS 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>

typedef struct TreeNode* TreePtr;
typedef struct TreeNode* Position;
typedef struct TreeNode TreeNode;
struct TreeNode{
	int data;
	TreePtr left, right;
	int height;
};
void PrintInorder(TreePtr root, FILE* output);
void DeleteTree(TreePtr root);
TreePtr makeNode(int x);

int Height(Position P);
int Max(int a, int b);
Position SingleRotateWithLeft(Position A);
Position SingleRotateWithRight(Position A);
Position DoubleRotateWithLeft(Position A);
Position DoubleRotateWithRight(Position A);
TreePtr Insert(int x, TreePtr T);

int err; 

int main() {
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");

	TreePtr root = NULL;

	int x;
	while (fscanf(input, "%d", &x) != EOF) {
		root = Insert(x, root);
		if (err) {
			fprintf(output, "%d already in the tree!\n", x);
		}
		else {
			PrintInorder(root, output);
			fprintf(output, "\n");
		}
		err = 0;
	}

	DeleteTree(root);

	fclose(input);
	fclose(output);
}
void DeleteTree(TreePtr root) {
	TreePtr node = root;
	if (node) {
		DeleteTree(node->left);
		DeleteTree(node->right);
		free(node);
	}
}
void PrintInorder(TreePtr root, FILE* output) {
	TreePtr node = root;
	if (node) {
		PrintInorder(node->left, output);
		fprintf(output, "%d(%d) ", node->data, node->height);
		PrintInorder(node->right, output);
	}
}
TreePtr makeNode(int x) {
	TreePtr node = (TreePtr)malloc(sizeof(TreeNode));
	node->left = NULL;
	node->right = NULL;
	node->data = x;
	node->height = 0;

	return node;
}
int Height(Position P) { // not to make a null node
	if (P == NULL) return -1;
	else return P->height;
}
int Max(int a, int b) {
	if (a >= b) return a;
	else return b;
}
Position SingleRotateWithLeft(Position A) { // LL
	Position B;
	B = A->left;
	A->left = B->right;
	B->right = A;

	A->height = Max(Height(A->left), Height(A->right)) + 1;
	B->height = Max(Height(B->left), Height(B->right)) + 1;

	return B; // new root
}
Position SingleRotateWithRight(Position A) { // RR
	Position B;
	B = A->right;
	A->right = B->left;
	B->left = A;

	A->height = Max(Height(A->left), Height(A->right)) + 1;
	B->height = Max(Height(B->left), Height(B->right)) + 1;

	return B; // new root
}
Position DoubleRotateWithLeft(Position A) { // LR
	A->left = SingleRotateWithRight(A->left);
	return SingleRotateWithLeft(A);
}
Position DoubleRotateWithRight(Position A) {
	A->right = SingleRotateWithLeft(A->right);
	return SingleRotateWithRight(A);
}
TreePtr Insert(int x, TreePtr T) {
	if (T == NULL) 
		T = makeNode(x);
	else if (x == T->data) {
		err = 1;
		return T;
	}

	else if (x < T->data) { // left insert
		T->left = Insert(x, T->left);

		// if height is unbalanced
		if (Height(T->left) - Height(T->right) == 2) {
			if (x < T->left->data) // LL
				T = SingleRotateWithLeft(T);
			else // LR
				T = DoubleRotateWithLeft(T);
		}
	}

	else if (x > T->data) { // right insert
		T->right = Insert(x, T->right);

		// if height is unbalanced
		if (Height(T->right) - Height(T->left) == 2) {
			if (x > T->right->data) // RR
				T = SingleRotateWithRight(T);
			else // RL
				T = DoubleRotateWithRight(T);
		}
	}

	T->height = Max(Height(T->left), Height(T->right)) + 1;
	return T;
}