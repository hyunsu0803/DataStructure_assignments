#define _CRT_SECURE_NO_WARNINGS 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>
typedef struct TreeNode* TreePtr;
typedef struct TreeNode TreeNode;
struct TreeNode{
	int data;
	TreePtr left, right;
};
TreePtr InsertNode(int x, TreePtr root);
TreePtr DeleteNode(int x, TreePtr root);
TreePtr FindNode(int x, TreePtr root);
TreePtr FindParent(int x, TreePtr root);
void PrintInorder(TreePtr root, FILE* output);
void PrintPreorder(TreePtr root, FILE* output);
void PrintPostorder(TreePtr root, FILE* output);
void DeleteTree(TreePtr root);
TreePtr makeNode();

int main() {
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");

	//make header node
	TreePtr root = makeNode();
	root->data = INT_MAX;

	char command = 0;
	while (fscanf(input, " %c", &command) != EOF) {
		//insertion
		if (command == 'i') {
			int x; fscanf(input, "%d", &x);
			TreePtr temp = InsertNode(x, root);
			if ((temp) == NULL) {
				fprintf(output, "%d already exists.\n", x);
			}
		}
		//search
		else if (command == 'f') {
			int x; fscanf(input, "%d", &x);
			if (FindNode(x, root)) {
				fprintf(output, "%d is in the tree.\n", x);
			}
			else {
				fprintf(output, "%d is not in the tree.\n", x);
			}
		}
		//deletion
		else if (command == 'd') {
			int x; fscanf(input, "%d", &x);
			if (DeleteNode(x, root) == NULL) {
				fprintf(output, "Deletion failed. %d does not exist.\n", x);
			}
		}
		//print tree
		else if (command == 'p') {
			char cmd; fscanf(input, "%c", &cmd);
			if (cmd == 'i') {
				fprintf(output, "pi - ");
				PrintInorder(root, output);
				fprintf(output, "\n");
			}
			else if (cmd == 'r') {
				fprintf(output, "pr - ");
				PrintPreorder(root, output);
				fprintf(output, "\n");
			}
			else if (cmd == 'o') {
				fprintf(output, "po - ");
				PrintPostorder(root, output);
				fprintf(output, "\n");
			}
		}
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
		if (node->data != INT_MAX)
			fprintf(output, "%d ", node->data);
		PrintInorder(node->right, output);
	}
}
void PrintPreorder(TreePtr root, FILE* output) {
	TreePtr node = root;
	if (node) {
		if (node->data != INT_MAX)
			fprintf(output, "%d ", node->data);
		PrintPreorder(node->left, output);
		PrintPreorder(node->right, output);
	}
}
void PrintPostorder(TreePtr root, FILE* output) {
	TreePtr node = root;
	if (node) {
		PrintPostorder(node->left, output);
		PrintPostorder(node->right, output);
		if (node->data != INT_MAX)
			fprintf(output, "%d ", node->data);
	}
}
TreePtr makeNode() {
	TreePtr node = (TreePtr)malloc(sizeof(TreeNode));
	node->left = NULL;
	node->right = NULL;
	node->data = 0;

	return node;
}
TreePtr InsertNode(int x, TreePtr root) {
	TreePtr p = root;
	while (1) {
		if (x < p->data) {
			if (p->left == NULL) {
				p->left = makeNode();
				p->left->data = x;
				return p->left;
			}
			else {
				p = p->left;
				continue;
			}
		}
		else if (x > p->data) {
			if (p->right == NULL) {
				p->right = makeNode();
				p->right->data = x; 
				return p->right;
			}
			else {
				p = p->right;
				continue;
			}
		}
		else { // x == p->data
			return NULL;
		}
	}
}
TreePtr FindNode(int x, TreePtr root) {
	TreePtr p = root;
	while (1) {
		if (x < p->data) {
			if (p->left == NULL) {
				return NULL;
			}
			else {
				p = p->left;
				continue;
			}
		}
		else if (x > p->data) {
			if (p->right == NULL) {
				return NULL;
			}
			else {
				p = p->right;
				continue;
			}
		}
		else { // x == p->data
			return p;
		}
	}
}
TreePtr FindParent(int x, TreePtr root) {
	TreePtr p = root;
	TreePtr B; // parent of p

	while (1) {
		if (x < p->data) {
			if (p->left == NULL) {
				return NULL;
			}
			else {
				B = p;
				p = p->left;
				continue;
			}
		}
		else if (x > p->data) {
			if (p->right == NULL) {
				return NULL;
			}
			else {
				B = p;
				p = p->right;
				continue;
			}
		}
		else { // x == p->data
			return B;
		}
	}
}
TreePtr DeleteNode(int x, TreePtr root) {
	if (FindNode(x, root) == NULL) return NULL;

	TreePtr P = FindNode(x, root); //P will be deleted
	TreePtr B = FindParent(x, root); //Before P 
	TreePtr temp; //Replacement of P

	//if P has no child
	if(!(P->left) && !(P->right)) {
		free(P); //just free P
		P = NULL;
		return B;
	}
	//if P has two child
	else if (P->left && P->right) {
		temp = P->left;
		while (1) {
			if (temp->right == NULL) { //temp is the Max child of left subtree of P
				TreePtr Parent_of_temp = FindParent(temp->data, root);
				if (Parent_of_temp->left == temp) Parent_of_temp->left = temp->left;
				else Parent_of_temp->right = temp->left;

				if (B->left == P) B->left = temp;
				else B->right = temp;
				temp->left = P->left;
				temp->right = P->right;

				free(P);
				return temp;
			}
			else {
				temp = temp->right;
			}
		}
	}
	//if P has one child
	else {
		//temp is child of P
		if (P->left) temp = P->left;
		else temp = P->right;
		//link temp to B
		if (B->left == P) B->left = temp;
		else B->right = temp;

		free(P); //free P
		return temp;
	}
}
