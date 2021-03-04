#include "Programing.h"

const char* CPrograming::SubName[] = { "C++2","C#","API2" };

CPrograming::CPrograming() {

}

CPrograming::~CPrograming() {

}

void CPrograming::input() {
	char _name[NAMESIZE];
	char _id[IDSIZE];
	int Score;

	if (PGcount == STUDENTSIZE) {
		cout << "최대 인원 초과" << endl;
		return;
	}

	cout << "이름 : ";
	cin >> _name;
	
	sprintf(_id,"201903200%d", PGcount + 1);

	CStudent* ptr = new CStudent(_name, _id);
	
	for (int i = 0; i < (sizeof(SubName) / sizeof(char*)); i++) {
		cout << SubName[i] << " : ";
		cin >> Score;

		if (cin.failbit) {
			cin.clear();
			cin.ignore(256, '\n');
		}

		if (Score > OVER) {
			cout << "점수를 잘못 입력하셨습니다." << endl;
			delete ptr;
			return;
		}
		ptr->SetScore(i, Score);
	}

	PGlist[PGcount++] = ptr;
}

void CPrograming::display() {
	for (int i = 0; i < PGcount; i++) {
		cout << "이름 : " << PGlist[i]->GetName() << endl;
		cout << "학번 : " << PGlist[i]->GetID() << endl;
		for (int j = 0; j < (sizeof(SubName) / sizeof(char*)); j++) {
			cout << SubName[j] << " : " << PGlist[i]->GetScore(j) << endl;
		}
		PGlist[i]->Grade();

		cout << "평점 : " << PGlist[i]->GetGrade() << endl;
	}
}

void CPrograming::serach() {
	char _id[IDSIZE];

	cout << "학번 : ";
	cin >> _id;

	for (int i = 0; i < PGcount; i++) {
		if (!strcmp(PGlist[i]->GetID(), _id)) {
			cout << "이름 : " << PGlist[i]->GetName() << endl;
			for (int j = 0; j < (sizeof(SubName) / sizeof(char*)); j++) {
				cout << SubName[j] << " : " << PGlist[i]->GetScore(j) << endl;
			}
			PGlist[i]->Grade();

			cout << "평점 : " << PGlist[i]->GetGrade() << endl;
		}
	}
}