#ifndef _INGAMEMANAGER_H
#define _INGAMEMANAGER_H

#include "Global.h"
#include "CUser.h"

class CInGameManager
{
private:
	int		mCorrectAnswer;
	int		mAnswerCount;
	CUser*  mCurrUser;

public:
	CInGameManager();
	~CInGameManager();

	int		GetCorrectAnswer();
	int		GetAnswerCount();

	void	GameStart(CUser*);
	void	GameOver();

	void	GameRun();

};

#endif // !_INGAMEMANAGER_H


