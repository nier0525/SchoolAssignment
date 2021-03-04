#pragma once

#include "global.h"

typedef int ElementType;

typedef struct QueueNode {
	ElementType data;
}Q_Node;

typedef struct QueueArray {
	int size;
	int front, back;
	Q_Node* node;
}Queue;

void AQ_CreateQueue(Queue**, int);
void AQ_FreeQueue(Queue*);
void AQ_EnQueue(Queue*, ElementType);
void AQ_isClean(Queue*);

ElementType AQ_DeQueue(Queue*);
ElementType AQ_Front(Queue*);

int AQ_GetSize(Queue*);

bool AQ_isEmpty(Queue*);
bool AQ_isFull(Queue*);