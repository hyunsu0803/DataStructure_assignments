#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#define order 3

typedef struct B_node B_node;
typedef B_node* B_tree;
struct B_node {
	int n_keys;
	B_node* child[order + 2];
	int key[order + 1];
};

B_tree makeNode();
void Inorder(B_tree root, FILE* output);
void Insert(int x, B_tree root);
void sort(int* key);
void reset(B_tree node);
B_tree FindParent(B_tree X, B_tree root);

int main() {
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");

	B_tree root = makeNode();
	root->n_keys = 0; //�� ���� key�� 0

	char command;
	while (fscanf(input, "%c", &command) != EOF) {
		if (command == 'p') {
			Inorder(root, output);
			fprintf(output, "\n");
		}
		else if (command == 'i') {
			int x;
			fscanf(input, "%d ", &x);

			Insert(x, root);
		}
	}

	fclose(input);
	fclose(output);
}
B_tree makeNode() {
	B_tree node = (B_tree)malloc(sizeof(B_node));

	node->key[0] = INT_MIN;
	for (int i = 1; i <= order; i++) {
		node->key[i] = INT_MAX;
	}
	for (int i = 1; i <= order + 1; i++) 
	{
		node->child[i] = NULL;
	}
	return node;
}
void reset(B_tree node) {
	node->key[0] = INT_MIN;
	for (int i = 1; i <= order; i++) {
		node->key[i] = INT_MAX;
	}
	for (int i = 1; i <= order + 1; i++)
	{
		node->child[i] = NULL;
	}
}
void sort(int* key) {
	//order-1�� key�� ������ �ε��� 
	for (int i = 1; i <= order - 1; i++) {
		for (int j = 1; j <= order - i; j++) {
			if (key[j] > key[j + 1]) {
				int temp = key[j];
				key[j] = key[j + 1];
				key[j + 1] = temp;
			}
		}
	}
}
void Inorder(B_tree root, FILE* output) {
	if (!root) return;
	int i;
	for (i = 1; i <= root->n_keys; i++) {
		Inorder(root->child[i], output);
		fprintf(output, "%d ", root->key[i]);
	}
	Inorder(root->child[root->n_keys+1], output);
}
B_tree FindParent(B_tree X, B_tree root) {
	//leaf���� ��������
	B_node* P = root;
	B_node* Parent_of_P = NULL;

	while (1) {
		if (P == X) break;

		if (P->key[P->n_keys] < X->key[1]) {
			Parent_of_P = P;
			P = P->child[P->n_keys + 1];
			continue;
		}
		for (int k = 1; k <= P->n_keys; k++) {
			if (X->key[1] < P->key[k]) {
				Parent_of_P = P;
				P = P->child[k];
				break;
			}
		}
	}
	return Parent_of_P;
}
void Insert(int x, B_tree root) {
	if (!root->n_keys) { //ù insert���
		root->n_keys = 1;
		root->key[root->n_keys] = x;
		return;
	}

	//leaf���� ��������
	B_node* P = root;
	while (1) {
		//leaf������� �˻�
		int leaf = 1; //��� child�� NULL�̶�� leaf�� 1�� ����
		for (int c = 1; c <= P->n_keys + 1; c++) {
			if (P->child[c]) leaf = 0; 
		}

		if (leaf) break; //leaf��� ����!
		else { //���� leaf�� �ƴ϶�� �� ĭ �� ��������
			if (P->key[P->n_keys] < x) {
				P = P->child[P->n_keys + 1];
				continue;
			}
			for (int k = 1; k <= P->n_keys; k++) {
				if (x < P->key[k]) {
					P = P->child[k];
					break;
				}
			}
		}
	}

	//leaf�� �ϴ� insert
	P->n_keys++;
	P->key[P->n_keys] = x;
	sort(P->key);

	while (1) {
		if (P->n_keys < order) {
			return; //�ȳ������� insert��
		}
		else if (P == root) { //root���� ��������
			int up = P->key[2];
			B_tree left = makeNode();
			B_tree right = makeNode();
			left->n_keys = 1;
			right->n_keys = 1;
			left->key[left->n_keys] = P->key[1];
			right->key[right->n_keys] = P->key[3];
			left->child[1] = P->child[1];
			left->child[2] = P->child[2];
			right->child[1] = P->child[3];
			right->child[2] = P->child[4];

			reset(root);
			root->n_keys = 1;
			root->key[root->n_keys] = up;
			root->child[1] = left;
			root->child[2] = right;
			return;
		}

		//P�� overflow�� ����� �θ� ��.
		P = FindParent(P, root);

		int i; //�θ��� i��° child�� overflow��
		for (i = 1; i <= P->n_keys + 1; i++) {
			if (P->child[i]->n_keys == order) break;
		}

		//C�� overflow�� child���
		B_tree C = P->child[i]; 

		//�θ���� merge�� key : up
		int up = C->key[order / 2 + 1];
		//�θ��� key[i]�� up������������
		if (i == P->n_keys + 1) {
			P->key[i] = up;
			P->n_keys++;
		}
		else {
			for (int j = P->n_keys; j >= i; j--) {
				P->key[j + 1] = P->key[j];
				if (j == i) {
					P->key[j] = up;
					P->n_keys++;
					break;
				}
			}
		}

		//overflow�� ���� �ɰ�����
		B_tree left = makeNode();
		B_tree right = makeNode();		 
		left->n_keys = 1;
		right->n_keys = 1;
		left->key[left->n_keys] = C->key[1]; 
		right->key[right->n_keys] = C->key[3];

		//�ɰ���带 �θ��� child[i]�� child[i+1]�� ������
		for (int j = order; j >= i; j--) { //�����Ⱚ�̰� ���� �ϴ� ������
			P->child[j + 1] = P->child[j];
			if (j == i) {
				P->child[i] = left;
				P->child[i + 1] = right;
				break;
			}
		}

		//C�� child�� left�� right�� �����ָ� ��
		left->child[1] = C->child[1];
		left->child[2] = C->child[2];
		right->child[1] = C->child[3];
		right->child[2] = C->child[4];

		//while�� ó������ ���ư��� ���� merge�� �θ��尡 overflow�ƴ��� �˻�
	}
	
}