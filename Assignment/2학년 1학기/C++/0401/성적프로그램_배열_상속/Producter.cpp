#include "Producter.h"

const char* CProducter::SubName[] = { "게임기획기초","게임시나리오","레벨디자인" };

CProducter::CProducter() {

}

CProducter::~CProducter() {

}

void CProducter::input() {
	char _name[NAMESIZE];
	char _id[IDSIZE];
	int Score;

	if (PDcount == STUDENTSIZE) {
		cout << "최대 인원 초과" << endl;
		return;
	}

	cout << "이름 : ";
	cin >> _name;

	sprintf(_id, "201903300%d", PDcount + 1);

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

	PDlist[PDcount++] = ptr;
}

void CProducter::display() {
	for (int i = 0; i < PDcount; i++) {
		cout << "이름 : " << PDlist[i]->GetName() << endl;
		cout << "학번 : " << PDlist[i]->GetID() << endl;
		for (int j = 0; j < (sizeof(SubName) / sizeof(char*)); j++) {
			cout << SubName[j] << " : " << PDlist[i]->GetScore(j) << endl;
		}
		PDlist[i]->Grade();

		cout << "평점 : " << PDlist[i]->GetGrade() << endl;
	}
}

void CProducter::serach() {
	char _id[IDSIZE];

	cout << "학번 : ";
	cin >> _id;

	for (int i = 0; i < PDcount; i++) {
		if (!strcmp(PDlist[i]->GetID(), _id)) {
			cout << "이름 : " << PDlist[i]->GetName() << endl;
			for (int j = 0; j < (sizeof(SubName) / sizeof(char*)); j++) {
				cout << SubName[j] << " : " << PDlist[i]->GetScore(j) << endl;
			}
			PDlist[i]->Grade();

			cout << "평점 : " << PDlist[i]->GetGrade() << endl;
		}
	}
}