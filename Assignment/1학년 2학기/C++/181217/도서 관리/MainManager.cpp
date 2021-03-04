#include "MainManager.h"

cMain::cMain() {
	mId = nullptr;
	mBook = nullptr;
}
cMain::~cMain() {
	delete mId;
	delete mBook;
};

void cMain::initialize() {
	mId = new cIDManager();
	mBook = new cBookManager();

	mBook->initelize();
}

void cMain::Menu() {

	initialize();

	while (1) {
		cout << " =============================================== " << endl;
		cout << "< 메 뉴 >" << endl;
		cout << "1. 회원가입" << endl << "2. 로그인" << endl << "3. 관리자 모드" << endl << "4. 종료" << endl;

		int sel;
		cout << "선택 : ";	cin >> sel;
		cout << " =============================================== " << endl;

		switch (sel) {
		
		case NEW:
			mId->New();
			break;
	
		case ROGIN:
			if (mId->Rogin()) {
				BookMenu();
			}
			break;

		case HOST:
			mBook->HostMode();
			break;
		
		case EXIT:
			cout << "< 종료 >" << endl;
			return;
		
		default:
			cout << "잘못 입력하셨습니다." << endl;
			continue;
		}
	}
}

void cMain::BookMenu() {

	bool endflug = false;

	while (1) {
		cout << " =============================================== " << endl;
		cout << "< 도서 대여 >" << endl;
		cout << "1. 도서 조회" << endl << "2. 도서 검색" << endl << "3. 도서 대여" << endl <<
			"4. 도서 반납" << endl << "5. 로그아웃" << endl << "6. 회원탈퇴" << endl;
		cout << "선택 : ";
		int sel;
		cin >> sel;
		cout << " =============================================== " << endl;

		switch(sel) {
		case DISPLAY:
			mBook->Display();
			break;
		case BOOKSERACH:
			mBook->Serach();
			break;
		case BOOKINPUT:
			mBook->Output(mId->GetNowUser());
			break;
		case BOOKRETURN:
			mBook->Return(mId->GetNowUser());
			break;
		case ROGOUT:
			mId->Rogout();
			endflug = true;
			break;
		case GETROG:
			mId->DeleteRog();
			endflug = true;
			break;

		default:
			cout << "잘못 입력하셨습니다." << endl;
			continue;
		}
		if (endflug) {
			break;
		}
	}
}