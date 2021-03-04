#include "GameManager.h"

// 생성자 멤버 변수 초기화
GameManager::GameManager() {
	LM = nullptr;
	CM = nullptr;
	IM = nullptr;
}

// 소멸자 메모리 반환
GameManager::~GameManager() {
	delete LM, CM, IM;
}

// 초기화 작업을 위한 함수
void GameManager::Init() {
	LM = new CLoginManager(); // 로그인 매니저 동적 할당
	LM->Init(); // 로그인 매니저 초기화 함수
	CM = new CharacterManager(); // 캐릭터 매니저 동적 할당
	IM = new ItemManager(); // 아이템 매니저 동적 할당
	IM->Init(); // 아이템 매니저 초기화 함수
}

// 로그인 메뉴
int GameManager::LoginMenu() {
	while (1) {
		int sel;
		cout << "< 메뉴 >" << endl << "1. 회원가입" << endl << "2. 로그인" << endl << "3. 종료" << endl;
		cout << "선택 : ";
		cin >> sel;
		if (cin.failbit) {	// cin 으로 입력한 sel 값이 int 형이 아니라면 오류 처리
			cin.clear();
			cin.ignore(256, '\n');
		}
		if (sel < 1 || sel > 3) { // 입력 받은 sel 값이 지정된 메뉴 값의 범위를 벗어날 경우
			cout << "< Error >" << endl;
			continue; // 다시 시작
		}
		return sel; // sel 값 리턴
	}
}

// 게임 메뉴
// 로그인 메뉴와 동일
int GameManager::GameMenu() {
	while (1) {
		int sel;
		cout << "< 메뉴 >" << endl 
			<< "1. 캐릭터 생성" << endl << "2. 캐릭터 삭제" << endl
			<< "3. 아이템 구입" << endl << "4. 아이템 판매" << endl
			<< 	"5. 로그아웃" << endl << "6. 회원 탈퇴" << endl;
		cout << "선택 : ";
		cin >> sel;
		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}
		if (sel < 1 || sel > 6) {
			cout << "< Error >" << endl;
			continue;
		}
		return sel;
	}
}

void GameManager::Run() {
	Init();	 // 객체 생성을 위한 초기화
	while (1) {
		switch (LoginMenu()) {
		case NEWLOG:
			LM->New(); // 로그인 매니저에서 New 함수 실행
			break;

		case LOGIN:
			if (LM->Login()) { // 로그인 매니저에서 Login 함수가 true 일 경우
				while (1) {
					bool flug = false; // while 종료 여부
					switch (GameMenu()) {
						// 매게 변수로 넘겨주는 것은 현재 접속 중인 유저의 객체 정보
					case NEW:
						CM->New(LM->GetNowUser()); // 캐릭터 매니저 New 함수 실행 
						break;

					case DEL:
						CM->Del(LM->GetNowUser()); // 캐릭터 매니저 Del 함수 실행
						break;

					case BUY:
						IM->Buy(LM->GetNowUser()); // 아이템 매니저 Buy 함수 실행
						break;

					case SELL:
						IM->Sell(LM->GetNowUser()); // 아이템 매니저 Sell 함수 실행
						break;

					case LOGOUT:
						LM->Logout(); // 로그인 매니저 Logout 함수 실행
						flug = true;  // while 종료
						break;

					case DELLOG:
						LM->Del();	// 로그인 매니저 Del 함수 실행
						flug = true;	// while 종료
						break;
					}
					if (flug) { // flug 가 true 면
						break; // whlie 종료
					}
				}
			}
			break;

		case EXIT:
			return; // 함수 종료 ( 프로그램 종료 )
			break;
		}
	}
}