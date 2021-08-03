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

	//노드들 입력받기
	int nV = 0;
	int nodes[105] = { 0 };
	char space;
	for(int i = 0; i<100; i++){
		fscanf(input, "%d%c", &nodes[i], &space);
		nV++;
		if (space == '\n') break;
	}

	int* d = (int*)malloc(sizeof(int)*nV);	//start점에서 i점까지 최단거리 저장
	int* pred = (int*)malloc(sizeof(int)*nV);//start점에서 i점까지 최단경로일때 i점 직전에 거친 점
	int** w = (int**)malloc(sizeof(int*)*nV);//i점에서 j점으로 가는 길의 가중치
	for (int i = 0; i < nV; i++) {
		w[i] = (int*)malloc(sizeof(int)*nV);
	}

	//기본설정은 INF
	for (int i = 0; i < nV; i++) {
		for (int j = 0; j < nV; j++) {
			w[i][j] = INF;
		}
		d[i] = INF;
		pred[i] = INF;
	}

	//간선 입력받기
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

	//시작과 끝 입력받기
	int startNum = a;
	int endNum = b;
	int startIdx = findIdx(startNum, nV, nodes);
	int endIdx = findIdx(endNum, nV, nodes);

	//d설정하기 : start랑 직접 연결된 곳은 INF아님
	//pred설정하기 : start랑 직접 연결된 곳은 pred가 start
	for (int i = 0; i <= nV; i++) {
		if (w[startIdx][i] != INF) {
			d[i] = w[startIdx][i];
			pred[i] = startIdx;
		}
	}
	d[startIdx] = 0;
	pred[startIdx] = startIdx;

	//힙 만들기
	Heap* H = createMinHeap(nV);

	//힙에 노드 다 넣기
	for (int i = 0; i < nV; i++) {
		if (i != startIdx)
			insert(H, nodes[i], d[i]);
	}

	//최단거리 찾기 시작
	while (H->size != 0) {
		int u = deleteMin(H);
		u = findIdx(u, nV, nodes);
		//~~ : u는 permanent됨
		for (int v = 0; v < nV; v++) {
			if (d[u] + w[u][v] < d[v]) {
				d[v] = d[u] + w[u][v];
				pred[v] = u;//다 인덱스로 들어가고 있음.
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
	H->element = (Node*)malloc(sizeof(Node)*(heapSize + 1)); //인덱스1부터

	return H;
}
void insert(Heap* H, int key, int w) {
	int i;
	H->size++; //일단 여기에 x를 넣어두고
	// i는 x의 인덱스
	// i가 root노드가 아니고, i가 부모보다 가중치가 작으면 자리바꾸기
	for (i = H->size; i / 2 > 0 && H->element[i / 2].weight > w; i /= 2) {
		H->element[i] = H->element[i / 2];
	}
	H->element[i].key = key;
	H->element[i].weight = w;
}
int deleteMin(Heap* H) {
	//Min을 지우고 Last를 Min자리에 올리고 다시 제자리를 찾아갈 예정
	Node Min = H->element[1];
	Node Last = H->element[H->size--];

	//Last가 자식보다 가중치가 크면 자리바꾸기
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

	//v의 가중치가 작아졌지만 부모보다는 여전히 크다면 그냥 할 일이 없다.
	if (H->element[i].weight > H->element[i / 2].weight)
		return;
	//v의 가중치가 부모보다 작아졌다면 위로 올라간다.
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
