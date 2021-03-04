#include "Global.h"
using namespace std::chrono;
using namespace std;

// 열린 노드, 닫힌 노드 벡터
vector<AStar_Node*> open_nodes;
vector<AStar_Node*> close_nodes;

// 맵 정보
int maps[MAP_Y][MAP_X];

// G 비용은 최초 위치에서 현재 위치까지 필요한 소모 비용
// 이동 방향이 대각선일 경우 14
// 직선인 경우 10
int G_Score(__in const AStar_Node* _parent, __in const AStar_Node* _child)
{
	// 절대값으로 현재 위치와 이동 할 위치의 차를 구함
	int x = abs(_parent->x - _child->x);
	int y = abs(_parent->y - _child->y);

	// 그 차가 2 인 경우 대각선이고, 그 이하면 직선임
	return (x + y == 2) ? _parent->G + 14 : _parent->G + 10;
}

// H 비용은 현재 위치에서 타겟 위치까지 필요한 소모 비용
// 장애물은 고려하지 않고 예상 거리차를 구함
// 구하는 공식은 여러 방법이 있음
// 아래 공식은 대각선을 제외한 가로, 세로 공식만 구한 공식
int H_Score(__in const AStar_Node* _path, __in const int _end_x, __in const int _end_y)
{
	// 절대값으로 현재 위치와 타겟의 위치의 차를 구함
	int x = abs(_path->x - END_X);
	int y = abs(_path->y - END_Y);

	// 가로,세로 방향 중 더 가까운 방향에 맞는 값을 구함
	return (x < y) ? (x * 7 + (y - x) * 5) : (y * 7 + (x - y) * 5);
}

// F 비용은 현재 위치에서 타겟 위치까지 가기 위해 필요한 총 소모 비용
// F = G + H
int F_Score(__in const AStar_Node* _path)
{
	return _path->G + _path->H;
}

// 길찾기 알고리즘
void PathFinding_Setting(__in const int, __in const int, __in const int, __in const int);
AStar_Node* PathFinding(__in const int, __in const int);

// 맵 그리기
void Print();

// 현 위치의 주변 노드 탐색 ( 3 * 3 )
vector<AStar_Node*> GetNeighbours(__in const AStar_Node* _node);

// 완료된 패스를 탐색
stack<AStar_Node*> SearchPath(__in AStar_Node* _node);

void Move(__in int start_x, __in int start_y, __in int end_x, __in int end_y);
void Animation(__in int start_x, __in int start_y, __in int end_x, __in int end_y);

int main()
{
	// 맵
	// ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ 
	// ■ · · ■ ■ ■ · · · · ■ ■ ■ · · · · ■ ■ ■ 
	// ■ · · ■ ■ ■ · · · · · ■ · · ■ ■ · ■ ■ ■ 
	// ■ · · ■ ■ ■ · · ■ · · ■ · · ■ ■ · · ■ ■ 
	// ■ · · ■ ■ · · ■ ■ · · · · · · ■ · · ■ ■ 
	// ■ ■ · · ■ · · ■ ■ · · · · ■ · · · · ■ ■ 
	// ■ ■ · · ■ · · · · · · ■ ■ ■ ■ ■ · · ■ ■ 
	// ■ · · · · · · · · · · ■ ■ ■ ■ ■ · · ■ ■ 
	// ■ ■ · · · ■ ■ ■ ■ · · ■ ■ ■ · · · · · ■ 
	// ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ 

	//Move(1, 1, END_X, END_Y);
	Animation(1, 1, END_X, END_Y);
}

void PathFinding_Setting(__in const int _start_x, __in const int _start_y, __in const int _end_x, __in const int _end_y)
{
	// 맵 정보 초기화
	int temp_maps[MAP_Y][MAP_X] = {
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

	// 맵 정보 변수에 카피
	memcpy(&maps, &temp_maps, sizeof(maps));

	// 타겟 위치 지정
	maps[_end_y][_end_x] = TARGET;

	// 최초 위치 세팅
	AStar_Node* node = new AStar_Node(_start_x, _start_y);
	node->G = 0;
	node->H = H_Score(node, _end_x, _end_y);
	node->F = F_Score(node);

	// 열린 노드 벡터에 추가
	open_nodes.push_back(node);
}

void Print()
{
	// 각 역할에 맞는 문자를 위치마다 출력
	for (int i = 0; i < MAP_Y; i++) {
		for (int j = 0; j < MAP_X; j++) {
			switch (maps[i][j]) {
			case EMPTY:
				printf("·");
				break;

			case WALL:
				printf("■");
				break;

			case PATH:
				printf("○");
				break;

			case TARGET:
				printf("★");
				break;

			case PLAYER:
				printf("옷");
				break;
			}
		}
		printf("\n");
	}
}

vector<AStar_Node*> GetNeighbours(__in const AStar_Node* _node)
{
	vector<AStar_Node*> neighbours;

	// 매개변수로 받은 기준이 되는 노드 위치를 기준으로 주변 노드를 탐색
	// -1 ~ 1 인 경우는 주변 노드의 3 * 3 범위를 탐색
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			// 0,0 은 기준이 되는 노드의 위치이기 때문에 추가 할 필요가 없으므로 예외 처리
			if (0 == x && 0 == y) continue;
			
			// check_x, check_y = 기준이 되는 노드 위치 + 주변 노드 위치
			bool flag = false;
			int check_x = _node->x + x;
			int check_y = _node->y + y;

			// 아래 세 조건이 부합하는 경우만 주변 노드 벡터에 추가한다.
			// 1. 노드의 위치가 맵의 범위를 벗어나지 않는다.
			// 2. 노드의 위치에 벽이 없다.
			// 3. 노드가 닫힌 노드에 포함되어 있지 않다.			
			if (check_x >= 0 && check_x < MAP_X && 
				check_y >= 0 && check_y < MAP_Y &&
				maps[check_y][check_x] != WALL)
			{
				for (const AStar_Node* temp : close_nodes)
				{
					if (temp->x == check_x && temp->y == check_y)
					{
						flag = true;
						break;
					}
				}
				if (!flag)
					neighbours.push_back(new AStar_Node(check_x, check_y));
			}			
		}
	}

	// 구해진 주변 노드를 반환한다.
	return neighbours;
}

AStar_Node* PathFinding(__in const int _end_x, __in const int _end_y)
{
	// 열린 노드가 빈다면 반복문을 종료한다.
	// 이 경우엔 길찾기 탐색을 실패했다는 의미가 된다.
	while (!open_nodes.empty())
	{
		// 열린 노드 벡터의 가장 앞에 있는 노드를 가져온다.
		// 이 벡터는 F 값을 기준으로 내림차순으로 정렬이 되어 있다.
		// 따라서 가장 비용이 적은 노드를 가지고 올 수 있다.
		AStar_Node* current_node = open_nodes.front();

		// 가지고 온 노드 정보는 열린 노드에서 제거한다.
		// 우선순위 큐를 사용하면 더 좋지만 열린 노드 순회를 위해 벡터와 정렬을 사용하였다.
		// 힙 정렬을 이용한 리스트를 사용하면 성능이 더 좋을수도 있다.
		open_nodes.erase(open_nodes.begin());

		// 가져 온 노드의 위치가 타겟 위치라면 타겟을 찾았다는 결과이다.
		// 마지막 노드를 반환한 후 함수를 종료한다.
		if (_end_x == current_node->x && _end_y == current_node->y)	
			return current_node;
		

		// 현재 노드 정보를 닫힌 노드에 추가한다.
		close_nodes.push_back(current_node);

		// 현재 노드 주변 노드를 탐색한 후 정보를 가지고 온다.
		vector<AStar_Node*> neighbourNodes = GetNeighbours(current_node);

		// 주변 노드를 탐색한다.
		for (int i = 0; i < neighbourNodes.size(); i++)
		{			
			bool visit = false;
			// 주변 노드를 하나씩 선택
			AStar_Node* search_node = neighbourNodes[i];

			// 현재 노드 위치와 선택된 주변 노드 위치 차를 구한다.
			int distance = G_Score(current_node, search_node);

			// 선택된 노드가 열린 노드 벡터 내에 존재하는 지 검사
			for (AStar_Node* node : open_nodes)
			{
				if (node->x == search_node->x && node->y == search_node->y)
				{
					visit = true;
					break;
				}
			}

			// 선택된 노드가 열린 노드 벡터에 포함되어 있지 않은 경우
			if (/*distance < search_node->G ||*/ !visit)
			{
				// 노드의 소모 비용 값들을 세팅
				search_node->G = distance;
				search_node->H = H_Score(search_node, _end_x, _end_y);
				search_node->F = F_Score(search_node);

				// 선택된 주변 노드의 부모 노드를 현재 노드로 지정
				search_node->parent = current_node;

				// 선택된 노드를 열린 노드 벡터에 추가
				open_nodes.push_back(search_node);

				// F 값을 기준으로 열린 노드 벡터를 내림차순으로 정렬
				sort(open_nodes.begin(), open_nodes.end(), compare);

				// 만약 열린 노드 내에 선택된 노드가 존재하는 경우
				// 현재 노드와 선택된 노드의 G 값을 비교한다.
				// 현재 노드의 G 값이 작은 경우
				// 현재 노드의 부모 노드가 아닌 선택된 노드의 부모 노드를 현재 노드로 지정한다.
				// 이 때 선택된 노드의 H, G, F 값을 다시 세팅한다.
				// 이 과정은 현재 코드에서는 하다.. 말았다..

				//if (!visit)
				//{
				//	open_nodes.push_back(search_node);
				//	sort(open_nodes.begin(), open_nodes.end(), compare);
				//}
				//else
				//{
				//	sort(open_nodes.begin(), open_nodes.end(), compare);
				//}
			}
		}
	}
	return nullptr;
}

stack<AStar_Node*> SearchPath(__in AStar_Node* _node)
{
	// 매개 변수로 받은 노드는 타겟 위치까지 도달한 노드
	// 이 노드의 부모 노드는 이전 위치의 노드이다.
	AStar_Node* node = _node;
	stack<AStar_Node*> nodes;

	while (nullptr != node)
	{
		nodes.push(node);
		node = node->parent;
	}	

	// 최소 비용으로 진행하였기 때문에 부모 노드를 따라가면 최적의 경로가 나온다.
	// 최종적으로 최적의 경로를 구할 수 있게 된다.
	//while (nullptr != node->parent)
	//{
	//	maps[node->y][node->x] = PATH;
	//	node = node->parent;
	//}

	//// 플레이어를 표시하기 위한 예외 처리
	//if (nullptr != node)
	//	maps[node->y][node->x] = PLAYER;

	// 열린 노드, 닫힌 노드 초기화
	open_nodes.clear();
	close_nodes.clear();

	return nodes;
}

void Move(__in int start_x, __in int start_y, __in int end_x, __in int end_y)
{
	int key;
	pair<int, int> pos = make_pair(start_x, start_y);

	while (true)
	{
		system("cls");

		auto start = high_resolution_clock::now();

		PathFinding_Setting(pos.first, pos.second, end_x, end_y);
		AStar_Node* node = PathFinding(end_x, end_y);
		
		if (nullptr == node)
			cout << "길찾기 실패" << endl;
		else
		{
			stack<AStar_Node*> nodes = SearchPath(node->parent);			

			maps[nodes.top()->y][nodes.top()->x] = PLAYER;
			nodes.pop();

			while (!nodes.empty())
			{
				maps[nodes.top()->y][nodes.top()->x] = PATH;
				nodes.pop();
			}
			
			Print();
		}

		auto end = high_resolution_clock::now() - start;
		cout << endl << duration_cast<milliseconds>(end).count() << " milliseconds" << endl;
		cout << endl << "방향키 : 이동" << endl << "Enter : 종료" << endl;

		key = _getch();
		if (key == 224)
		{
			key = _getch();
			switch (key)
			{
			case INPUT_KEY::UP:
				if (WALL != maps[pos.second - 1][pos.first] &&
					TARGET != maps[pos.second - 1][pos.first])
					--pos.second;
				break;

			case INPUT_KEY::DOWN:
				if (WALL != maps[pos.second + 1][pos.first] &&
					TARGET != maps[pos.second + 1][pos.first])
					++pos.second;
				break;

			case INPUT_KEY::LEFT:
				if (WALL != maps[pos.second][pos.first - 1] &&
					TARGET != maps[pos.second][pos.first - 1])
					--pos.first;
				break;

			case INPUT_KEY::RIGHT:
				if (WALL != maps[pos.second][pos.first + 1] &&
					TARGET != maps[pos.second][pos.first + 1])
					++pos.first;
				break;
			}
		}
		else
		{
			if (key == INPUT_KEY::ENTER)
				break;
		}
	}
}

void Animation(__in int start_x, __in int start_y, __in int end_x, __in int end_y)
{
	PathFinding_Setting(start_x, start_y, end_x, end_y);
	AStar_Node* node = PathFinding(end_x, end_y);
	stack<AStar_Node*> nodes = SearchPath(node);
	node = node->parent;

	while (!nodes.empty())
	{
		maps[node->y][node->x] = EMPTY;

		node = nodes.top();
		nodes.pop();

		maps[node->y][node->x] = PLAYER;

		system("cls");
		Print();
		Sleep(500);
	}
}
