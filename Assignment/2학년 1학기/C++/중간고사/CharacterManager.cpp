#include "CharacterManager.h"

// 메뉴에 넣을 char 값 배열
const char* CharacterManager::brood[] = { " ", "엘프" , "오크" , "휴먼" };
const char* CharacterManager::sex[] = { " ", "남자" , "여자" };

const char* CharacterManager::Elfjob[] = { " ", "보우 마스터", "백마도사" , "어쌔신" };
const char* CharacterManager::Oakjob[] = { " ", "오버 테이커" , "버서커" , "흑마도사" };
const char* CharacterManager::Humanjob[] = { " ", "건슬링거" , "팔라딘" , "소환술사" };

CharacterManager::CharacterManager() {

}

CharacterManager::~CharacterManager() {

}

int CharacterManager::broodMenu() {	// 종족 메뉴
	while (1) {
		int sel;
		cout << "< 종족 선택 >" << endl;
		for (int i = 1; i < sizeof(brood) / sizeof(char*); i++) {	// brood[] 배열의 값만큼 i 값 상승
			cout << i << ". " << brood[i] << endl;	// 순서대로 brood[] 배열 내의 char 값 출력
		}
		cout << "선택 : ";
		cin >> sel;
		if (cin.failbit) {	// 입력 받은 sel 값이 정수가 아닐 때 오류 체크
			cin.clear();
			cin.ignore(256, '\n');
		}
		if (sel < 1 || sel > sizeof(brood) / sizeof(char*) - 1) { // sel 값이 지정된 메뉴의 값의 범위 밖일 때
			cout << "< Error >" << endl;
			continue; // 다시 메뉴 출력
		}
		return sel; // sel 값 반환
	}
}

const char* CharacterManager::sexMenu() {	 // 성별 메뉴. 종족 메뉴와 동일
	while (1) {
		int sel;
		cout << "< 성별 선택 >" << endl;
		for (int i = 1; i < sizeof(sex) / sizeof(char*); i++) {
			cout << i << ". " << sex[i] << endl;
		}
		cout << "선택 : ";
		cin >> sel;
		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}
		if (sel < 1 || sel > 2) {
			cout << "< Error >" << endl;
			continue;
		}
		return sex[sel];
	}
}

const char* CharacterManager::jobMenu(int _brood) {	// 종족별 직업 메뉴
	switch (_brood) {	// 종족 선택 후, 그 값에 맞게 스위치문 실행
	case ELF:
		while (1) {
			int sel;
			cout << "< 직업 선택 >" << endl;
			for (int i = 1; i < sizeof(Elfjob) / sizeof(char*); i++) {
				cout << i << ". " << Elfjob[i] << endl;
			}
			cout << "선택 : ";
			cin >> sel;
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (sel < 1 || sel > sizeof(Elfjob) / sizeof(char*) - 1) {
				cout << "< Error >" << endl;
				continue;
			}
			return Elfjob[sel];	// Elfjob 배열 내에 sel 번째 char 값을 리턴
		}
		break;

		// 아래는 동일
	case OAK:
		while (1) {
			int sel;
			cout << "< 직업 선택 >" << endl;
			for (int i = 1; i < sizeof(Oakjob) / sizeof(char*); i++) {
				cout << i << ". " << Oakjob[i] << endl;
			}
			cout << "선택 : ";
			cin >> sel;
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (sel < 1 || sel > sizeof(Oakjob) / sizeof(char*) - 1) {
				cout << "< Error >" << endl;
				continue;
			}
			return Oakjob[sel];
		}
		break;

	case HUMAN:
		while (1) {
			int sel;
			cout << "< 직업 선택 >" << endl;
			for (int i = 1; i < sizeof(Humanjob) / sizeof(char*); i++) {
				cout << i << ". " << Humanjob[i] << endl;
			}
			cout << "선택 : ";
			cin >> sel;
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (sel < 1 || sel > sizeof(Humanjob) / sizeof(char*) - 1) {
				cout << "< Error >" << endl;
				continue;
			}
			return Humanjob[sel];
		}
		break;
	}
}

void CharacterManager::New(Node<CUser*>* NowUser) {	// 캐릭터 생성 함수
	if (!NowUser->GetData()->CheakCharCount()) {	// 현재 유저의 캐릭터의 개수 한도 체크
		cout << "생성할 수 있는 캐릭터의 개수가 초과했습니다." << endl;
		return; // 함수 종료
	}
	
	char name[CHARSIZE];
	cout << "닉네임 : ";
	cin >> name;

	NowUser->GetData()->SetChar(NowUser->GetData()->Gethead());
	// 유저내의 캐릭터 리스트의 헤드 노드 위치에 유저의 캐릭터 노드를 위치시킴
	while (NowUser->GetData()->GetChar() != nullptr) {	// 유저의 캐릭터 노드가 nullptr 를 만날때까지
		if (!strcmp(NowUser->GetData()->GetChar()->GetData()->GetName(), name)) {
			// 유저의 캐릭터 노드가 위치한 리스트의 노드 데이터 안의 닉네임 값이 입력 받은 닉네임 값과 일치할 시
			cout << "중복된 아이디가 있습니다." << endl; // 중복 체크
			return; // 함수 종료
		}
		NowUser->GetData()->SetChar(NowUser->GetData()->GetChar()->GetNext()); 
		// 유저의 캐릭터 노드는 다음 노드로 이동
	}

	Character* ptr; // 캐릭터 객체 포인터

	switch (broodMenu()) {	// 종족 메뉴 실행 후 반환 받은 정수를 스위치 값으로 실행
	case ELF:
		ptr = new Elf(name); // 객체 포인터를 Elf 객체로 동적 할당
		ptr->SetBrood(brood[ELF]);	// 종족 설정
		ptr->SetSex(sexMenu());		// 성별 설정
		ptr->SetJob(jobMenu(ELF));	// 엘프 직업군 설정
		NowUser->GetData()->New(ptr);	// 설정 다 된 객체를 유저의 캐릭터 리스트로 삽입
		break;
		
		// 아래는 위와 동일
	case OAK:
		ptr = new Oak(name);
		ptr->SetBrood(brood[OAK]);
		ptr->SetSex(sexMenu());
		ptr->SetJob(jobMenu(OAK));
		NowUser->GetData()->New(ptr);
		break;

	case HUMAN:
		ptr = new Human(name);
		ptr->SetBrood(brood[HUMAN]);
		ptr->SetSex(sexMenu());
		ptr->SetJob(jobMenu(HUMAN));
		NowUser->GetData()->New(ptr);
		break;
	}
}

void CharacterManager::Del(Node<CUser*>* NowUser) {	// 캐릭터 삭제 함수
	if (NowUser->GetData()->Getcount() == NODATA) { // 유저가 보유한 캐릭터가 없는 경우
		cout << "현재 보유하고 계신 캐릭터가 없습니다." << endl;
		return;
	}
	
	bool flug = false;
	int index = broodMenu();	// 종족 선택 값을 index 에 저장

	NowUser->GetData()->SetChar(NowUser->GetData()->Gethead());
	// 유저내의 캐릭터 리스트의 헤드 노드 위치에 유저의 캐릭터 노드를 위치시킴
	while (NowUser->GetData()->GetChar() != nullptr) {   
		// 유저의 캐릭터 노드가 nullptr 를 만날때까지
		if (!strcmp(NowUser->GetData()->GetChar()->GetData()->GetBrood(), brood[index])) {
			// 유저의 캐릭터 노드가 위치한 리스트의 노드 데이터 안의 종족 값이 선택한 종족 값과 일치할 시
			cout << endl << "닉네임 : " << NowUser->GetData()->GetChar()->GetData()->GetName() << " / ";
			cout << "성별 : " << NowUser->GetData()->GetChar()->GetData()->GetSex() << " / ";
			cout << "직업 : " << NowUser->GetData()->GetChar()->GetData()->GetJob() << endl;
			flug = true;
			// 해당 종족 객체 내의 데이터를 출력해주고 flug 는 참
		}
		NowUser->GetData()->SetChar(NowUser->GetData()->GetChar()->GetNext());
		// 유저의 캐릭터 노드는 다음 노드로 이동
	}
	if (!flug) { // 만일 flug 가 거짓일 시 (유저의 리스트 내에 선택한 종족의 데이터가 없을 시)
		cout << "해당 종족 중 보유하고 있는 캐릭터가 없습니다." << endl;
		return;
	}

	char name[CHARSIZE];
	cout << endl << "삭제할 캐릭터의 닉네임을 입력하세요." << endl;
	cout << "닉네임 : ";
	cin >> name;

	NowUser->GetData()->SetChar(NowUser->GetData()->Gethead());
	// 유저내의 캐릭터 리스트의 헤드 노드 위치에 유저의 캐릭터 노드를 다시 위치시킴
	while (NowUser->GetData()->GetChar() != nullptr) {
		// 유저의 캐릭터 노드가 nullptr 를 만날 때까지
		if (!strcmp(NowUser->GetData()->GetChar()->GetData()->GetName(), name)) {
			// 유저의 캐릭터 노드가 위치한 리스트의 노드 데이터 안의 닉네임 값이 입력한 닉네임 값과 일치할 시
			cout << NowUser->GetData()->GetChar()->GetData()->GetName() << " 캐릭터를 삭제했습니다." << endl;
			NowUser->GetData()->Del(NowUser->GetData()->GetChar());
			// 해당 노드를 삭제
			return;
		}
		NowUser->GetData()->SetChar(NowUser->GetData()->GetChar()->GetNext());
		// 유저의 캐릭터 노드는 다음 노드로 이동
	}
	// 만약 캐릭터 노드가 끝까지 돌고 while 문을 벗어났으면 해당 닉네임을 가진 캐릭터를 찾지 못한 것
	cout << "해당 닉네임의 캐릭터를 찾지 못했습니다." << endl;
}