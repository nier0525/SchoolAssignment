#ifndef CSTUDENT_H
#define CSTUDENT_H
#include "Global.h"

class CStudent
{
private:
	char mId[IDSIZE];
	char mName[NAMESIZE];
	int mSubject[3];
	double mGrade;

public:
	CStudent(const char*, const char*);
	~CStudent();

	void SetId(const char*);
	void SetName(const char*);

	const char* GetId();
	const char* GetName();

	void SetSubject(int, int);
	int GetSubject(int);

	double GetGrade();

	void CalculateGrade();
};
#endif