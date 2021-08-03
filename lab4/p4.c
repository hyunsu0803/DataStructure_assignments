#define _CRT_SECURE_NO_WARNINGS 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct threaded_tree* threaded_ptr;
typedef struct threaded_tree threaded_tree;
struct threaded_tree{
	short int left_thread;
	threaded_ptr left_child;
	char data;
	threaded_ptr right_child;
	short int right_thread; 
};
threaded_ptr insucc(threaded_ptr tree); //find my inorder successor
void InsertNode(threaded_ptr parent, threaded_ptr child);
void tinorder(threaded_ptr tree, FILE* output);
threaded_ptr makeNode();
void linkTree(threaded_ptr* arr, threaded_ptr tree, int number_of_node);

int main() {
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");

	int number_of_node = 0; 
	fscanf(input, "%d", &number_of_node);

	// make array of nodes
	threaded_ptr* arr = (threaded_ptr*)malloc(sizeof(threaded_ptr) * (number_of_node+1));

	// make header node
	threaded_ptr tree = makeNode();
	tree->right_child = tree;
	arr[0] = tree;
	tree->data = '$';
	tree->left_thread = 0;
	tree->right_thread = 0;

	// make left node
	for (int i = 1; i <= number_of_node; i++) {
		char data;
		fscanf(input, " %c", &data);
		arr[i] = makeNode();
		arr[i]->data = data;
	}

	// link the nodes
	linkTree(arr, tree, number_of_node);
	
	//print inorder traversal
	tinorder(tree, output);

	fclose(input);
	fclose(output);
	for (int i = 0; i <= number_of_node; i++) 
		free(arr[i]);
	free(arr);
}
void tinorder(threaded_ptr tree, FILE* output) {
	threaded_ptr temp = tree;
	while (1) {
		temp = insucc(temp);
		if (temp == tree) break;
		fprintf(output, "%c ", temp->data);
	}
}
threaded_ptr insucc(threaded_ptr node) { //find my inorder successor
	threaded_ptr temp = node->right_child;

	if (!node->right_thread)
		while (!temp->left_thread)
			temp = temp->left_child;

	return temp;
}
threaded_ptr makeNode() {
	threaded_ptr node = (threaded_ptr)malloc(sizeof(threaded_tree));
	node->left_thread = 1; node->right_thread = 1;
	node->left_child = NULL; node->right_child = NULL;
	node->data = 0;
	return node;
}
void InsertNode(threaded_ptr parent, threaded_ptr child) 
{
	if (!parent->left_child) {
		parent->left_child = child;
		parent->left_thread = 0;
	}
	else if (!parent->right_child) {
		parent->right_child = child;
		parent->right_thread = 0;
	}
}
void linkTree(threaded_ptr* arr, threaded_ptr tree, int number_of_node) {

	//link each other
	InsertNode(tree, arr[1]);
	for (int i = 1; i <= number_of_node; i++) {
		int l_child_idx = 2 * i;
		int r_child_idx = 2 * i + 1;
		if (l_child_idx <= number_of_node)
			InsertNode(arr[i], arr[l_child_idx]); //link child to left child of parent
		if (r_child_idx <= number_of_node)
			InsertNode(arr[i], arr[r_child_idx]); //link child to right child of parent 

		if (l_child_idx > number_of_node && r_child_idx > number_of_node)
			break;
	}

	// thread
	for (int i = number_of_node; i >= 1; i--) {
		// if arr[i] do not have any thread than break;
		if (arr[i]->left_thread == 0 && arr[i]->right_thread == 0)
			break;

		//finding left thread
		if (arr[i]->left_thread == 1) {
			int idx = i; //idx is the left thread we are finding
			while (1) {
				if (idx % 2 == 1) {
					idx = idx / 2;
					arr[i]->left_child = arr[idx];
					break;
				}
				idx = idx / 2;
			}
		}
		//finding right thread
		if (arr[i]->right_thread == 1) {
			int idx = i;
			while (1) {
				if (idx % 2 == 0) {
					idx = idx / 2;
					arr[i]->right_child = arr[idx];
					break;
				}
				idx = idx / 2;
			}
		}
	}
}