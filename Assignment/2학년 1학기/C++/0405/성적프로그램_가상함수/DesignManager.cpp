#include "DesignManager.h"

const char* CDesign::SubName[] = { "게임기획기초", "아이디어발상", "레벨디자인" };

CDesign::CDesign() {

}

CDesign::~CDesign() {

}

void CDesign::Input() {
	char name[NAMESIZE];
	char id[IDSIZE];
	int _score;

	cout << "이름 : ";
	cin >> name;
	sprintf(id, "19%s%03d", ID_DESIGN, StudentList->GetNodeCount() + 1);

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

void CDesign::Display() {
	NowStudent = StudentList->GetHead();
	while (NowStudent != nullptr) {
		cout << "이름 : " << NowStudent->GetData()->GetName() << endl;
		cout << "학번 : " << NowStudent->GetData()->GetID() << endl;
		for (int i = 0; i < sizeof(SubName) / sizeof(char*); i++) {
			cout << SubName[i] << " : " << NowStudent->GetData()->GetScore(i) << endl;
		}
		cout << "평점 : " << NowStudent->GetData()->GetGrade() << endl;

		NowStudent = NowStudent->GetLink();
	}
}

void CDesign::Search() {
	char id[IDSIZE];
	cout << "학번 : ";
	cin >> id;

	NowStudent = StudentList->GetHead();
	while (NowStudent != nullptr) {
		if (!strcmp(NowStudent->GetData()->GetID(), id)) {
			cout << "이름 : " << NowStudent->GetData()->GetName() << endl;
			for (int i = 0; i < sizeof(SubName) / sizeof(char*); i++) {
				cout << SubName[i] << " : " << NowStudent->GetData()->GetScore(i) << endl;
			}
			cout << "평점 : " << NowStudent->GetData()->GetGrade() << endl;
			break;
		}
		NowStudent = NowStudent->GetLink();
	}
}