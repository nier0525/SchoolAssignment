#include "DesignManager.h"

const char* CDesign::Sublist[] = { "게임기획기초", "게임시나리오" , "레벨디자인" };

CDesign::CDesign() {}

CDesign::~CDesign() {
	cout << "~CDesign" << endl;
}

void CDesign::input() {
	char _name[NAMESIZE];
	char _id[IDSIZE];
	int _score;

	cout << "이름 : ";
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
			cout << "점수를 잘못 입력했습니다." << endl;
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
		cout << "이름 : " << NowStudent->GetData()->GetName() << endl;
		cout << "학번 : " << NowStudent->GetData()->GetId() << endl;
		for (int i = 0; i < sizeof(Sublist) / sizeof(char*); i++) {
			cout << Sublist[i] << " : " << NowStudent->GetData()->GetScore(i) << endl;
		}
		cout << "평점 : " << NowStudent->GetData()->GetGrade() << endl;

		NowStudent = NowStudent->Getlink();
	}
}

void CDesign::search() {
	char _id[IDSIZE];
	cout << "학번 : ";
	cin >> _id;

	NowStudent = list->GetHead();
	while (NowStudent != nullptr) {
		if (!strcmp(NowStudent->GetData()->GetId(), _id)) {
			cout << "이름 : " << NowStudent->GetData()->GetName() << endl;
			for (int i = 0; i < sizeof(Sublist) / sizeof(char*); i++) {
				cout << Sublist[i] << " : " << NowStudent->GetData()->GetScore(i) << endl;
			}
			cout << "평점 : " << NowStudent->GetData()->GetGrade() << endl;
			break;
		}
		else {
			NowStudent = NowStudent->Getlink();
		}
	}
}