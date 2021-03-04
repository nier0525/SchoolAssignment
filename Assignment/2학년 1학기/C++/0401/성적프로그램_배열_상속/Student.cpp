#include "Student.h"

CStudent::CStudent(const char* _name, const char* _id) {
	strcpy(name, _name);
	strcpy(ID, _id);
	memset(score, 0, sizeof(score));
	grade = 0.0;
}

CStudent::~CStudent() {

}

const char* CStudent::GetName() {
	return name;
}

const char* CStudent::GetID() {
	return ID;
}

int CStudent::GetScore(int _index) {
	return score[_index];
}

double CStudent::GetGrade() {
	return grade;
}

void CStudent::SetName(const char* _name) {
	strcpy(name, _name);
}

void CStudent::SetID(const char* _id) {
	strcpy(ID, _id);
}

void CStudent::SetScore(int index, int temp) {
	score[index] = temp;
}

void CStudent::Grade() {

	double temp = 0.0;

	for (int i = 0; i < SUBSIZE; i++) {
		int s = score[i] / 10;
		int a = score[i] % 10;

		switch (s) {
		case 10:		
		case 9:
			if (a > 4) {
				temp = temp + (4.5 * 3);
			}
			else {
				temp = temp + (4.0 * 3);
			}
			break;

		case 8:
			if (a > 4) {
				temp = temp + (3.5 * 3);
			}
			else {
				temp = temp + (3.0 * 3);
			}
			break;

		case 7:
			if (a > 4) {
				temp = temp + (2.5 * 3);
			}
			else {
				temp = temp + (2.0 * 3);
			}
			break;

		case 6:
			if (a > 4) {
				temp = temp + (1.5 * 3);
			}
			else {
				temp = temp + (1.0 * 3);
			}
			break;
		}
	}

	grade = temp / 9.0;
}