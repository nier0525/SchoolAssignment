#pragma once

#include "global.h"

typedef int  ElementType;

struct StackListNode {
	ElementType data;
	StackListNode* link;
};

struct StackList {
	StackListNode* top;
	StackListNode* node;
};

void ASL_CreateStackList(StackList**);
void ASL_FreeStackList(StackList*);

void ASL_Push(StackList*, ElementType);

ElementType ASL_Pop(StackList*);
ElementType ASL_Top(StackList*);
