#include "MainManager.h"

char cMain::_ID[] = { "koreait" };
char cMain::_PASS[] = { "1234" };

cMain::cMain() {
	mId = nullptr;
	mBook = nullptr;
}
cMain::~cMain() {
	if (mId != nullptr) {
		delete mId;
	}
	if (mBook != nullptr) {
		delete mBook;
	}
};

void cMain::initialize() {
	mId = new cIDManager();
	mBook = new cBookManager();
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
			ManagerID();
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
		cout << "1. 도서 검색" << endl << "2. 도서 대여" << endl << "3. 도서 반납" << endl << "4. 로그아웃" << endl
			<< "5. 회원탈퇴" << endl;
		cout << "선택 : ";
		int sel;
		cin >> sel;
		cout << " =============================================== " << endl;

		switch(sel) {
		case BOOKSERACH:
			mBook->Serach();
			break;
		case BOOKINPUT:
			mBook->Output();
			break;
		case BOOKRETURN:
			mBook->Return();
			break;
		case ROGOUT:
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

void cMain::ManagerID() {
	char id[ID];
	char pass[ID];

	cout << "ID : ";	cin >> id;
	cout << "PASSWORD : ";	cin >> pass;

	if (!strcmp(_ID, id) && !strcmp(_PASS, pass)) {
		while (1) {
			cout << " =============================================== " << endl;
			cout << "< 관리자 메뉴 >" << endl << "1. 도서 추가" << endl << "2. 도서 삭제" << endl << "3. 종료" << endl;
			cout << "선택 : ";
			int sel;
			cin >> sel;
			cout << " =============================================== " << endl;

			bool endflug = false;

			switch (sel)
			{
			case INPUT:
				mBook->NewBook();
				break;
			case OUTPUT:
				mBook->DeleteBook();
				break;
			case RETURN:
				endflug = true;
				break;
			default:
				cout << "잘못 입력하셨습니다." << endl;
				continue;
			}
			if (endflug) {
				break;
			}
			break;
		}
	}
	else {
		cout << "ID 또는 PASSWORD 가 맞지 않습니다." << endl;
	}
}