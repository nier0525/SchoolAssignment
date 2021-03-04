#pragma once

#include "global.h"

typedef int ElementType;

struct QueueListNode {
	ElementType data;
	QueueListNode* link;
};

struct QueueList {
	QueueListNode* front;
	QueueListNode* back;
};

void AQL_CreateQueueList(QueueList**);
void AQL_FreeQueueList(QueueList*);
void AQL_EnQueue(QueueList*, ElementType);

ElementType AQL_GetFront(QueueList*);
ElementType AQL_DeQueue(QueueList*);

int AQL_isEmpty(QueueList*);
