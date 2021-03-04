#pragma once
#include "MySQLDataBaseManager.h"
#include "ClientSection.h"

class LoginManager : public CMultiThreadSyns<LoginManager>
{
private:
	DECLARE_SINGLETONE(LoginManager)

	LoginManager();
	~LoginManager();
private:
	vector<UserInfo*>* mUser_List;
	list<UserInfo*>* mNowLoginUser_List;
public:
	bool UserDataLoad();

	void Begin();
	void End();

	bool Join(CClientSection* ptr);
	bool Login(CClientSection* ptr);
	void Logout(CClientSection* ptr);

	int Packing(char* buf, int result, const char* str);
	//int Packing(char* buf, const char* str);

	void UnPacking(const char* buf, char* id, char* pw);
	void UnPacking(const char* buf, char* id, char* pw, char* name);
};