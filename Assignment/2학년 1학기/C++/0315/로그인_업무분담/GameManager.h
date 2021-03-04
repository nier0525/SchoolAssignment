#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <ctime>
#include <cstdlib>

#include "User.h"

class GManager {
private:
	int answer;
	int score;
	CUser* mUser;
public:
	GManager();
	~GManager();

	void Gameinit(CUser*);
	
	void GameEnd();
	void GameRun();

	const int GetAnswer();
	const int GetScore();
};

#endif
