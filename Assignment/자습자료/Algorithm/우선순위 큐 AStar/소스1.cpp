#include "PriorityQueue.h"
#include "DoubleLinkedList.h"

// F = G + H
// G = 가로,세로 10 / 대각선 14
// H = 목표까지 거리 ( 대각선 이동 제외 )
// 열린 리스트 = 탐색한 경로 중 아직 가지 않은 길
// 닫힌 리스트 = 탐색한 경로 중 이미 지나 온 길

#define MAP_X 20
#define MAP_Y 10

#define END_X 18
#define END_Y 8

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13

#define MAX(A,B) ((a > b) ? a : b)
#define MIN(A,B) ((a < b) ? a : b)

PriorityQueue* Queue;
ListNode<Path>* OpenList;
ListNode<Path>* CloseList;

ListNode<Path>* PathList;

int map[MAP_Y][MAP_X];

int dx[8] = { 1 , 1, 1, 0, -1, -1, -1, 0};
int dy[8] = { 1, 0, -1, -1, -1, 0, 1, 1 };

int end_x, end_y;

void Init_map();
void Draw_map();

void PathFinding();
void SavePath();

int oper(int a, int b) {
	int m = MAX(a, b);
	int n = MIN(a, b);

	return m - n;
}

int H_Score(Path path) {
	int a = oper(path.x, end_x);
	int b = oper(path.y, end_y);

	a = a * 10;
	b = b * 10;

	return a + b;
}

int main()
{
	int key;
	end_x = END_X;
	end_y = END_Y;

	while (1) {
		PQ_CreateQueue(&Queue, 100);
		DL_CreateListNode(&OpenList);
		DL_CreateListNode(&CloseList);
		DL_CreateListNode(&PathList);

		system("cls");
		Init_map();
		PathFinding();

		key = getch();
		if (key == 224) {
			key = getch();
			switch (key) {
			case UP:
				if (map[end_y - 1][end_x] != 1) {
					end_y--;
				}
				break;

			case DOWN:
				if (map[end_y+ 1][end_x] != 1) {
					end_y++;
				}
				break;

			case LEFT:
				if (map[end_y][end_x - 1] != 1) {
					end_x--;
				}
				break;

			case RIGHT:
				if (map[end_y][end_x + 1] != 1) {
					end_x++;
				}
				break;
			}
		}
		else {
			if (key == ENTER) {
				break;
			}
		}
		map[end_y][end_x] = 0;

		PQ_DestroyQueue(Queue);
		DL_FreeListNode(OpenList);
		DL_FreeListNode(CloseList);
		DL_FreeListNode(PathList);
	}
	return 0;
}

void Init_map() {
	int temp_map[MAP_Y][MAP_X] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1 },
		{1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1 },
		{1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1 },
		{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1 },
		{1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1 },
		{1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1 },
		{1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1 },
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};

	for (int i = 0; i < MAP_Y; i++) {
		for (int j = 0; j < MAP_X; j++) {
			map[i][j] = temp_map[i][j];
		}
	}

	map[end_y][end_x] = 3;

	Path path;
	ZeroMemory(&path, sizeof(Path));
	path.x = 1;
	path.y = 1;

	path.G = 0;
	path.H = H_Score(path);
	path.F = path.G + path.H;

	PQ_Enqueue(Queue, path);
	DL_Insert(OpenList, path);
}

void Draw_map() {
	for (int i = 0; i < MAP_Y; i++) {
		for (int j = 0; j < MAP_X; j++) {
			switch (map[i][j]) {
			case 0:
				printf("·");
				break;

			case 1:
				printf("■");
				break;

			case 2:
				printf("○");
				break;

			case 3:
				printf("★");
				break;
			}
		}
		printf("\n");
	}
}

void PathFinding() {
	Path path = PQ_Dequeue(Queue);

	if (path.x == 0 && path.y == 0) {
		return;
	}
	else if (path.x == end_x && path.y == end_y) {
		SavePath();
		return;
	}

	DL_Insert(CloseList, path);

	Node<Path>* ptr = OpenList->head->R_link;
	while (ptr != OpenList->tail) {
		if (ptr->data.x == path.x && ptr->data.y == path.y) {
			DL_Delete(OpenList, ptr);
			break;
		}
		ptr = ptr->R_link;
	}

	for (int i = 0; i < 8; i++) {
		int y = path.y + dy[i];
		int x = path.x + dx[i];

		if (map[y][x] != 1) {
			Node<Path>* close = CloseList->head->R_link;
			Node<Path>* open = OpenList->head->R_link;
			bool flag = false;

			while (close != CloseList->tail) {
				if (close->data.x == x && close->data.y == y) {
					flag = true;
					break;
				}
				close = close->R_link;
			}

			if (!flag) {
				while (open != OpenList->tail) {
					if (open->data.x == x && open->data.y == y) {	
						flag = true;
						break;
					}
					open = open->R_link;
				}
			}

			if (!flag) {
				//printf("%d %d Find!\n", y, x);
				Path newPath;
				
				newPath.x = x;
				newPath.y = y;

				if (path.x != newPath.x && path.y != newPath.y) {
					newPath.G =/* path.G +*/ 14;
				}
				else {
					newPath.G =/* path.G +*/ 10;
				}

				newPath.H = H_Score(newPath);

				newPath.F = newPath.H + newPath.G;

				PQ_Enqueue(Queue, newPath);
				DL_Insert(OpenList, newPath);

			}
		}
	}

	//Node<Path>* print = PathList->head->R_link;
	//while (print != PathList->tail) {
	//	printf("(%d,%d)  F  %d\n", print->data.y, print->data.x, print->data.F);
	//	print = print->R_link;
	//}

	//for (int i = Queue->Front + 1; i <= Queue->Rear; i++) {
	//	printf("%d %d %d\n", Queue->path[i].y, Queue->path[i].x, Queue->path[i].F);
	//}
	//printf("%d %d", Queue->Front, Queue->Rear);
	//printf("\n\n");

	//getch();
	PathFinding();
}

void SavePath() {
	Node<Path>* close = CloseList->head->R_link;
	DL_Insert(PathList, close->data);

	while (close != CloseList->tail) {
		Path temp;
		ZeroMemory(&temp, sizeof(Path));

		if (close->data.x == PathList->head->R_link->data.x && close->data.y == PathList->head->R_link->data.y) {
			for (int i = 0; i < 8; i++) {
				Node<Path>* ptr = close;

				int y = close->data.y + dy[i];
				int x = close->data.x + dx[i];

				while (ptr != CloseList->tail) {
					ptr = ptr->R_link;
					if (ptr->data.y == y && ptr->data.x == x) {
						//printf("%d %d\n", ptr->data.y, ptr->data.x);

						if (ptr->data.F >= temp.F) {
							temp = ptr->data;
						}
					}
				}
			}
			DL_Insert(PathList, temp);
		}
		close = close->R_link;
	}

	Node<Path>* print = PathList->head->R_link->R_link;
	while (print != PathList->tail) {
		map[print->data.y][print->data.x] = 2;
		print = print->R_link;
	}

	Draw_map();

	//print = PathList->head->R_link->R_link;
	//while (print != PathList->tail) {
	//	printf("%d %d %d\n", print->data.y, print->data.x, print->data.F);
	//	print = print->R_link;
	//}
}