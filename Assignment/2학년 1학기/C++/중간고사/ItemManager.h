#pragma once

#include "Portion.h"
#include "Armor.h"
#include "Wenpon.h"

#include "ListNode.h"
#include "User.h"

// 아이템 구입,판매를 담당하는 매니저 클래스
// 상위 매니저 클래스에게 현재 접속중인 유저 값을 받음
// 받은 유저 객체에 아이템 매니저가 가진 객체의 정보를 전달
class ItemManager {
private:
	static const char* ItemName[]; // 아이템 목록

	static const char* PortionList[]; // 포션 종류
	static const char* ArmorList[];	  // 방어구 종류
	static const char* WenponList[];  // 무기 종류

	ListNode<Item*>* Itemlist[4];	// 포션,방어구,무기 각각의 아이템 리스트
	Node<Item*>* NowItem;			// 아이템을 순회할 노드
public:
	ItemManager();
	~ItemManager();

	int Menu();	// 포션,방어구,무기 선택 메뉴
	const char* ItemMenu(int);	// 아이템 상세 선택 메뉴

	void Init();	// 객체 생성을 위한 초기화 함수
	void Buy(Node<CUser*>*);	// 아이템 구입
	void Sell(Node<CUser*>*);	// 아이템 판매
};