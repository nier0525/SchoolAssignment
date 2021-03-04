#include "Graphic.h"

const char* CGraphic::SubName[] = { "2D","3D","드로잉" };

CGraphic::CGraphic() {

}

CGraphic::~CGraphic() {

}

void CGraphic::input() {
	char _name[NAMESIZE];
	char _id[IDSIZE];
	int Score;

	if (GPcount == STUDENTSIZE) {
		cout << "최대 인원 초과" << endl;
		return;
	}

	cout << "이름 : ";
	cin >> _name;

	sprintf(_id, "201903300%d", GPcount + 1);

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

	GPlist[GPcount++] = ptr;
}

void CGraphic::display() {
	for (int i = 0; i < GPcount; i++) {
		cout << "이름 : " << GPlist[i]->GetName() << endl;
		cout << "학번 : " << GPlist[i]->GetID() << endl;
		for (int j = 0; j < (sizeof(SubName) / sizeof(char*)); j++) {
			cout << SubName[j] << " : " << GPlist[i]->GetScore(j) << endl;
		}
		GPlist[i]->Grade();

		cout << "평점 : " << GPlist[i]->GetGrade() << endl;
	}
}

void CGraphic::serach() {
	char _id[IDSIZE];

	cout << "학번 : ";
	cin >> _id;

	for (int i = 0; i < GPcount; i++) {
		if (!strcmp(GPlist[i]->GetID(), _id)) {
			cout << "이름 : " << GPlist[i]->GetName() << endl;
			for (int j = 0; j < (sizeof(SubName) / sizeof(char*)); j++) {
				cout << SubName[j] << " : " << GPlist[i]->GetScore(j) << endl;
			}
			GPlist[i]->Grade();

			cout << "평점 : " << GPlist[i]->GetGrade() << endl;
		}
	}
}