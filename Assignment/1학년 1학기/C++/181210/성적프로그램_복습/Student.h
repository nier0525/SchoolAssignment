#ifndef CSTUDENT_H
#define CSTUDENT_H

#include "Base.h"

class cStudent {
private:
	char mId[IDSIZE];
	char mName[NAMESIZE];
	int mSubject[3];
	double mGrade;
public:
	cStudent(const char*, const char*);
	~cStudent();

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