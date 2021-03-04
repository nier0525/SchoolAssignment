#include "CharacterManager.h"
#include "LoginManager.h"

const char* CharacterManager::brood[] = { "","엘프","오크","휴먼" };

CharacterManager* CharacterManager::pthis = nullptr;

CharacterManager* CharacterManager::GetInstance() {
	if (!pthis) {
		pthis = new CharacterManager();
	}
	return pthis;
}

void CharacterManager::Destory() {
	if (pthis) {
		delete pthis;
		pthis = nullptr;
	}
}

CharacterManager::CharacterManager() {

}

CharacterManager::~CharacterManager() {

}

void CharacterManager::Init() {

}

void CharacterManager::NewChar() {

	User* NowUser = LoginManager::GetInstance()->GetNowUser();

	while (1) {
		if (!NowUser->CharCountCheck()) {
			cout << "더 이상 캐릭터를 생성할 수 없습니다." << endl;
			return;
		}

		int sel;

		cout << "< 종족 선택 >" << endl;
		for (int i = 1; i < sizeof(brood) / sizeof(char*); i++) {
			cout << i << ". " << brood[i] << endl;
		}
		cout << "선택 : ";
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (sel < 1 || sel > sizeof(brood) / sizeof(char*) - 1) {
			cout << "< Error >" << endl;
			continue;
		}

		while (1) {

			char name[CHARSIZE];
			cout << "캐릭터 이름 : ";
			cin >> name;

			Character* ptr = nullptr;
			bool flug = false;
			while (NowUser->GetCharacter(&ptr)) {
				if (!strcmp(ptr->GetCharName(), name)) {
					cout << "이미 존재하는 캐릭터명 입니다." << endl;
					flug = true;
				}
			}

			NowUser->SearchCharEnd();

			if (flug) {
				continue;
			}

			switch (sel)
			{
			case ELF:
				NowUser->NewCharacter(new Elf(name));
				break;

			case OAK:
				NowUser->NewCharacter(new Oak(name));
				break;

			case HUMAN:
				NowUser->NewCharacter(new Human(name));
				break;
			}
			break;
		}
		break;
	}
}

void CharacterManager::SelChar() {
	
}

void CharacterManager::DelChar() {
	User* NowUser = LoginManager::GetInstance()->GetNowUser();
	Character* ptr = nullptr;
	bool flug = false;
	while (NowUser->GetCharacter(&ptr)) {
		this->PrintChar(ptr);
		flug = true;
	}

	NowUser->SearchCharEnd();

	if (!flug) {
		cout << "보유하고 계신 캐릭터가 없습니다." << endl;
		return;
	}

	char name[CHARSIZE];

	cout << "캐릭터 이름 : ";
	cin >> name;

	while (NowUser->GetCharacter(&ptr)) {
		if (!strcmp(ptr->GetCharName(), name)) {
			cout << ptr->GetCharName() << " 캐릭터를 삭제합니다." << endl;
			NowUser->DelCharacter(ptr);
			NowUser->SearchCharEnd();
			return;
		}
	}

	cout << "찾은 이름의 캐릭터가 없습니다." << endl;
}

void CharacterManager::PrintChar(Character* index) {
	cout << endl;
	cout << "종족 : " << brood[index->GetBroodCode()] << endl;
	cout << "캐릭터 이름 : " << index->GetCharName() << endl;
	cout << "레벨 : " << index->GetLevel() << endl;
	cout << "HP : " << index->GetHP() << endl;
	cout << "MP : " << index->GetMP() << endl;
	cout << "공격력 : " << index->GetAtt() << endl;
	cout << "방어력 : " << index->GetDef() << endl;
	cout << endl;
}