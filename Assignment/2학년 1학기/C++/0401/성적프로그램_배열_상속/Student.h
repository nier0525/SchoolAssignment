#pragma once

#include "Global.h"

class CStudent {
private:
	char name[NAMESIZE];
	char ID[IDSIZE];
	int score[SUBSIZE];
	double grade;
public:
	CStudent(const char*, const char*);
	~CStudent();

	void SetName(const char*);
	void SetID(const char*);
	void SetScore(int,int);
	void Grade();

	const char* GetName();
	const char* GetID();
	int GetScore(int);
	double GetGrade();

};