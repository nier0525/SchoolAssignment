#include "Oak.h"

// �����ڿ��� �ʱ�ȭ�� ���� �θ� Ŭ�������� �Ѱ���
Oak::Oak(const char* _name) : Character(_name) {
	memset(brood, 0, sizeof(brood));
	memset(job, 0, sizeof(job));
}

Oak::~Oak() {

}

// ���� ������
void Oak::SetBrood(const char* _brood) {
	strcpy(brood, _brood);
}

void Oak::SetJob(const char* _job) {
	strcpy(job, _job);
}

// �б� ������
const char* Oak::GetBrood() {
	return brood;
}

const char* Oak::GetJob() {
	return job;
}