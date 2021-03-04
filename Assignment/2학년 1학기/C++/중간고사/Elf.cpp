#include "Elf.h"

// �����ڿ��� �ʱ�ȭ�� ���� �θ� Ŭ�������� �Ѱ���
Elf::Elf(const char* _name) : Character(_name) {
	memset(brood, 0, sizeof(brood));
	memset(job, 0, sizeof(job));
}

Elf::~Elf() {

}

// ���� ������
void Elf::SetBrood(const char* _brood) {
	strcpy(brood, _brood);
}

void Elf::SetJob(const char* _job) {
	strcpy(job, _job);
}

// �б� ������
const char* Elf::GetBrood() {
	return brood;
}

const char* Elf::GetJob() {
	return job;
}