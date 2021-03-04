#pragma once

#include "Global.h"

class CStudent {
private:
	char mName[NAMESIZE];
	char mID[IDSIZE];
	int score[SUB];
	double Grade;
public:
	CStudent(const char*, const char*);
	~CStudent();

	void SetName(const char*);
	void SetID(const char*);
	void SetScore(int, int);
	void GradeCheck();

	const char* GetName();
	const char* GetID();
	int GetScore(int);
	double GetGrade();
};