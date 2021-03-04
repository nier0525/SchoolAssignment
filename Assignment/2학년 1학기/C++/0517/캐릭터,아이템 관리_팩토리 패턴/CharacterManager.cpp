#include "CharacterManager.h"
#include "CharacterFactory.h"
#include "LoginManager.h"

const char* CharacterManager::brood[] = { "����","��ũ","�޸�" };
const char* CharacterManager::job[] = { "����", "�ü�" , "������" };

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
	cout << "���� : " << brood[temp->GetBroodCode() - 1] << endl;
	cout << "ĳ���� �̸� : " << temp->GetNickName() << endl;
	cout << "���� : " << job[temp->GetJobCode() - 1] << endl;
	cout << "���� : " << temp->GetLevel() << endl;
	cout << endl;
}

void CharacterManager::New() {
	User* NowUser = LoginManager::GetInstance()->GetNowUser();

	if (!NowUser->CharacterCountCheak()) {
		cout << "ĳ���� �ִ� ���� Ƚ���� �ʰ��߽��ϴ�." << endl;
		return;
	}

	while (1) {
		int sel;

		cout << "< ���� ���� >" << endl;
		for (int i = 0; i < sizeof(brood) / sizeof(char*); i++) {
			cout << i + 1 << ". " << brood[i] << endl;
		}

		cout << "���� : ";
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

			cout << "< ���� ���� >" << endl;
			for (int j = 0; j < sizeof(job) / sizeof(char*); j++) {
				cout << j + 1 << ". " << job[j] << endl;
			}
			cout << "���� : ";
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
				cout << "ĳ���� �̸� : ";
				cin >> name;

				Character* ptr = nullptr;
				bool flug = false;
				while (NowUser->GetCharacter(&ptr)) {
					if (!strcmp(ptr->GetNickName(), name)) {
						cout << "�̹� �����ϴ� ĳ���� �̸��Դϴ�." << endl;
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
		cout << "�����ϰ� �ִ� ĳ���Ͱ� �����ϴ�." << endl;
		return false;
	}

	cout << "< ĳ���� ���� >" << endl;
	while (NowUser->GetCharacter(&ptr)) {
		this->PrintCharacter(ptr);
	}
	NowUser->SearchEnd();

	char name[CHARSIZE];

	cout << "ĳ���� �̸� : ";
	cin >> name;

	while (NowUser->GetCharacter(&ptr)) {
		if (!strcmp(ptr->GetNickName(), name)) {
			cout << ptr->GetNickName() << " ĳ���ͷ� �����մϴ�." << endl;
			NowCharacter = ptr;
			NowUser->SearchEnd();
			return true;
		}
	}
	cout << "���� ĳ�����Դϴ�." << endl;
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
		cout << "�����ϰ� �ִ� ĳ���Ͱ� �����ϴ�." << endl;
		return;
	}

	char name[CHARSIZE];

	cout << "ĳ���� �̸� : ";
	cin >> name;

	while (NowUser->GetCharacter(&ptr)) {
		if (!strcmp(ptr->GetNickName(), name)) {
			cout << ptr->GetNickName() << " ĳ���͸� �����մϴ�." << endl;
			NowUser->DelCharacter(ptr);
			NowUser->SearchEnd();
			return;
		}
	}
	cout << "ã�� �̸��� ĳ���Ͱ� �����ϴ�." << endl;
}