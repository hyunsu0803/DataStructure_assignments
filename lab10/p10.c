#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Graph Graph;
struct _Graph {
	int size;
	int* node;
	int** matrix;
	int* indegree; //각 노드의 indegree 저장
};
typedef struct _Queue Queue;
struct _Queue {
	int* key;
	int front;
	int rear;
	int qsize;
	int max_queue_size;
};
Graph* CreateGraph(int* nodes, int nV); //맨처음 그래프 읽어와서 그래프 만들기
void InsertEdge(Graph* G, int a, int b); //행렬에 1 넣기
void TopSort(Graph* G, FILE* output); // topological sort결과 프린트
Queue* MakeNewQueue(int nV); //queue 만들고 초기화하기
void Enqueue(Queue* Q, int x); 
int Dequeue(Queue* Q);
void PrintMatrix(Graph* G, FILE* output);
void sort(int* key, int n);
int main() {
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");

	Graph* G = NULL;

	int nV = 0;//노드의 개수
	char getNodes[20] = { 0 }; //input에서 노드 읽어오기
	int Nodes[10] = { 0 }; //읽은 노드를 int로 저장

	fgets(getNodes, sizeof(getNodes), input);	
	for (int i = 0; i < strlen(getNodes); i += 2) {
		Nodes[i / 2] = getNodes[i] - '0';
		nV++;
	}
	//노드 만들기
	G = CreateGraph(Nodes, nV);
	//인접행렬 만들기
	char temp[3];
	while (fscanf(input, " %c%c%c", &temp[0], &temp[1], &temp[2]) != EOF) {
		InsertEdge(G, temp[0] - '0', temp[2] - '0');
	}
	
	//인접 행렬 프린트
	PrintMatrix(G, output);
	//topological sort 프린트
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
	int x, y; //인접행렬에 1 들어갈 인덱스

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
	Q->front = 0; //deq 할때마다 front 1증가
	Q->rear = -1; //enq 할때마가 rear 1증가
	Q->qsize = 0;
	Q->max_queue_size = nV;

	return Q;
}
void Enqueue(Queue* Q, int x) {
	if (Q->qsize == Q->max_queue_size) {
		return; //꽉 찬 큐에는 못넣음
	}
	else if (Q->rear + 1 != Q->max_queue_size) {
		Q->rear++; //rear가 넘어가지 않으면 그냥 증가
		Q->key[Q->rear] = x;
		Q->qsize++;
	}
	else { //넘어가는 경우
		Q->rear = 0;
		Q->key[Q->rear] = x;
		Q->qsize++;
	}
}
int Dequeue(Queue* Q) {
	int pop = 0;
	if (Q->qsize == 0) {
		return pop; //빈 큐를 디큐할 수 없음
	}
	else if (Q->front + 1 != Q->max_queue_size) {
		pop = Q->key[Q->front];
		Q->front++; //front가 넘어가지 않으면 그냥 당기기
		Q->qsize--;
	}
	else {
		pop = Q->key[Q->front];
		Q->front = 0; //front가 넘어가면
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
	//큐 만들기
	Queue* Q = MakeNewQueue(G->size);

	//방문 표시할 배열
	int* visited = (int*)malloc(sizeof(int)*G->size);
	memset(visited, 0, sizeof(visited)*G->size);

	while (1) {
		//indegree 0인 노드를 큐에 넣자
		int goingQ[10]; //이번판에 큐에 들어갈 친구들
		int n = 0;//이번판에 큐에 들어가는 개수
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

		//pop 출력
		int pop = Dequeue(Q);
		fprintf(output, "%d ", pop);
		for (int i = 0; i < G->size; i++) {
			if (pop == G->node[i]) {
				pop = i;
				break;
			}
		}
		//pop에 연결된 간선 삭제
		for (int i = 0; i < G->size; i++) {
			if (G->matrix[pop][i] == 1) {
				G->indegree[i]--;
				G->matrix[pop][i] = 0;
			}
		}

		//모든 노드에 방문했다면 break;
		int fin = 1;
		for (int i = 0; i < G->size; i++) {
			if (!visited[i]) fin = 0;
		}
		if (fin) break;
	}
}
void sort(int* key, int n) {
	//n-1이 key의 마지막 인덱스 
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
