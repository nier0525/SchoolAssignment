#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <conio.h>

#include <chrono>

// 맵 크기
#define MAP_X 20
#define MAP_Y 10

// 타겟 위치
#define END_X 18
#define END_Y 8

// 빈 공간, 벽, 패스, 타겟, 플레이어
#define EMPTY 0
#define WALL 1
#define PATH 2
#define TARGET 3
#define PLAYER 4

// 키 입력 값
enum INPUT_KEY
{
	UP = 72, 
	DOWN = 80, 
	LEFT = 75, 
	RIGHT = 77, 
	ENTER = 13
};

// A* 노드 데이터
struct AStar_Node
{
	// 위치값
	int x, y;

	// F, G, H 비용 값
	int F, G, H;

	// 부모 노드
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

// 정렬 사용자 정의
bool compare(__in const AStar_Node* _a, __in const AStar_Node* _b)
{
	return _a->F < _b->F;
}

//// 우선순위 큐 정렬 사용자 정의
//bool operator <(__in const AStar_Node _a, __in const AStar_Node _b)
//{
//	return _a.F > _b.F;
//}