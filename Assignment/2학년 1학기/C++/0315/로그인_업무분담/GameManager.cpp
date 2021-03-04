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
	cout << "<Up and Down ������ �����մϴ�>" << endl << " ������ 1 ���� 100 ���̿� �ֽ��ϴ�. " << endl;

	while (1) {
		int sel;

		cout << ++score << "��° �õ��Դϴ�. " << endl;
		cout << "�Է� : ";
		cin >> sel;

		if (sel < 1 || sel > 100) {
			cout << "���� ������ �ʰ��߽��ϴ�. ������ 1 ���� 100 ���̿� �ֽ��ϴ�." << endl;
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
			cout << mUser->Getid() << " �÷��̾��� ���� �ְ� ����� " << mUser->getScore() << " �� �Դϴ�." << endl;
			break;
		}
	}
}