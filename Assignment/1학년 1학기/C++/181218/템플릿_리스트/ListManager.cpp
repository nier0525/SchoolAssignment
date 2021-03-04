#include "ListManager.h"

LinkedList<int>* L;
Node<int>* p;
LinkedList<double>* L2;
Node<double>* p2;
LinkedList<char>* L3;
Node<char>* p3;

NodeManager::NodeManager() {
	StartData();
}
NodeManager::~NodeManager() {
	delete intL;
	delete doubleL;
	delete charL;
}
void NodeManager::StartData() {
	intL = new ListNode<int, LISTSIZE>();
	doubleL = new ListNode<double, LISTSIZE>();
	charL = new ListNode<char, LISTSIZE>();
	L = intL->CreateNode();
	L2 = doubleL->CreateNode();
	L3 = charL->CreateNode();
}

int NodeManager::SelMenu() {

	cout << "======================================================" << endl;
	cout << "< 타입 별 리스트 메뉴 >" << endl;
	cout << "1. 첫번째 노드 삽입" << endl;
	cout << "2. 중간 노드 삽입" << endl;
	cout << "3. 마지막 노드 삽입" << endl;
	cout << "4. 노드 삭제" << endl;
	cout << "5. 노드 조회" << endl;
	cout << "6. 전체 노드 삭제" << endl;
	cout << "7. 종료" << endl;
	int sel;
	cout << "선택 : ";	cin >> sel;
	return sel;
}

void NodeManager::intList() {

	while (1) {
		int sel = SelMenu();
		bool flug = false;
		int dex;
		int temp;
		switch (sel) {
		case FIRST:
			dex = SelCount<int>();
			intL->FirstAdd(L, dex);
			break;

		case MID:
			temp;
			cout << "값(찾기) : "; cin >> temp;
			p = intL->Search(L, temp);
			if (p == nullptr) {
				cout << "값을 찾지 못했습니다." << endl;
				break;
			}
		    dex = SelCount<int>();
			intL->MiddleAdd(L, p, dex);
			break;

		case LAST:
			dex = SelCount<int>();
			intL->LastAdd(L, dex);
			break;

		case DEL:
			temp;
			cout << "값(찾기) : "; cin >> temp;
			p = intL->Search(L, temp);
			intL->Delete(L, p);
			break;

		case DIS:
			intL->Display(L);
			break;

		case ALLDEL:
			intL->Reset(L);
			break;

		case EXIT:
			flug = true;
			break;

		default:
			cout << "잘못 입력하셨습니다." << endl;
			continue;
		}
		if (flug) {
			break;
		}
	}
}

void NodeManager::doubleList() {

	while (1) {
		int sel = SelMenu();
		bool flug = false;
		double dex;
		double temp;
		switch (sel) {
		case FIRST:
			dex = SelCount<double>();
			doubleL->FirstAdd(L2, dex);
			break;

		case MID:
			temp;
			cout << "값(찾기) : "; cin >> temp;
			p2 = doubleL->Search(L2, temp);
			if (p2 == nullptr) {
				cout << "값을 찾지 못했습니다." << endl;
				break;
			}
			dex = SelCount<double>();
			doubleL->MiddleAdd(L2, p2, dex);
			break;

		case LAST:
			dex = SelCount<double>();
			doubleL->LastAdd(L2, dex);
			break;

		case DEL:
			temp;
			cout << "값(찾기) : "; cin >> temp;
			p2 = doubleL->Search(L2, temp);
			doubleL->Delete(L2, p2);
			break;

		case DIS:
			doubleL->Display(L2);
			break;

		case ALLDEL:
			doubleL->Reset(L2);
			break;

		case EXIT:
			flug = true;
			break;

		default:
			cout << "잘못 입력하셨습니다." << endl;
			continue;
		}
		if (flug) {
			break;
		}
	}
}

void NodeManager::charList() {

	while (1) {
		int sel = SelMenu();
		bool flug = false;
		char dex;
		char temp;
		switch (sel) {
		case FIRST:
			dex = SelCount<char>();
			charL->FirstAdd(L3, dex);
			break;

		case MID:
			temp;
			cout << "값(찾기) : "; cin >> temp;
			p3 = charL->Search(L3, temp);
			if (p3 == nullptr) {
				cout << "값을 찾지 못했습니다." << endl;
				break;
			}
			dex = SelCount<char>();
			charL->MiddleAdd(L3, p3, dex);
			break;

		case LAST:
			dex = SelCount<char>();
			charL->LastAdd(L3, dex);
			break;

		case DEL:
			temp;
			cout << "값(찾기) : "; cin >> temp;
			p3 = charL->Search(L3, temp);
			charL->Delete(L3, p3);
			break;

		case DIS:
			charL->Display(L3);
			break;

		case ALLDEL:
			charL->Reset(L3);
			break;

		case EXIT:
			flug = true;
			break;

		default:
			cout << "잘못 입력하셨습니다." << endl;
			continue;
		}
		if (flug) {
			break;
		}
	}

}