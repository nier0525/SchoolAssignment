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
	cout << "<Up and Down 게임을 시작합니다>" << endl << " 정답은 1 부터 100 사이에 있습니다. " << endl;
	while (1) {

		int sel;

		cout << score << "번째 시도입니다. " << endl;
		cout << "입력 : ";
		cin >> sel;

		if (SetAnswer(sel)) {
			return true;
		}
	}
}