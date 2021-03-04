#include "Producter.h"

const char* CProducter::SubName[] = { "���ӱ�ȹ����","���ӽó�����","����������" };

CProducter::CProducter() {

}

CProducter::~CProducter() {

}

void CProducter::input() {
	char _name[NAMESIZE];
	char _id[IDSIZE];
	int Score;

	if (PDcount == STUDENTSIZE) {
		cout << "�ִ� �ο� �ʰ�" << endl;
		return;
	}

	cout << "�̸� : ";
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
			cout << "������ �߸� �Է��ϼ̽��ϴ�." << endl;
			delete ptr;
			return;
		}
		ptr->SetScore(i, Score);
	}

	PDlist[PDcount++] = ptr;
}

void CProducter::display() {
	for (int i = 0; i < PDcount; i++) {
		cout << "�̸� : " << PDlist[i]->GetName() << endl;
		cout << "�й� : " << PDlist[i]->GetID() << endl;
		for (int j = 0; j < (sizeof(SubName) / sizeof(char*)); j++) {
			cout << SubName[j] << " : " << PDlist[i]->GetScore(j) << endl;
		}
		PDlist[i]->Grade();

		cout << "���� : " << PDlist[i]->GetGrade() << endl;
	}
}

void CProducter::serach() {
	char _id[IDSIZE];

	cout << "�й� : ";
	cin >> _id;

	for (int i = 0; i < PDcount; i++) {
		if (!strcmp(PDlist[i]->GetID(), _id)) {
			cout << "�̸� : " << PDlist[i]->GetName() << endl;
			for (int j = 0; j < (sizeof(SubName) / sizeof(char*)); j++) {
				cout << SubName[j] << " : " << PDlist[i]->GetScore(j) << endl;
			}
			PDlist[i]->Grade();

			cout << "���� : " << PDlist[i]->GetGrade() << endl;
		}
	}
}