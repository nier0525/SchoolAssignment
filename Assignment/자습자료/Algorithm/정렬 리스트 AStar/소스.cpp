#include "Global.h"
using namespace std::chrono;
using namespace std;

// ���� ���, ���� ��� ����
vector<AStar_Node*> open_nodes;
vector<AStar_Node*> close_nodes;

// �� ����
int maps[MAP_Y][MAP_X];

// G ����� ���� ��ġ���� ���� ��ġ���� �ʿ��� �Ҹ� ���
// �̵� ������ �밢���� ��� 14
// ������ ��� 10
int G_Score(__in const AStar_Node* _parent, __in const AStar_Node* _child)
{
	// ���밪���� ���� ��ġ�� �̵� �� ��ġ�� ���� ����
	int x = abs(_parent->x - _child->x);
	int y = abs(_parent->y - _child->y);

	// �� ���� 2 �� ��� �밢���̰�, �� ���ϸ� ������
	return (x + y == 2) ? _parent->G + 14 : _parent->G + 10;
}

// H ����� ���� ��ġ���� Ÿ�� ��ġ���� �ʿ��� �Ҹ� ���
// ��ֹ��� ������� �ʰ� ���� �Ÿ����� ����
// ���ϴ� ������ ���� ����� ����
// �Ʒ� ������ �밢���� ������ ����, ���� ���ĸ� ���� ����
int H_Score(__in const AStar_Node* _path, __in const int _end_x, __in const int _end_y)
{
	// ���밪���� ���� ��ġ�� Ÿ���� ��ġ�� ���� ����
	int x = abs(_path->x - END_X);
	int y = abs(_path->y - END_Y);

	// ����,���� ���� �� �� ����� ���⿡ �´� ���� ����
	return (x < y) ? (x * 7 + (y - x) * 5) : (y * 7 + (x - y) * 5);
}

// F ����� ���� ��ġ���� Ÿ�� ��ġ���� ���� ���� �ʿ��� �� �Ҹ� ���
// F = G + H
int F_Score(__in const AStar_Node* _path)
{
	return _path->G + _path->H;
}

// ��ã�� �˰���
void PathFinding_Setting(__in const int, __in const int, __in const int, __in const int);
AStar_Node* PathFinding(__in const int, __in const int);

// �� �׸���
void Print();

// �� ��ġ�� �ֺ� ��� Ž�� ( 3 * 3 )
vector<AStar_Node*> GetNeighbours(__in const AStar_Node* _node);

// �Ϸ�� �н��� Ž��
stack<AStar_Node*> SearchPath(__in AStar_Node* _node);

void Move(__in int start_x, __in int start_y, __in int end_x, __in int end_y);
void Animation(__in int start_x, __in int start_y, __in int end_x, __in int end_y);

int main()
{
	// ��
	// �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� 
	// �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� 
	// �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� 
	// �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� 
	// �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� 
	// �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� 
	// �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� 
	// �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� 
	// �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� 
	// �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� 

	//Move(1, 1, END_X, END_Y);
	Animation(1, 1, END_X, END_Y);
}

void PathFinding_Setting(__in const int _start_x, __in const int _start_y, __in const int _end_x, __in const int _end_y)
{
	// �� ���� �ʱ�ȭ
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

	// �� ���� ������ ī��
	memcpy(&maps, &temp_maps, sizeof(maps));

	// Ÿ�� ��ġ ����
	maps[_end_y][_end_x] = TARGET;

	// ���� ��ġ ����
	AStar_Node* node = new AStar_Node(_start_x, _start_y);
	node->G = 0;
	node->H = H_Score(node, _end_x, _end_y);
	node->F = F_Score(node);

	// ���� ��� ���Ϳ� �߰�
	open_nodes.push_back(node);
}

void Print()
{
	// �� ���ҿ� �´� ���ڸ� ��ġ���� ���
	for (int i = 0; i < MAP_Y; i++) {
		for (int j = 0; j < MAP_X; j++) {
			switch (maps[i][j]) {
			case EMPTY:
				printf("��");
				break;

			case WALL:
				printf("��");
				break;

			case PATH:
				printf("��");
				break;

			case TARGET:
				printf("��");
				break;

			case PLAYER:
				printf("��");
				break;
			}
		}
		printf("\n");
	}
}

vector<AStar_Node*> GetNeighbours(__in const AStar_Node* _node)
{
	vector<AStar_Node*> neighbours;

	// �Ű������� ���� ������ �Ǵ� ��� ��ġ�� �������� �ֺ� ��带 Ž��
	// -1 ~ 1 �� ���� �ֺ� ����� 3 * 3 ������ Ž��
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			// 0,0 �� ������ �Ǵ� ����� ��ġ�̱� ������ �߰� �� �ʿ䰡 �����Ƿ� ���� ó��
			if (0 == x && 0 == y) continue;
			
			// check_x, check_y = ������ �Ǵ� ��� ��ġ + �ֺ� ��� ��ġ
			bool flag = false;
			int check_x = _node->x + x;
			int check_y = _node->y + y;

			// �Ʒ� �� ������ �����ϴ� ��츸 �ֺ� ��� ���Ϳ� �߰��Ѵ�.
			// 1. ����� ��ġ�� ���� ������ ����� �ʴ´�.
			// 2. ����� ��ġ�� ���� ����.
			// 3. ��尡 ���� ��忡 ���ԵǾ� ���� �ʴ�.			
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

	// ������ �ֺ� ��带 ��ȯ�Ѵ�.
	return neighbours;
}

AStar_Node* PathFinding(__in const int _end_x, __in const int _end_y)
{
	// ���� ��尡 ��ٸ� �ݺ����� �����Ѵ�.
	// �� ��쿣 ��ã�� Ž���� �����ߴٴ� �ǹ̰� �ȴ�.
	while (!open_nodes.empty())
	{
		// ���� ��� ������ ���� �տ� �ִ� ��带 �����´�.
		// �� ���ʹ� F ���� �������� ������������ ������ �Ǿ� �ִ�.
		// ���� ���� ����� ���� ��带 ������ �� �� �ִ�.
		AStar_Node* current_node = open_nodes.front();

		// ������ �� ��� ������ ���� ��忡�� �����Ѵ�.
		// �켱���� ť�� ����ϸ� �� ������ ���� ��� ��ȸ�� ���� ���Ϳ� ������ ����Ͽ���.
		// �� ������ �̿��� ����Ʈ�� ����ϸ� ������ �� �������� �ִ�.
		open_nodes.erase(open_nodes.begin());

		// ���� �� ����� ��ġ�� Ÿ�� ��ġ��� Ÿ���� ã�Ҵٴ� ����̴�.
		// ������ ��带 ��ȯ�� �� �Լ��� �����Ѵ�.
		if (_end_x == current_node->x && _end_y == current_node->y)	
			return current_node;
		

		// ���� ��� ������ ���� ��忡 �߰��Ѵ�.
		close_nodes.push_back(current_node);

		// ���� ��� �ֺ� ��带 Ž���� �� ������ ������ �´�.
		vector<AStar_Node*> neighbourNodes = GetNeighbours(current_node);

		// �ֺ� ��带 Ž���Ѵ�.
		for (int i = 0; i < neighbourNodes.size(); i++)
		{			
			bool visit = false;
			// �ֺ� ��带 �ϳ��� ����
			AStar_Node* search_node = neighbourNodes[i];

			// ���� ��� ��ġ�� ���õ� �ֺ� ��� ��ġ ���� ���Ѵ�.
			int distance = G_Score(current_node, search_node);

			// ���õ� ��尡 ���� ��� ���� ���� �����ϴ� �� �˻�
			for (AStar_Node* node : open_nodes)
			{
				if (node->x == search_node->x && node->y == search_node->y)
				{
					visit = true;
					break;
				}
			}

			// ���õ� ��尡 ���� ��� ���Ϳ� ���ԵǾ� ���� ���� ���
			if (/*distance < search_node->G ||*/ !visit)
			{
				// ����� �Ҹ� ��� ������ ����
				search_node->G = distance;
				search_node->H = H_Score(search_node, _end_x, _end_y);
				search_node->F = F_Score(search_node);

				// ���õ� �ֺ� ����� �θ� ��带 ���� ���� ����
				search_node->parent = current_node;

				// ���õ� ��带 ���� ��� ���Ϳ� �߰�
				open_nodes.push_back(search_node);

				// F ���� �������� ���� ��� ���͸� ������������ ����
				sort(open_nodes.begin(), open_nodes.end(), compare);

				// ���� ���� ��� ���� ���õ� ��尡 �����ϴ� ���
				// ���� ���� ���õ� ����� G ���� ���Ѵ�.
				// ���� ����� G ���� ���� ���
				// ���� ����� �θ� ��尡 �ƴ� ���õ� ����� �θ� ��带 ���� ���� �����Ѵ�.
				// �� �� ���õ� ����� H, G, F ���� �ٽ� �����Ѵ�.
				// �� ������ ���� �ڵ忡���� �ϴ�.. ���Ҵ�..

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
	// �Ű� ������ ���� ���� Ÿ�� ��ġ���� ������ ���
	// �� ����� �θ� ���� ���� ��ġ�� ����̴�.
	AStar_Node* node = _node;
	stack<AStar_Node*> nodes;

	while (nullptr != node)
	{
		nodes.push(node);
		node = node->parent;
	}	

	// �ּ� ������� �����Ͽ��� ������ �θ� ��带 ���󰡸� ������ ��ΰ� ���´�.
	// ���������� ������ ��θ� ���� �� �ְ� �ȴ�.
	//while (nullptr != node->parent)
	//{
	//	maps[node->y][node->x] = PATH;
	//	node = node->parent;
	//}

	//// �÷��̾ ǥ���ϱ� ���� ���� ó��
	//if (nullptr != node)
	//	maps[node->y][node->x] = PLAYER;

	// ���� ���, ���� ��� �ʱ�ȭ
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
			cout << "��ã�� ����" << endl;
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
		cout << endl << "����Ű : �̵�" << endl << "Enter : ����" << endl;

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
