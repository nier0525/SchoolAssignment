#include "Queue.h"

void AQ_CreateQueue(Queue** q, int size) {
	(*q) = (Queue*)malloc(sizeof(Queue));
	(*q)->node = (Q_Node*)malloc(sizeof(Q_Node) * (size + 1));

	(*q)->size = size;
	(*q)->back = EMPTY;
	(*q)->front = EMPTY;
}

void AQ_FreeQueue(Queue* q) {
	free(q->node);
	free(q);
}

void AQ_EnQueue(Queue* q, ElementType data) {
	if (AQ_isFull(q)) {
		return;
	}

	q->back = (q->back + 1) % (q->size + 1);
	q->node[q->back].data = data;
}

void AQ_isClean(Queue* q) {
	q->front = EMPTY;
	q->back = EMPTY;
}

ElementType AQ_DeQueue(Queue* q) {
	if (AQ_isEmpty(q)) {
		return ERROR;
	}
	else {
		q->node[q->front].data = EMPTY;
		q->front = (q->front + 1) % (q->size + 1);
		return q->node[q->front].data;

		//ElementType value = q->node[++q->front].data;
		//if (q->front < q->size) {
		//	q->node[q->front].data = EMPTY;
		//}

		//return value;
	}
}

ElementType AQ_Front(Queue* q) {
	return q->node[q->front + 1].data;
}

int AQ_GetSize(Queue* q) {
	return q->back;
}

bool AQ_isEmpty(Queue* q) {
	if (q->front == q->back) {
		return true;
	}
	
	return false;
}

bool AQ_isFull(Queue* q) {
	if (((q->back + 1) % q->size) == q->front) {
		return true;
	}
	
	return false;
}