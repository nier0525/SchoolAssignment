#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <ctime>
#include <cstdlib>

#include "Global.h"

class GManager {
private:
	int answer;
	int score;
public:
	GManager();
	~GManager();

	void init();
	
	bool SetAnswer(const int);
	bool Run();

	const int GetScore();
};

#endif
