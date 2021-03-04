#pragma once

#include "Global.h"

class CStudent {
private:
	char mName[NAMESIZE];
	char mId[IDSIZE];
	int score[SUBSIZE];
	double mGrade;
public:
	CStudent(const char*, const char*);
	~CStudent();

	void SetName(const char*);
	void SetId(const char*);
	void SetScore(int, int);
	void GradeCheck();

	const char* GetName();
	const char* GetId();
	int GetScore(int);
	double GetGrade();
};