#pragma once
#include "Global.h"

struct PriorityQueue {
	int Capacity;
	int Front;
	int Rear;

	Path* path;
};

void PQ_CreateQueue(PriorityQueue** Queue, int Capacity) { 
	(*Queue) = (PriorityQueue*)malloc(sizeof(PriorityQueue));
	(*Queue)->path = (Path*)malloc(sizeof(Path) * (Capacity + 1));
	(*Queue)->Capacity = Capacity;

	(*Queue)->Front = 0;
	(*Queue)->Rear = 0;
}

void PQ_DestroyQueue(PriorityQueue* Queue) {
	free(Queue->path);
	free(Queue);
}

int PQ_IsEmpty(PriorityQueue* Queue) {
	return (Queue->Front == Queue->Rear);
}

int PQ_IsFull(PriorityQueue* Queue) {
	return (Queue->Front == ((Queue->Rear + 1) % Queue->Capacity));
}

void PQ_Sort(PriorityQueue* Queue) {
	for (int i = Queue->Rear; i > Queue->Front + 1; i--) {
		for (int j = Queue->Front + 1; j < i; j++) {
			if (Queue->path[j].F > Queue->path[j + 1].F) {
				Path temp = Queue->path[j];
				Queue->path[j] = Queue->path[j + 1];
				Queue->path[j + 1] = temp;
			}
		}
	}

	//for (int i = Queue->Front + 1; i <= Queue->Rear; i++) {
	//	for (int j = Queue->Front + 1; j <= Queue->Rear - i; j++) {
	//		if (Queue->path[j].F > Queue->path[j + 1].F && Queue->Front != 0) {
	//			Path Temp = Queue->path[j];
	//			Queue->path[j] = Queue->path[j + 1];
	//			Queue->path[j + 1] = Temp;
	//		}
	//	}
	//}
}

void PQ_Enqueue(PriorityQueue* Queue, Path Data) {
	if (PQ_IsFull(Queue)) return;

	Queue->Rear = (Queue->Rear + 1) % (Queue->Capacity + 1);
	Queue->path[Queue->Rear] = Data;
	PQ_Sort(Queue);
}

Path PQ_Dequeue(PriorityQueue* Queue) {
	if (PQ_IsEmpty(Queue)) {
		Path Empty;
		ZeroMemory(&Empty, sizeof(Path));
		return Empty;
	}

	Queue->Front = (Queue->Front + 1) % (Queue->Capacity + 1);
	return Queue->path[Queue->Front];
}