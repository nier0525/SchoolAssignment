#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <conio.h>

#include <chrono>

// �� ũ��
#define MAP_X 20
#define MAP_Y 10

// Ÿ�� ��ġ
#define END_X 18
#define END_Y 8

// �� ����, ��, �н�, Ÿ��, �÷��̾�
#define EMPTY 0
#define WALL 1
#define PATH 2
#define TARGET 3
#define PLAYER 4

// Ű �Է� ��
enum INPUT_KEY
{
	UP = 72, 
	DOWN = 80, 
	LEFT = 75, 
	RIGHT = 77, 
	ENTER = 13
};

// A* ��� ������
struct AStar_Node
{
	// ��ġ��
	int x, y;

	// F, G, H ��� ��
	int F, G, H;

	// �θ� ���
	AStar_Node* parent;

	AStar_Node() = default;
	AStar_Node(int _x, int _y)
	{
		x = _x;
		y = _y;
		F = G = H = 0;
		parent = nullptr;
	}
	AStar_Node(int _x, int _y, int _f, int _g, int _h)
	{
		x = _x;
		y = _y;
		F = _f;
		G = _g;
		H = _h;
		parent = nullptr;
	}
};

// ���� ����� ����
bool compare(__in const AStar_Node* _a, __in const AStar_Node* _b)
{
	return _a->F < _b->F;
}

//// �켱���� ť ���� ����� ����
//bool operator <(__in const AStar_Node _a, __in const AStar_Node _b)
//{
//	return _a.F > _b.F;
//}