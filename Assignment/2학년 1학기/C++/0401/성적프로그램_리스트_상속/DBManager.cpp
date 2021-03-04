#include "DBManager.h"

DBManager::DBManager() {
	list = new ListNode<CStudent*>();
	NowStudent = nullptr;
}

DBManager::~DBManager() {

}
