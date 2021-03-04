#pragma once

#include "User.h"
#include "ListNode.h"

class CLoginManager {
private:
	ListNode<CUser*>* ListUser;	// 유저 객체를 저장할 리스트
	Node<CUser*>* NowUser;		// 접속중인 유저를 지칭할 노드
public:
	CLoginManager();
	~CLoginManager();

	void Init();	// 초기화 작업

	void New();		// 유저 객체 생성
	bool Login();	// 로그인 체크
	void Logout();	// 로그아웃 체크
	void Del();		// 유저 객체 삭제

	Node<CUser*>* GetNowUser() { return NowUser; }
	// 현재 접속한 유저의 정보를 가진 노드를 외부에 읽을 수 있게 하는 읽기 접근자
};