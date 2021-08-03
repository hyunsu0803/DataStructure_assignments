#define _CRT_SECURE_NO_WARNINGS
#define INF 99999
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;
struct Node {
	int key;
	int weight;
};
typedef struct Heap Heap;
struct Heap {
	int capacity;
	int size;
	Node* element;
};

Heap* createMinHeap(int heapSize);
void insert(Heap* minHeap, int vertex, int distance);
int deleteMin(Heap* minHeap);
void printShortestPath(int* pred, int end, int* nodes, FILE* output);
void Decrease_Priority(Heap* H, int v, int w);
int findIdx(int v, int nV, int* nodes);

int main() {
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");

	//���� �Է¹ޱ�
	int nV = 0;
	int nodes[105] = { 0 };
	char space;
	for(int i = 0; i<100; i++){
		fscanf(input, "%d%c", &nodes[i], &space);
		nV++;
		if (space == '\n') break;
	}

	int* d = (int*)malloc(sizeof(int)*nV);	//start������ i������ �ִܰŸ� ����
	int* pred = (int*)malloc(sizeof(int)*nV);//start������ i������ �ִܰ���϶� i�� ������ ��ģ ��
	int** w = (int**)malloc(sizeof(int*)*nV);//i������ j������ ���� ���� ����ġ
	for (int i = 0; i < nV; i++) {
		w[i] = (int*)malloc(sizeof(int)*nV);
	}

	//�⺻������ INF
	for (int i = 0; i < nV; i++) {
		for (int j = 0; j < nV; j++) {
			w[i][j] = INF;
		}
		d[i] = INF;
		pred[i] = INF;
	}

	//���� �Է¹ޱ�
	char tmp;
	int a, b, weight;
	char t1, t2;
	while (1) {
		if (fscanf(input, "%d%c%d%c%d", &a, &t1, &b, &t2, &weight) != 5) 
			break;

		int x = findIdx(a, nV, nodes);
		int y = findIdx(b, nV, nodes);
		w[x][y] = weight;
	}

	//���۰� �� �Է¹ޱ�
	int startNum = a;
	int endNum = b;
	int startIdx = findIdx(startNum, nV, nodes);
	int endIdx = findIdx(endNum, nV, nodes);

	//d�����ϱ� : start�� ���� ����� ���� INF�ƴ�
	//pred�����ϱ� : start�� ���� ����� ���� pred�� start
	for (int i = 0; i <= nV; i++) {
		if (w[startIdx][i] != INF) {
			d[i] = w[startIdx][i];
			pred[i] = startIdx;
		}
	}
	d[startIdx] = 0;
	pred[startIdx] = startIdx;

	//�� �����
	Heap* H = createMinHeap(nV);

	//���� ��� �� �ֱ�
	for (int i = 0; i < nV; i++) {
		if (i != startIdx)
			insert(H, nodes[i], d[i]);
	}

	//�ִܰŸ� ã�� ����
	while (H->size != 0) {
		int u = deleteMin(H);
		u = findIdx(u, nV, nodes);
		//~~ : u�� permanent��
		for (int v = 0; v < nV; v++) {
			if (d[u] + w[u][v] < d[v]) {
				d[v] = d[u] + w[u][v];
				pred[v] = u;//�� �ε����� ���� ����.
				Decrease_Priority(H, nodes[v], d[v]);
			}
		}
	}

	printShortestPath(pred, endIdx, nodes, output);

    fclose(input);
    fclose(output);
}
Heap* createMinHeap(int heapSize) {
	Heap* H = (Heap*)malloc(sizeof(Heap));
	H->capacity = 100;
	H->size = 0;
	H->element = (Node*)malloc(sizeof(Node)*(heapSize + 1)); //�ε���1����

	return H;
}
void insert(Heap* H, int key, int w) {
	int i;
	H->size++; //�ϴ� ���⿡ x�� �־�ΰ�
	// i�� x�� �ε���
	// i�� root��尡 �ƴϰ�, i�� �θ𺸴� ����ġ�� ������ �ڸ��ٲٱ�
	for (i = H->size; i / 2 > 0 && H->element[i / 2].weight > w; i /= 2) {
		H->element[i] = H->element[i / 2];
	}
	H->element[i].key = key;
	H->element[i].weight = w;
}
int deleteMin(Heap* H) {
	//Min�� ����� Last�� Min�ڸ��� �ø��� �ٽ� ���ڸ��� ã�ư� ����
	Node Min = H->element[1];
	Node Last = H->element[H->size--];

	//Last�� �ڽĺ��� ����ġ�� ũ�� �ڸ��ٲٱ�
	int child, i;
	for (i = 1; i*2 < H->size; i = child) {
		child = i * 2;
		if (child != H->size && H->element[child + 1].weight < H->element[child].weight)
			child++;
		if (Last.weight > H->element[child].weight)
			H->element[i] = H->element[child];
		else break;
	}
	H->element[i] = Last;
	
	return Min.key;
}
int findIdx(int v, int nV, int* nodes) {
	int resultIndex;
	for (int i = 0; i < nV; i++) {
		if (nodes[i] == v) resultIndex = i;
	}
	return resultIndex;
}
void Decrease_Priority(Heap* H, int v, int w) {
	Node V;
	int i;
	for (i = 1; i <= H->size; i++) {
		if (H->element[i].key == v) {
			H->element[i].weight = w;
			V = H->element[i];
			break;
		}
	}

	//v�� ����ġ�� �۾������� �θ𺸴ٴ� ������ ũ�ٸ� �׳� �� ���� ����.
	if (H->element[i].weight > H->element[i / 2].weight)
		return;
	//v�� ����ġ�� �θ𺸴� �۾����ٸ� ���� �ö󰣴�.
	for (i = H->size; i / 2 > 0 && H->element[i].weight < H->element[i / 2].weight; i /= 2) {
		H->element[i] = H->element[i / 2];
	}
	H->element[i] = V;
}
void printShortestPath(int* pred, int idx, int* nodes, FILE* output) {
	if (pred[idx] == INF) {
		fprintf(output, "no path\n");
		return;
	}
	if (pred[idx] == idx)
	{
		fprintf(output, "%d ", nodes[idx]);
		return;
	}
	printShortestPath(pred, pred[idx], nodes, output);
	fprintf(output, "%d ", nodes[idx]);
}
