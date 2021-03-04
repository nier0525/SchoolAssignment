#include "CharacterManager.h"
#include "CharacterFactory.h"
#include "LoginManager.h"

const char* CharacterManager::brood[] = { "엘프","오크","휴먼" };
const char* CharacterManager::job[] = { "전사", "궁수" , "마법사" };

CharacterManager* CharacterManager::pthis = nullptr;

CharacterManager* CharacterManager::GetInstance() {
	if (!pthis) {
		pthis = new CharacterManager();
	}
	CharacterFactory::GetInstance();
	return pthis;
}

void CharacterManager::Destory() {
	if (pthis) {
		delete pthis;
		pthis = nullptr;
	}
	CharacterFactory::Destory();
}

CharacterManager::CharacterManager() {
	NowCharacter = nullptr;
}

CharacterManager::~CharacterManager() {

}

Character* CharacterManager::GetNowCharacter() {
	return NowCharacter;
}

void CharacterManager::PrintCharacter(Character* temp) {
	cout << endl;
	cout << "종족 : " << brood[temp->GetBroodCode() - 1] << endl;
	cout << "캐릭터 이름 : " << temp->GetNickName() << endl;
	cout << "직업 : " << job[temp->GetJobCode() - 1] << endl;
	cout << "레벨 : " << temp->GetLevel() << endl;
	cout << endl;
}

void CharacterManager::New() {
	User* NowUser = LoginManager::GetInstance()->GetNowUser();

	if (!NowUser->CharacterCountCheak()) {
		cout << "캐릭터 최대 생성 횟수를 초과했습니다." << endl;
		return;
	}

	while (1) {
		int sel;

		cout << "< 종족 선택 >" << endl;
		for (int i = 0; i < sizeof(brood) / sizeof(char*); i++) {
			cout << i + 1 << ". " << brood[i] << endl;
		}

		cout << "선택 : ";
		cin >> sel;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}
		if (sel < 1 || sel > sizeof(brood) / sizeof(char*)) {
			cout << "< Error >" << endl;
			continue;
		}

		while (1)
		{
			int num;

			cout << "< 직업 선택 >" << endl;
			for (int j = 0; j < sizeof(job) / sizeof(char*); j++) {
				cout << j + 1 << ". " << job[j] << endl;
			}
			cout << "선택 : ";
			cin >> num;

			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (num < 1 || num > sizeof(job) / sizeof(char*)) {
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
					if (!strcmp(ptr->GetNickName(), name)) {
						cout << "이미 존재하는 캐릭터 이름입니다." << endl;
						flug = true;
					}
				}
				NowUser->SearchEnd();

				if (flug) {
					continue;
				}

				NowUser->NewCharacter(CharacterFactory::GetInstance()->NewCharacter(name, sel, num));
				break;
			}
			break;
		}
		break;
	}
}

bool CharacterManager::SelectCharacter() {
	User* NowUser = LoginManager::GetInstance()->GetNowUser();
	Character* ptr = nullptr;

	if (NowUser->GetCount() == NODATA) {
		cout << "보유하고 있는 캐릭터가 없습니다." << endl;
		return false;
	}

	cout << "< 캐릭터 선택 >" << endl;
	while (NowUser->GetCharacter(&ptr)) {
		this->PrintCharacter(ptr);
	}
	NowUser->SearchEnd();

	char name[CHARSIZE];

	cout << "캐릭터 이름 : ";
	cin >> name;

	while (NowUser->GetCharacter(&ptr)) {
		if (!strcmp(ptr->GetNickName(), name)) {
			cout << ptr->GetNickName() << " 캐릭터로 접속합니다." << endl;
			NowCharacter = ptr;
			NowUser->SearchEnd();
			return true;
		}
	}
	cout << "없는 캐릭터입니다." << endl;
	return false;
}

void CharacterManager::Delete() {
	User* NowUser = LoginManager::GetInstance()->GetNowUser();
	Character* ptr = nullptr;
	bool flug = false;
	while (NowUser->GetCharacter(&ptr)) {
		this->PrintCharacter(ptr);
		flug = true;
	}
	NowUser->SearchEnd();

	if (!flug) {
		cout << "보유하고 있는 캐릭터가 없습니다." << endl;
		return;
	}

	char name[CHARSIZE];

	cout << "캐릭터 이름 : ";
	cin >> name;

	while (NowUser->GetCharacter(&ptr)) {
		if (!strcmp(ptr->GetNickName(), name)) {
			cout << ptr->GetNickName() << " 캐릭터를 삭제합니다." << endl;
			NowUser->DelCharacter(ptr);
			NowUser->SearchEnd();
			return;
		}
	}
	cout << "찾는 이름의 캐릭터가 없습니다." << endl;
}