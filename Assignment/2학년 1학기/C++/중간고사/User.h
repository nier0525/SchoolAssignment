#pragma once

#include "Elf.h"
#include "Oak.h"
#include "Human.h"

#include "Portion.h"
#include "Armor.h"
#include "Wenpon.h"

#include "ListNode.h"

class CUser {
private:
	char id[CHARSIZE];		// ID
	char pass[CHARSIZE];	// PASSWORD
	int money;				// 유저가 보유한 돈
	int count;				// 유저가 보유한 캐릭터 수
	bool state;				// 유저의 로그인 현황

	ListNode<Character*>* UserCharList;	// 유저가 보유한 캐릭터 리스트
	Node<Character*>* UserChar;			// 캐릭터 리스트를 순회하기 위한 노드

	ListNode<Item*>* UserItemList;		// 유저가 보유한 아이템 리스트
	Node<Item*>* UserItem;				// 아이템 리스트를 순회하기 위한 노드
public:
	CUser(const char*, const char*);	// 생성자 (아이디,비번)
	~CUser();							// 소멸자

	// 리스트 객체 생성 함수
	void Init();

	// 쓰기 접근자
	void Setid(const char*);		
	void Setpass(const char*);
	void Setmoney(int);
	void Setcount(int);

	// 읽기 접근자
	const char* Getid();
	const char* Getpass();
	int Getmoney();
	int Getcount();

	// 로그인 체크
	void Login() { state = true; }
	void Logout() { state = false; }

	// 아이템 구입,판매 및 캐릭터 생성,삭제 시 조건 체크
	bool CheakMoney(int);		// 유저의 돈 + 지출해야 할 돈이 0 보다 크면 true
	bool CheakCharCount();		// 캐릭터 생성수가 지정한 디폴트값 이하면 true

	// 캐릭터 읽기,쓰기 접근자 멤버 함수
	void SetChar(Node<Character*>*); // 캐릭터 노드 쓰기
	Node<Character*>* Gethead();	// 캐릭터 리스트의 헤드 노드 읽기
	Node<Character*>* GetChar();	// 캐릭터 노드 읽기

	void New(Character*);			// 생성
	void Del(Node<Character*>*);	// 삭제

	// 아이템 읽기,쓰기 접근자 멤버 함수
	void SetItem(Node<Item*>*); // 아이템 노드 쓰기
	Node<Item*>* GetItemhead(); // 아이템 리스트의 헤드 노드 읽기
	Node<Item*>* GetItem();	    // 아이템 노드 읽기

	void Buy(Item*);			// 아이템 첫 구입 시 리스트 삽입 연산
	void Sell(Node<Item*>*);	// 아이템 판매 후 보유한 아이템이 없을 시 삭제 연산
};