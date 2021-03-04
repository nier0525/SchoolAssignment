#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include "Node.h"

class NodeManager {
private:
	ListNode<int, LISTSIZE>*	intL;
	ListNode<double, LISTSIZE>* doubleL;
	ListNode<char, LISTSIZE>*	charL;
public:
	NodeManager();
	~NodeManager();

	void StartData();
	int SelMenu();

	void intList();
	void doubleList();
	void charList();
};

template <typename T>
T SelCount() {
	T sel;
	cout << "°ª(»ðÀÔ) : "; cin >> sel;
	return sel;
}
#endif