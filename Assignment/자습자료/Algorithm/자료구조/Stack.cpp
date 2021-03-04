#include "Stack.h"

void AS_CreateStack(Stack** s, int size) {
	(*s) = (Stack*)malloc(sizeof(Stack));
	(*s)->node = (S_Node*)malloc(sizeof(S_Node) * size);

	(*s)->size = size;
	(*s)->top = EMPTY;
}

void AS_FreeStack(Stack* s) { 
	free(s->node);
	free(s);
}

void AS_Push(Stack* s, ElementType data) {
	int pos = s->top;

	if (pos < s->size) {
		s->node[pos].data = data;
		s->top++;
	}
	else {
		printf("\nStack is Full\n");
	}
}

ElementType AS_Pop(Stack* s) {
	if (AS_IsEmpty(s)) {
		return ERROR;
	}

	else {
		int pos = --(s->top);
		return s->node[pos].data;
	}
}

ElementType AS_Top(Stack* s) {
	int pos = s->top - 1;

	return s->node[pos].data;
};

int AS_GetSize(Stack* s) { 
	return s->top;
}

void AS_isClean(Stack* s) {
	s->top = EMPTY;
}

bool AS_IsEmpty(Stack* s) {
	if (s->top <= EMPTY) {
		return true;
	}
	else {
		return false;
	}
}
