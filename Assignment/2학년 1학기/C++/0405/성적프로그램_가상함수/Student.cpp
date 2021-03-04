#include "Student.h"

CStudent::CStudent(const char* _name, const char* _id) {
	strcpy(mName, _name);
	strcpy(mID, _id);
	memset(score, 0, sizeof(score));
	Grade = 0.0;
}

CStudent::~CStudent() {

}

void CStudent::SetName(const char* _name) {
	strcpy(mName, _name);
}

void CStudent::SetID(const char* _id) {
	strcpy(mID, _id);
}

void CStudent::SetScore(int index, int _score) {
	score[index] = _score;
}

const char* CStudent::GetName() {
	return mName;
}

const char* CStudent::GetID() {
	return mID;
}

int CStudent::GetScore(int index) {
	return score[index];
}

void CStudent::GradeCheck() {
	double temp = 0.0;

	for (int i = 0; i < SUB; i++) {
		int _score = score[i] / 10;
		int m = score[i] % 10;

		switch (_score) {
		case 10:
		case 9:
			if (m > 4) {
				temp = temp + (4.5 * 3);
			}
			else {
				temp = temp + (4.0 * 3);
			}
			break;
		case 8:
			if (m > 4) {
				temp = temp + (3.5 * 3);
			}
			else {
				temp = temp + (3.0 * 3);
			}
			break;
		case 7:
			if (m > 4) {
				temp = temp + (2.5 * 3);
			}
			else {
				temp = temp + (2.0 * 3);
			}
			break;
		case 6:
			if (m > 4) {
				temp = temp + (1.5 * 3);
			}
			else {
				temp = temp + (1.0 * 3);
			}
			break;
		}
	}
	Grade = temp / 9.0;
}

double CStudent::GetGrade() {
	return Grade;
}