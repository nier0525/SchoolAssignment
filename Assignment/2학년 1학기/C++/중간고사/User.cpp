#include "User.h"

CUser::CUser(const char* _id, const char* _pass) {
	strcpy(id, _id);
	strcpy(pass, _pass);
	money = GAMEMONEY;		// 유저의 돈을 지정된 디폴트 값으로 초기화
	count = NODATA;			// 유저의 캐릭터 보유수 NODATA
}

CUser::~CUser() {
	delete UserCharList, UserItemList;		// 리스트 메모리 반환
	delete UserChar, UserItem;				// 노드 메모리 반환
}

void CUser::Init() {
	UserCharList = new ListNode<Character*>();  // 캐릭터 리스트 동적 할당
	UserChar = nullptr;							// 캐릭터 노드 초기화
	UserItemList = new ListNode<Item*>();		// 아이템 리스트 동적 할당
	UserItem = nullptr;							// 아이템 노드 초기화
}

// 쓰기 접근자 함수

void CUser::Setid(const char* _id) {
	strcpy(id, _id);
}

void CUser::Setpass(const char* _pass) {
	strcpy(pass, _pass);
}

void CUser::Setmoney(int _money) {
	money = _money;
}

void CUser::Setcount(int _count) {
	count = _count;
}

// 읽기 접근자 함수

const char* CUser::Getid() {
	return id;
}

const char* CUser::Getpass() {
	return pass;
}

int CUser::Getmoney() {
	return money;
}

int CUser::Getcount() {
	return count;
}

// 캐릭터 생성,삭제 및 아이템 구입,판매시 조건 체크 함수

bool CUser::CheakMoney(int _money) {
	if (money - _money >= NODATA) {
		return true;
	}
	else {
		return false;
	}
}

bool CUser::CheakCharCount() {
	if (count < MAXCOUNT) {
		return true;
	}
	else {
		return false;
	}
}

// 캐릭터에 관한 함수
void CUser::SetChar(Node<Character*>* _char) {	// 캐릭터 노드 쓰기
		UserChar = _char;
}

Node<Character*>* CUser::GetChar() {		// 캐릭터 노드 읽기
	return UserChar;
}

Node<Character*>* CUser::Gethead() {		// 캐릭터 리스트의 헤드 노드 읽기
	return UserCharList->GetHead();
}

void CUser::New(Character* _char) {			// 캐릭터 생성
	UserCharList->Input(_char);
	count++;
}

void CUser::Del(Node<Character*>* _char) {	// 캐릭터 삭제
	UserCharList->Delete(_char);
	count--;
}

// 아이템에 관한 함수

void CUser::SetItem(Node<Item*>* _item) {	// 아이템 노드 쓰기
	UserItem = _item;
}

Node<Item*>* CUser::GetItem() {				// 아이템 노드 읽기
	return UserItem;
}

Node<Item*>* CUser::GetItemhead() {			// 아이템 리스트의 헤드 노드 읽기
	return UserItemList->GetHead();
}

void CUser::Buy(Item* _item) {				// 아이템 첫 구매 시 노드 생성 함수
	UserItemList->Input(_item);
}

void CUser::Sell(Node<Item*>* _item) {		// 아이템 판매 후 아이템이 없을 시 노드 삭제 함수
    UserItemList->Delete(_item);
}
