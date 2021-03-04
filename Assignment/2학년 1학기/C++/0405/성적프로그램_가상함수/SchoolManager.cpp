#include "SchoolManager.h"

CSchoolManager::CSchoolManager() {
	StudentList = new ListNode<CStudent*>();
	NowStudent = nullptr;
}

CSchoolManager::~CSchoolManager() {

}