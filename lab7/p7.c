#define _CRT_SECURE_NO_WARNINGS_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* Set;
char** maze;
int Find(int x) {
	if (Set[x] <= 0) return x;

	return Set[x] = Find(Set[x]);
}
void Union(int x, int y) {
	x = Find(x);
	y = Find(y);
	if (Set[x] > Set[y]) 
		Set[x] = y;
	else {
		if (Set[x] == Set[y]) 
			Set[x]--;
		Set[y] = x;
	}
}

int main(void) {
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");

	srand(time(NULL));

	int n; 
	fscanf(input, "%d", &n);

	Set = (int*)malloc(sizeof(int)*(n*n + 1));
	maze = (char**)malloc(sizeof(char*)* (2 * n + 1));

	for (int i = 0; i <= n * n; i++)
		Set[i] = 0;

	//struct maze
	for (int i = 0; i <= 2 * n; i++) {
		maze[i] = (char*)malloc(sizeof(char)*(2 * n + 1));
		if (i % 2 == 0) {
			for (int j = 0; j <= 2 * n; j++) {
				if (j % 2 == 0) maze[i][j] = '+';
				else maze[i][j] = '-';
			}
		}
		else {
			for (int j = 0; j <= 2 * n; j++) {
				if (j % 2 == 0) maze[i][j] = '|';
				else maze[i][j] = ' ';
			}
		}
	}
	maze[1][0] = ' ';
	maze[2 * n - 1][2 * n] = ' ';

	//union a & b
	int ans = 0; 
	while (ans != 1) {
		int a = rand() % (n*n) + 1;
		int edge = rand() % 4;

		if (edge == 0) { //up
			int b = a - n;
			if (b <= 0) continue; //out of maze

			int edge_x = a / n * 2;
			int edge_y = a % n * 2 - 1;

			if (a%n == 0) {
				edge_x -= 2;
				edge_y += 2 * n;
			}

			if (maze[edge_x][edge_y] == '-') {
				if (Find(a) == Find(b)) continue;
				else {
					Union(a, b);
					maze[edge_x][edge_y] = ' ';
				}
			}
			else continue;
		}
		else if (edge == 1) { //down
			int b = a + n;
			if (b > n*n) continue; //out of maze

			int edge_x = a / n * 2 + 2;
			int edge_y = a % n * 2 - 1;

			if (a%n == 0) {
				edge_x -= 2;
				edge_y += 2 * n;
			}

			if (maze[edge_x][edge_y] == '-') {
				if (Find(a) == Find(b)) continue;
				else {
					Union(a, b);
					maze[edge_x][edge_y] = ' ';
				}
			}
			else continue;
		}
		else if (edge == 2) { //left
			int b = a - 1;
			if (b%n == 0) continue; //out of maze

			int edge_x = a / n * 2 + 1;
			int edge_y = a % n * 2 - 2;

			if (a%n == 0) {
				edge_x -= 2;
				edge_y += 2 * n;
			}

			if (maze[edge_x][edge_y] == '|') {
				if (Find(a) == Find(b)) continue;
				else {
					Union(a, b);
					maze[edge_x][edge_y] = ' ';
				}
			}
			else continue;
		}
		else {//right
			int b = a + 1;
			if (a%n == 0) continue; // out of maze

			int edge_x = a / n * 2 + 1;
			int edge_y = a % n * 2;

			if (a%n == 0) {
				edge_x -= 2;
				edge_y += 2 * n;
			}

			if (maze[edge_x][edge_y] == '|') {
				if (Find(a) == Find(b)) continue;
				else {
					Union(a, b);
					maze[edge_x][edge_y] = ' ';
				}
			}
			else continue;
		}

		ans = 0;
		for (int i = 1; i <= n * n; i++) {
			if (Set[i] <= 0) ans++;
		}
	}

	for (int i = 0; i <= 2 * n; i++) {
		for (int j = 0; j <= 2 * n; j++) {
			fprintf(output, "%c", maze[i][j]);
		}
		fprintf(output,"\n");
	}

	free(Set);
	for (int i = 0; i <= 2 * n; i++) {
		free(maze[i]);
	}
	free(maze);

}

