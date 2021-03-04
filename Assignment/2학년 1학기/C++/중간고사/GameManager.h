#pragma once

#include "LoginManager.h"
#include "CharacterManager.h"
#include "ItemManager.h"

// 최상위 매니저 클래스
class GameManager {
private:
	CLoginManager* LM;		// 로그인 매니저
	CharacterManager* CM;	// 캐릭터 매니저
	ItemManager* IM;		// 아이템 매니저
public:
	GameManager();
	~GameManager();

	void Init(); // 초기화 작업
	void Run();  // 전체적인 실행 작업

	int LoginMenu(); // 로그인 메뉴
	int GameMenu();  // 게임 메뉴
};