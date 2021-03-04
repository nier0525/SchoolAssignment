#include "GraphicManager.h"

const char* CGraphic::SubName[] = { "2D","3D","드로잉" };

CGraphic::CGraphic() {

}

CGraphic::~CGraphic() {

}

void CGraphic::Input() {
	char name[NAMESIZE];
	char id[IDSIZE];
	int _score;

	cout << "이름 : ";
	cin >> name;
	sprintf(id, "19%s%03d", ID_GRAPHIC, StudentList->GetNodeCount() + 1);

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

void CGraphic::Display() {
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

void CGraphic::Search() {
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