#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Graph Graph;
struct _Graph {
	int size;
	int* node;
	int** matrix;
	int* indegree; //�� ����� indegree ����
};
typedef struct _Queue Queue;
struct _Queue {
	int* key;
	int front;
	int rear;
	int qsize;
	int max_queue_size;
};
Graph* CreateGraph(int* nodes, int nV); //��ó�� �׷��� �о�ͼ� �׷��� �����
void InsertEdge(Graph* G, int a, int b); //��Ŀ� 1 �ֱ�
void TopSort(Graph* G, FILE* output); // topological sort��� ����Ʈ
Queue* MakeNewQueue(int nV); //queue ����� �ʱ�ȭ�ϱ�
void Enqueue(Queue* Q, int x); 
int Dequeue(Queue* Q);
void PrintMatrix(Graph* G, FILE* output);
void sort(int* key, int n);
int main() {
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");

	Graph* G = NULL;

	int nV = 0;//����� ����
	char getNodes[20] = { 0 }; //input���� ��� �о����
	int Nodes[10] = { 0 }; //���� ��带 int�� ����

	fgets(getNodes, sizeof(getNodes), input);	
	for (int i = 0; i < strlen(getNodes); i += 2) {
		Nodes[i / 2] = getNodes[i] - '0';
		nV++;
	}
	//��� �����
	G = CreateGraph(Nodes, nV);
	//������� �����
	char temp[3];
	while (fscanf(input, " %c%c%c", &temp[0], &temp[1], &temp[2]) != EOF) {
		InsertEdge(G, temp[0] - '0', temp[2] - '0');
	}
	
	//���� ��� ����Ʈ
	PrintMatrix(G, output);
	//topological sort ����Ʈ
	TopSort(G, output);
}
Graph* CreateGraph(int* nodes, int nV) {
	Graph* G = (Graph*)malloc(sizeof(Graph)*nV);
	G->size = nV;
	G->node = (int*)malloc(sizeof(int)*nV);
	G->indegree = (int*)malloc(sizeof(int)*nV);
	for (int i = 0; i < nV; i++) {
		G->node[i] = nodes[i];
		G->indegree[i] = 0;
	}
	G->matrix = (int**)malloc(sizeof(int*)*nV);
	for (int i = 0; i < nV; i++) {
		G->matrix[i] = (int*)malloc(sizeof(int)*nV);
		for (int j = 0; j < nV; j++) {
			G->matrix[i][j] = 0;
		}
	}

	return G;
}
void InsertEdge(Graph* G, int a, int b) {
	int x, y; //������Ŀ� 1 �� �ε���

	for (int i = 0; i < G->size; i++) {
		if (a == G->node[i]) x = i;
		else if (b == G->node[i]) y = i;
	}

	G->matrix[x][y] = 1;
	G->indegree[y]++;
}
Queue* MakeNewQueue(int nV) {
	Queue* Q = (Queue*)malloc(sizeof(Queue));
	Q->key = (int*)malloc(sizeof(int)*nV);
	Q->front = 0; //deq �Ҷ����� front 1����
	Q->rear = -1; //enq �Ҷ����� rear 1����
	Q->qsize = 0;
	Q->max_queue_size = nV;

	return Q;
}
void Enqueue(Queue* Q, int x) {
	if (Q->qsize == Q->max_queue_size) {
		return; //�� �� ť���� ������
	}
	else if (Q->rear + 1 != Q->max_queue_size) {
		Q->rear++; //rear�� �Ѿ�� ������ �׳� ����
		Q->key[Q->rear] = x;
		Q->qsize++;
	}
	else { //�Ѿ�� ���
		Q->rear = 0;
		Q->key[Q->rear] = x;
		Q->qsize++;
	}
}
int Dequeue(Queue* Q) {
	int pop = 0;
	if (Q->qsize == 0) {
		return pop; //�� ť�� ��ť�� �� ����
	}
	else if (Q->front + 1 != Q->max_queue_size) {
		pop = Q->key[Q->front];
		Q->front++; //front�� �Ѿ�� ������ �׳� ����
		Q->qsize--;
	}
	else {
		pop = Q->key[Q->front];
		Q->front = 0; //front�� �Ѿ��
		Q->qsize--;
	}
	return pop;
}
void PrintMatrix(Graph* G, FILE* output) {
	fprintf(output, "Adjacency matrix\n  ");
	for (int i = 0; i < G->size; i++) {
		fprintf(output , "%d ", G->node[i]);
	}
	fprintf(output, "\n");
	for (int i = 0; i < G->size; i++) {
		fprintf(output, "%d ", G->node[i]);
		for (int j = 0; j < G->size; j++) {
			fprintf(output, "%d ", G->matrix[i][j]);
		}
		fprintf(output, "\n");
	}
}
void TopSort(Graph* G, FILE* output) {
	fprintf(output, "\nTopSort Result : ");
	//ť �����
	Queue* Q = MakeNewQueue(G->size);

	//�湮 ǥ���� �迭
	int* visited = (int*)malloc(sizeof(int)*G->size);
	memset(visited, 0, sizeof(visited)*G->size);

	while (1) {
		//indegree 0�� ��带 ť�� ����
		int goingQ[10]; //�̹��ǿ� ť�� �� ģ����
		int n = 0;//�̹��ǿ� ť�� ���� ����
		for (int i = 0; i < G->size; i++) {
			if (G->indegree[i] == 0 && !visited[i]) {
				goingQ[n++] = G->node[i];
				visited[i] = 1;
			}
		}
		sort(goingQ, n);
		for (int i = 0; i < n; i++) {
			Enqueue(Q, goingQ[i]);
		}

		//pop ���
		int pop = Dequeue(Q);
		fprintf(output, "%d ", pop);
		for (int i = 0; i < G->size; i++) {
			if (pop == G->node[i]) {
				pop = i;
				break;
			}
		}
		//pop�� ����� ���� ����
		for (int i = 0; i < G->size; i++) {
			if (G->matrix[pop][i] == 1) {
				G->indegree[i]--;
				G->matrix[pop][i] = 0;
			}
		}

		//��� ��忡 �湮�ߴٸ� break;
		int fin = 1;
		for (int i = 0; i < G->size; i++) {
			if (!visited[i]) fin = 0;
		}
		if (fin) break;
	}
}
void sort(int* key, int n) {
	//n-1�� key�� ������ �ε��� 
	for (int i = 0; i <= n; i++) {
		for (int j = 0; j <= n - i - 2; j++) {
			if (key[j] > key[j + 1]) {
				int temp = key[j];
				key[j] = key[j + 1];
				key[j + 1] = temp;
			}
		}
	}
}
