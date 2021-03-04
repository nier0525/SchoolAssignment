#include "Student.h"

CStudent::CStudent(const char* _name, const char* _id) {
	strcpy_s(mId, _id);
	strcpy_s(mName, _name);
	memset(mSubject, 0, sizeof(mSubject));
	mGrade = 0.0;
}

CStudent::~CStudent() {}

void CStudent::SetId(const char* _id) {
	strcpy_s(mId, _id);
}

void CStudent::SetName(const char* _name) {
	strcpy_s(mName, _name);
}

const char* CStudent::GetId() {
	return mId;
}

const char* CStudent::GetName() {
	return mName;
}

void CStudent::SetSubject(int _sub, int _index) {
	mSubject[_index] = _sub;
}

int CStudent::GetSubject(int _index) {
	return mSubject[_index];
}

double CStudent::GetGrade() {
	return mGrade;
}

void CStudent::CalculateGrade() {
	double temp = 0.0;
	for (int i = 0; i < 3; i++) {
		int score = mSubject[i] / 10;
		int m = mSubject[i] % 10;
		switch (score) {
		case 10:
		case 9:
			if (m > 4) {
				temp = temp + (4.5 * 3);
			}
			else
			{
				temp = temp + (4.0 * 3);
			}
		case 8:
			if (m > 4) {
				temp = temp + (3.5 * 3);
			}
			else
			{
				temp = temp + (3.0 * 3);
			}
		case 7:
			if (m > 4) {
				temp = temp + (2.5 * 3);
			}
			else
			{
				temp = temp + (2.0 * 3);
			}
		case 6:
			if (m > 4) {
				temp = temp + (1.5 * 3);
			}
			else
			{
				temp = temp + (1.0 * 3);
			}
			break;
		}
	}
	mGrade = temp / 9.0;
}