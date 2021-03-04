#pragma once

#include "global.h"

typedef int ElementType;

typedef struct StackNode {
	ElementType data;
}S_Node;

typedef struct StackArray {
	int size;
	int top;
	S_Node* node;
}Stack;

void AS_CreateStack(Stack**, int);
void AS_FreeStack(Stack*);
void AS_Push(Stack*, ElementType);
void AS_isClean(Stack*);

ElementType AS_Pop(Stack*);
ElementType AS_Top(Stack*);

int AS_GetSize(Stack*);

bool AS_IsEmpty(Stack*);

