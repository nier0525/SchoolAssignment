#include "Stack_list.h"

void ASL_CreateStackList(StackList** s) {
	(*s) = new StackList();
	(*s)->top = nullptr;
}

void ASL_FreeStackList(StackList* s) {
	delete s;
}

void ASL_Push(StackList* s, ElementType data) {
	StackListNode* node = new StackListNode();
	node->data = data;
	node->link = s->top;
	s->top = node;
}

ElementType ASL_Pop(StackList* s) {
	ElementType data;
	StackListNode* ptr = s->top;

	if (s->top == nullptr) {
		printf("\nStack is Empty\n");
		return ERROR;
	}

	data = ptr->data;
	s->top = ptr->link;
	delete ptr;
	return data;
}

ElementType ASL_Top(StackList* s) {
	return s->top->data;
}
