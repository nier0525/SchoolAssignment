#include "Stack.h"
#include "Stack_list.h"

#include "Queue.h"
#include "Queue_list.h"

#include "List.h"

int main() {
	StackList* list = nullptr;

	ASL_CreateStackList(&list);

	ASL_Push(list, 10);
	ASL_Push(list, 20);

	printf("Pop : %d\n", ASL_Pop(list));
	printf("Pop : %d\n", ASL_Pop(list));

	ASL_FreeStackList(list);
	system("pause");
	return 0;
}