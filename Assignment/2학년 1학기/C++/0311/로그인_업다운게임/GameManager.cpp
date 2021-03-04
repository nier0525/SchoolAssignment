#include "GameManager.h"

GManager::GManager() {
	
}

GManager::~GManager() {
	
}

void GManager::init() {
	score = 1;
	srand((unsigned)time(nullptr));
	answer = (rand() % 100) + 1;
}

bool GManager::SetAnswer(const int _ans) {
	if (answer > _ans) {
		cout << "	<Up>" << endl;
	}
	else {
		cout << "	<Down>" << endl;
	}

	if (answer == _ans) { 
		return true; 
	}
	else { 
		score++;
		return false;
	}
}

const int GManager::GetScore() {
	return score;
}

bool GManager::Run() {
	init();
	cout << "<Up and Down ������ �����մϴ�>" << endl << " ������ 1 ���� 100 ���̿� �ֽ��ϴ�. " << endl;
	while (1) {

		int sel;

		cout << score << "��° �õ��Դϴ�. " << endl;
		cout << "�Է� : ";
		cin >> sel;

		if (SetAnswer(sel)) {
			return true;
		}
	}
}