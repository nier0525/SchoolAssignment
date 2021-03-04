#include "CharacterManager.h"

// �޴��� ���� char �� �迭
const char* CharacterManager::brood[] = { " ", "����" , "��ũ" , "�޸�" };
const char* CharacterManager::sex[] = { " ", "����" , "����" };

const char* CharacterManager::Elfjob[] = { " ", "���� ������", "�鸶����" , "��ؽ�" };
const char* CharacterManager::Oakjob[] = { " ", "���� ����Ŀ" , "����Ŀ" , "�渶����" };
const char* CharacterManager::Humanjob[] = { " ", "�ǽ�����" , "�ȶ��" , "��ȯ����" };

CharacterManager::CharacterManager() {

}

CharacterManager::~CharacterManager() {

}

int CharacterManager::broodMenu() {	// ���� �޴�
	while (1) {
		int sel;
		cout << "< ���� ���� >" << endl;
		for (int i = 1; i < sizeof(brood) / sizeof(char*); i++) {	// brood[] �迭�� ����ŭ i �� ���
			cout << i << ". " << brood[i] << endl;	// ������� brood[] �迭 ���� char �� ���
		}
		cout << "���� : ";
		cin >> sel;
		if (cin.failbit) {	// �Է� ���� sel ���� ������ �ƴ� �� ���� üũ
			cin.clear();
			cin.ignore(256, '\n');
		}
		if (sel < 1 || sel > sizeof(brood) / sizeof(char*) - 1) { // sel ���� ������ �޴��� ���� ���� ���� ��
			cout << "< Error >" << endl;
			continue; // �ٽ� �޴� ���
		}
		return sel; // sel �� ��ȯ
	}
}

const char* CharacterManager::sexMenu() {	 // ���� �޴�. ���� �޴��� ����
	while (1) {
		int sel;
		cout << "< ���� ���� >" << endl;
		for (int i = 1; i < sizeof(sex) / sizeof(char*); i++) {
			cout << i << ". " << sex[i] << endl;
		}
		cout << "���� : ";
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

const char* CharacterManager::jobMenu(int _brood) {	// ������ ���� �޴�
	switch (_brood) {	// ���� ���� ��, �� ���� �°� ����ġ�� ����
	case ELF:
		while (1) {
			int sel;
			cout << "< ���� ���� >" << endl;
			for (int i = 1; i < sizeof(Elfjob) / sizeof(char*); i++) {
				cout << i << ". " << Elfjob[i] << endl;
			}
			cout << "���� : ";
			cin >> sel;
			if (cin.failbit) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (sel < 1 || sel > sizeof(Elfjob) / sizeof(char*) - 1) {
				cout << "< Error >" << endl;
				continue;
			}
			return Elfjob[sel];	// Elfjob �迭 ���� sel ��° char ���� ����
		}
		break;

		// �Ʒ��� ����
	case OAK:
		while (1) {
			int sel;
			cout << "< ���� ���� >" << endl;
			for (int i = 1; i < sizeof(Oakjob) / sizeof(char*); i++) {
				cout << i << ". " << Oakjob[i] << endl;
			}
			cout << "���� : ";
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
			cout << "< ���� ���� >" << endl;
			for (int i = 1; i < sizeof(Humanjob) / sizeof(char*); i++) {
				cout << i << ". " << Humanjob[i] << endl;
			}
			cout << "���� : ";
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

void CharacterManager::New(Node<CUser*>* NowUser) {	// ĳ���� ���� �Լ�
	if (!NowUser->GetData()->CheakCharCount()) {	// ���� ������ ĳ������ ���� �ѵ� üũ
		cout << "������ �� �ִ� ĳ������ ������ �ʰ��߽��ϴ�." << endl;
		return; // �Լ� ����
	}
	
	char name[CHARSIZE];
	cout << "�г��� : ";
	cin >> name;

	NowUser->GetData()->SetChar(NowUser->GetData()->Gethead());
	// �������� ĳ���� ����Ʈ�� ��� ��� ��ġ�� ������ ĳ���� ��带 ��ġ��Ŵ
	while (NowUser->GetData()->GetChar() != nullptr) {	// ������ ĳ���� ��尡 nullptr �� ����������
		if (!strcmp(NowUser->GetData()->GetChar()->GetData()->GetName(), name)) {
			// ������ ĳ���� ��尡 ��ġ�� ����Ʈ�� ��� ������ ���� �г��� ���� �Է� ���� �г��� ���� ��ġ�� ��
			cout << "�ߺ��� ���̵� �ֽ��ϴ�." << endl; // �ߺ� üũ
			return; // �Լ� ����
		}
		NowUser->GetData()->SetChar(NowUser->GetData()->GetChar()->GetNext()); 
		// ������ ĳ���� ���� ���� ���� �̵�
	}

	Character* ptr; // ĳ���� ��ü ������

	switch (broodMenu()) {	// ���� �޴� ���� �� ��ȯ ���� ������ ����ġ ������ ����
	case ELF:
		ptr = new Elf(name); // ��ü �����͸� Elf ��ü�� ���� �Ҵ�
		ptr->SetBrood(brood[ELF]);	// ���� ����
		ptr->SetSex(sexMenu());		// ���� ����
		ptr->SetJob(jobMenu(ELF));	// ���� ������ ����
		NowUser->GetData()->New(ptr);	// ���� �� �� ��ü�� ������ ĳ���� ����Ʈ�� ����
		break;
		
		// �Ʒ��� ���� ����
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

void CharacterManager::Del(Node<CUser*>* NowUser) {	// ĳ���� ���� �Լ�
	if (NowUser->GetData()->Getcount() == NODATA) { // ������ ������ ĳ���Ͱ� ���� ���
		cout << "���� �����ϰ� ��� ĳ���Ͱ� �����ϴ�." << endl;
		return;
	}
	
	bool flug = false;
	int index = broodMenu();	// ���� ���� ���� index �� ����

	NowUser->GetData()->SetChar(NowUser->GetData()->Gethead());
	// �������� ĳ���� ����Ʈ�� ��� ��� ��ġ�� ������ ĳ���� ��带 ��ġ��Ŵ
	while (NowUser->GetData()->GetChar() != nullptr) {   
		// ������ ĳ���� ��尡 nullptr �� ����������
		if (!strcmp(NowUser->GetData()->GetChar()->GetData()->GetBrood(), brood[index])) {
			// ������ ĳ���� ��尡 ��ġ�� ����Ʈ�� ��� ������ ���� ���� ���� ������ ���� ���� ��ġ�� ��
			cout << endl << "�г��� : " << NowUser->GetData()->GetChar()->GetData()->GetName() << " / ";
			cout << "���� : " << NowUser->GetData()->GetChar()->GetData()->GetSex() << " / ";
			cout << "���� : " << NowUser->GetData()->GetChar()->GetData()->GetJob() << endl;
			flug = true;
			// �ش� ���� ��ü ���� �����͸� ������ְ� flug �� ��
		}
		NowUser->GetData()->SetChar(NowUser->GetData()->GetChar()->GetNext());
		// ������ ĳ���� ���� ���� ���� �̵�
	}
	if (!flug) { // ���� flug �� ������ �� (������ ����Ʈ ���� ������ ������ �����Ͱ� ���� ��)
		cout << "�ش� ���� �� �����ϰ� �ִ� ĳ���Ͱ� �����ϴ�." << endl;
		return;
	}

	char name[CHARSIZE];
	cout << endl << "������ ĳ������ �г����� �Է��ϼ���." << endl;
	cout << "�г��� : ";
	cin >> name;

	NowUser->GetData()->SetChar(NowUser->GetData()->Gethead());
	// �������� ĳ���� ����Ʈ�� ��� ��� ��ġ�� ������ ĳ���� ��带 �ٽ� ��ġ��Ŵ
	while (NowUser->GetData()->GetChar() != nullptr) {
		// ������ ĳ���� ��尡 nullptr �� ���� ������
		if (!strcmp(NowUser->GetData()->GetChar()->GetData()->GetName(), name)) {
			// ������ ĳ���� ��尡 ��ġ�� ����Ʈ�� ��� ������ ���� �г��� ���� �Է��� �г��� ���� ��ġ�� ��
			cout << NowUser->GetData()->GetChar()->GetData()->GetName() << " ĳ���͸� �����߽��ϴ�." << endl;
			NowUser->GetData()->Del(NowUser->GetData()->GetChar());
			// �ش� ��带 ����
			return;
		}
		NowUser->GetData()->SetChar(NowUser->GetData()->GetChar()->GetNext());
		// ������ ĳ���� ���� ���� ���� �̵�
	}
	// ���� ĳ���� ��尡 ������ ���� while ���� ������� �ش� �г����� ���� ĳ���͸� ã�� ���� ��
	cout << "�ش� �г����� ĳ���͸� ã�� ���߽��ϴ�." << endl;
}