#include "GameManager.h"

// ������ ��� ���� �ʱ�ȭ
GameManager::GameManager() {
	LM = nullptr;
	CM = nullptr;
	IM = nullptr;
}

// �Ҹ��� �޸� ��ȯ
GameManager::~GameManager() {
	delete LM, CM, IM;
}

// �ʱ�ȭ �۾��� ���� �Լ�
void GameManager::Init() {
	LM = new CLoginManager(); // �α��� �Ŵ��� ���� �Ҵ�
	LM->Init(); // �α��� �Ŵ��� �ʱ�ȭ �Լ�
	CM = new CharacterManager(); // ĳ���� �Ŵ��� ���� �Ҵ�
	IM = new ItemManager(); // ������ �Ŵ��� ���� �Ҵ�
	IM->Init(); // ������ �Ŵ��� �ʱ�ȭ �Լ�
}

// �α��� �޴�
int GameManager::LoginMenu() {
	while (1) {
		int sel;
		cout << "< �޴� >" << endl << "1. ȸ������" << endl << "2. �α���" << endl << "3. ����" << endl;
		cout << "���� : ";
		cin >> sel;
		if (cin.failbit) {	// cin ���� �Է��� sel ���� int ���� �ƴ϶�� ���� ó��
			cin.clear();
			cin.ignore(256, '\n');
		}
		if (sel < 1 || sel > 3) { // �Է� ���� sel ���� ������ �޴� ���� ������ ��� ���
			cout << "< Error >" << endl;
			continue; // �ٽ� ����
		}
		return sel; // sel �� ����
	}
}

// ���� �޴�
// �α��� �޴��� ����
int GameManager::GameMenu() {
	while (1) {
		int sel;
		cout << "< �޴� >" << endl 
			<< "1. ĳ���� ����" << endl << "2. ĳ���� ����" << endl
			<< "3. ������ ����" << endl << "4. ������ �Ǹ�" << endl
			<< 	"5. �α׾ƿ�" << endl << "6. ȸ�� Ż��" << endl;
		cout << "���� : ";
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
	Init();	 // ��ü ������ ���� �ʱ�ȭ
	while (1) {
		switch (LoginMenu()) {
		case NEWLOG:
			LM->New(); // �α��� �Ŵ������� New �Լ� ����
			break;

		case LOGIN:
			if (LM->Login()) { // �α��� �Ŵ������� Login �Լ��� true �� ���
				while (1) {
					bool flug = false; // while ���� ����
					switch (GameMenu()) {
						// �Ű� ������ �Ѱ��ִ� ���� ���� ���� ���� ������ ��ü ����
					case NEW:
						CM->New(LM->GetNowUser()); // ĳ���� �Ŵ��� New �Լ� ���� 
						break;

					case DEL:
						CM->Del(LM->GetNowUser()); // ĳ���� �Ŵ��� Del �Լ� ����
						break;

					case BUY:
						IM->Buy(LM->GetNowUser()); // ������ �Ŵ��� Buy �Լ� ����
						break;

					case SELL:
						IM->Sell(LM->GetNowUser()); // ������ �Ŵ��� Sell �Լ� ����
						break;

					case LOGOUT:
						LM->Logout(); // �α��� �Ŵ��� Logout �Լ� ����
						flug = true;  // while ����
						break;

					case DELLOG:
						LM->Del();	// �α��� �Ŵ��� Del �Լ� ����
						flug = true;	// while ����
						break;
					}
					if (flug) { // flug �� true ��
						break; // whlie ����
					}
				}
			}
			break;

		case EXIT:
			return; // �Լ� ���� ( ���α׷� ���� )
			break;
		}
	}
}