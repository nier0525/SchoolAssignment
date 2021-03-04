#include "ProgramingManager.h"

const char* CPrograming::SubName[] = { "C++", "C#", "API" };

CPrograming::CPrograming() {

}

CPrograming::~CPrograming() {

}

void CPrograming::Input() {
	char name[NAMESIZE];
	char id[IDSIZE];
	int _score;

	cout << "�̸� : ";
	cin >> name;
	sprintf(id, "19%s%03d", ID_PROGRAMING, StudentList->GetNodeCount() + 1);

	CStudent* ptr = new CStudent(name, id);

	for (int i = 0; i < sizeof(SubName) / sizeof(char*); i++) {
		cout << SubName[i] << " : ";
		cin >> _score;
		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}
		ptr->SetScore(i, _score);
	}
	ptr->GradeCheck();
	StudentList->Input(ptr);
}

void CPrograming::Display() {
	NowStudent = StudentList->GetHead();
	while (NowStudent != nullptr) {
		cout << "�̸� : " << NowStudent->GetData()->GetName() << endl;
		cout << "�й� : " << NowStudent->GetData()->GetID() << endl;
		for (int i = 0; i < sizeof(SubName) / sizeof(char*); i++) {
			cout << SubName[i] << " : " << NowStudent->GetData()->GetScore(i) << endl;
		}
		cout << "���� : " << NowStudent->GetData()->GetGrade() << endl;

		NowStudent = NowStudent->GetLink();
	}
}

void CPrograming::Search() {
	char id[IDSIZE];
	cout << "�й� : ";
	cin >> id;

	NowStudent = StudentList->GetHead();
	while (NowStudent != nullptr) {
		if (!strcmp(NowStudent->GetData()->GetID(), id)) {
			cout << "�̸� : " << NowStudent->GetData()->GetName() << endl;
			for (int i = 0; i < sizeof(SubName) / sizeof(char*); i++) {
				cout << SubName[i] << " : " << NowStudent->GetData()->GetScore(i) << endl;
			}
			cout << "���� : " << NowStudent->GetData()->GetGrade() << endl;
			break;
		}
		NowStudent = NowStudent->GetLink();
	}
}