#include "DesignManager.h"

const char* CDesign::Sublist[] = { "���ӱ�ȹ����", "���ӽó�����" , "����������" };

CDesign::CDesign() {}

CDesign::~CDesign() {
	cout << "~CDesign" << endl;
}

void CDesign::input() {
	char _name[NAMESIZE];
	char _id[IDSIZE];
	int _score;

	cout << "�̸� : ";
	cin >> _name;

	sprintf(_id, "201903100%d", list->GetCount() + 1);

	CStudent* ptr = new CStudent(_name, _id);

	for (int i = 0; i < sizeof(Sublist) / sizeof(char*); i++) {
		cout << Sublist[i] << " : ";
		cin >> _score;
		
		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (_score > OVER) {
			cout << "������ �߸� �Է��߽��ϴ�." << endl;
			delete ptr;
			return;
		}
		ptr->SetScore(i, _score);
	}
	ptr->GradeCheck();
	list->insert(ptr);
}

void CDesign::display() {
	NowStudent = list->GetHead();
	while (NowStudent != nullptr) {
		cout << "�̸� : " << NowStudent->GetData()->GetName() << endl;
		cout << "�й� : " << NowStudent->GetData()->GetId() << endl;
		for (int i = 0; i < sizeof(Sublist) / sizeof(char*); i++) {
			cout << Sublist[i] << " : " << NowStudent->GetData()->GetScore(i) << endl;
		}
		cout << "���� : " << NowStudent->GetData()->GetGrade() << endl;

		NowStudent = NowStudent->Getlink();
	}
}

void CDesign::search() {
	char _id[IDSIZE];
	cout << "�й� : ";
	cin >> _id;

	NowStudent = list->GetHead();
	while (NowStudent != nullptr) {
		if (!strcmp(NowStudent->GetData()->GetId(), _id)) {
			cout << "�̸� : " << NowStudent->GetData()->GetName() << endl;
			for (int i = 0; i < sizeof(Sublist) / sizeof(char*); i++) {
				cout << Sublist[i] << " : " << NowStudent->GetData()->GetScore(i) << endl;
			}
			cout << "���� : " << NowStudent->GetData()->GetGrade() << endl;
			break;
		}
		else {
			NowStudent = NowStudent->Getlink();
		}
	}
}