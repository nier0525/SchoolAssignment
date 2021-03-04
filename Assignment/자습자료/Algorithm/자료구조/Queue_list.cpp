#include "Queue_list.h"

void AQL_CreateQueueList(QueueList** Q) {
	(*Q) = (QueueList*)malloc(sizeof(QueueList));
	(*Q)->front = nullptr;
	(*Q)->back = nullptr;
}

void AQL_FreeQueueList(QueueList* Q) {
	free(Q);
}

void AQL_EnQueue(QueueList* Q, ElementType data) {
	QueueListNode* newNode = (QueueListNode*)malloc(sizeof(QueueListNode));
	newNode->data = data;
	newNode->link = nullptr;

	if (Q->front == nullptr) {
		Q->front = newNode;
		Q->back = newNode;
	}
	else {
		Q->back->link = newNode;
		Q->back = newNode;
	}
}

ElementType AQL_GetFront(QueueList* Q) {
	if (AQL_isEmpty(Q) != ERROR) {
		return Q->front->data;
	}
	else {
		return ERROR;
	}
}

int AQL_isEmpty(QueueList* Q) {
	if (Q->front == nullptr) {
		printf("\nQueue is Empty\n");
		return ERROR;
	}

	return 0;
}

ElementType AQL_DeQueue(QueueList* Q) {
	QueueListNode* ptr = Q->front;
	ElementType data;
	
	if (AQL_isEmpty(Q) == ERROR) {
		return ERROR;
	}

	else {
		data = ptr->data;
		Q->front = Q->front->link;
		
		if (Q->front == nullptr) {
			Q->back = nullptr;
		}

		free(ptr);
		return data;
	}
}