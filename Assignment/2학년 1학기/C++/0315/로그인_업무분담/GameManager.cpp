#include "GameManager.h"

GManager::GManager() {
	score = 0;
	mUser = nullptr;
	answer = NODATA;
}

GManager::~GManager() {
}

void GManager::Gameinit(CUser* _mUser) {
	srand((unsigned)time(nullptr));
	answer = (rand() % 100) + 1;
	mUser = _mUser;
}

void GManager::GameEnd() {
	score = 0;
	mUser = nullptr;
	answer = NODATA;
}

const int GManager::GetScore() {
	return score;
}

const int GManager::GetAnswer() {
	return answer;
}

void GManager::GameRun() {
	cout << "<Up and Down 게임을 시작합니다>" << endl << " 정답은 1 부터 100 사이에 있습니다. " << endl;

	while (1) {
		int sel;

		cout << ++score << "번째 시도입니다. " << endl;
		cout << "입력 : ";
		cin >> sel;

		if (sel < 1 || sel > 100) {
			cout << "정답 범위를 초과했습니다. 정답은 1 부터 100 사이에 있습니다." << endl;
			score--;
		}

		if (answer > sel) {
			cout << "	<Up>" << endl;
		}
		else {
			cout << "	<Down>" << endl;
		}

		if (sel == answer) {
			if (mUser->getScore() == 0 || mUser->getScore() > score) {
				mUser->SetScore(score);
			}
			cout << "< Your Win ! >" << endl;
			cout << mUser->Getid() << " 플레이어의 현재 최고 기록은 " << mUser->getScore() << " 번 입니다." << endl;
			break;
		}
	}
}