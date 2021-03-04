#include "Human.h"

// �����ڿ��� �ʱ�ȭ�� ���� �θ� Ŭ�������� �Ѱ���
Human::Human(const char* _name) : Character(_name) {
	memset(brood, 0, sizeof(brood));
	memset(job, 0, sizeof(job));
}

Human::~Human() {

}

// ���� ������
void Human::SetBrood(const char* _brood) {
	strcpy(brood, _brood);
}

void Human::SetJob(const char* _job) {
	strcpy(job, _job);
}

// �б� ������
const char* Human::GetBrood() {
	return brood;
}

const char* Human::GetJob() {
	return job;
}