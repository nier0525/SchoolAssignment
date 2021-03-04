#ifndef CUSER_H
#define CUSER_H

#include "Global.h"

class CUser
{
private:
	char	mId[10];
	char	mPw[10];
	bool	mLoginState;
	int		mHighGameScore;
	
public:
	CUser(const char*, const char*);
	~CUser();

	const char* GetId();
	const char* GetPw();
	bool	GetLoginState();
	int		GetHighGameScore();

	bool SetId(const char*);
	bool SetPw(const char*);
	void SetLogin();
	void SetLogout();
	void SetHighGameScore(int);
};

#endif