#include "LoginManager.h"

// 생성자 초기화 작업
CLoginManager::CLoginManager() {
	ListUser = nullptr;
	NowUser = nullptr;
}

// 소멸자 반환 작업
CLoginManager::~CLoginManager() {
	if (ListUser->GetHead() != nullptr) {
		delete ListUser;
	}
	if (NowUser != nullptr) {
		delete NowUser;
	}
}

// 초기화 영억
void CLoginManager::Init() {
	ListUser = new ListNode<CUser*>(); // 유저 리스트 생성
}

// 유저 생성
void CLoginManager::New() {
	char id[CHARSIZE];
	char pass[CHARSIZE];

	while (1) {
		cout << "ID : ";
		cin >> id;

		bool flug = false;
		NowUser = ListUser->GetHead();
		// 유저 리스트의 헤드 노드 위치에 유저 노드를 위치
		while (NowUser != nullptr) {	// 유저 노드가 nullptr 를 만날때까지
			if (!strcmp(NowUser->GetData()->Getid(), id)) {
				// 리스트 내에 노드 중 입력 받은 id 값을 가진 노드가 있을 경우
				flug = true;
				break; // while 중지
			}
			NowUser = NowUser->GetNext(); // 다음 노드로 이동
		}

		if (flug) {	// 같은 id를 찾은 경우
			cout << "중복된 ID 가 있습니다. 다시 입력하세요." << endl;
			continue; // 처음부터 다시 시작
		}
		break; // while 중지
	}

	cout << "PASSWORD : ";
	cin >> pass;

	CUser* ptr = new CUser(id, pass);	// 유저 객체 생성 
	ptr->Init();	// 생성한 유저 객체의 초기화를 위한 함수
	ListUser->Input(ptr);	// 유저 리스트에 추가
}

bool CLoginManager::Login() {
	char id[CHARSIZE];
	char pass[CHARSIZE];

	cout << "ID : ";
	cin >> id;

	NowUser = ListUser->GetHead();
	// 유저 리스트의 헤드 노드 위치에 유저 노드를 위치
	while (NowUser != nullptr) {	// 유저 노드가 nullptr 를 만날때까지
		if (!strcmp(NowUser->GetData()->Getid(), id)) {
			// 리스트 내에 노드 중 입력 받은 id 값을 가진 노드가 있을 경우
			break; // while 문 탈출
			// 탈출함으로써 NowUser 는 리스트 내에 id 값이 일치하는 노드의 주소에 머무름.
		}
		NowUser = NowUser->GetNext(); // 다음 노드로 이동
	}
	if (NowUser == nullptr) {	// 노드가 nullptr 까지 이동한 경우 (id 를 찾지 못한 경우)
		cout << "없는 계정입니다." << endl;
		return false;
	}

	cout << "PASSWORD : ";
	cin >> pass;

	if (!strcmp(NowUser->GetData()->Getpass(), pass)) {
		// NowUser 가 가르키는 노드의 유저 데이터 중 pass 값이 입력 받은 pass 와 일치하는 경우
		NowUser->GetData()->Login(); // 유저 로그인 상태로 체크
		return true;
	}
	else { // pass 가 일치하지 않는 경우
		cout << "PASSWORD 가 일치하지 않습니다." << endl;
		return false;
	}
}

void CLoginManager::Logout() {
	NowUser->GetData()->Logout();	// 현재 접속한 유저를 로그아웃 상태로 체크
	NowUser = nullptr; // 유저 노드 초기화
}

void CLoginManager::Del() {
	ListUser->Delete(NowUser); // 현재 접속한 유저의 노드를 리스트에서 삭제
	NowUser = nullptr;		// 유저 노드 초기화
}