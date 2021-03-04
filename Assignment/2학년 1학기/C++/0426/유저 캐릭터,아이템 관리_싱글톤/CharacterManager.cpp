#include "CharacterManager.h"
#include "LoginManager.h"

const char* CharacterManager::brood[] = { "","����","��ũ","�޸�" };

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
			cout << "�� �̻� ĳ���͸� ������ �� �����ϴ�." << endl;
			return;
		}

		int sel;

		cout << "< ���� ���� >" << endl;
		for (int i = 1; i < sizeof(brood) / sizeof(char*); i++) {
			cout << i << ". " << brood[i] << endl;
		}
		cout << "���� : ";
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
			cout << "ĳ���� �̸� : ";
			cin >> name;

			Character* ptr = nullptr;
			bool flug = false;
			while (NowUser->GetCharacter(&ptr)) {
				if (!strcmp(ptr->GetCharName(), name)) {
					cout << "�̹� �����ϴ� ĳ���͸� �Դϴ�." << endl;
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
		cout << "�����ϰ� ��� ĳ���Ͱ� �����ϴ�." << endl;
		return;
	}

	char name[CHARSIZE];

	cout << "ĳ���� �̸� : ";
	cin >> name;

	while (NowUser->GetCharacter(&ptr)) {
		if (!strcmp(ptr->GetCharName(), name)) {
			cout << ptr->GetCharName() << " ĳ���͸� �����մϴ�." << endl;
			NowUser->DelCharacter(ptr);
			NowUser->SearchCharEnd();
			return;
		}
	}

	cout << "ã�� �̸��� ĳ���Ͱ� �����ϴ�." << endl;
}

void CharacterManager::PrintChar(Character* index) {
	cout << endl;
	cout << "���� : " << brood[index->GetBroodCode()] << endl;
	cout << "ĳ���� �̸� : " << index->GetCharName() << endl;
	cout << "���� : " << index->GetLevel() << endl;
	cout << "HP : " << index->GetHP() << endl;
	cout << "MP : " << index->GetMP() << endl;
	cout << "���ݷ� : " << index->GetAtt() << endl;
	cout << "���� : " << index->GetDef() << endl;
	cout << endl;
}