#include "Oak.h"

// 생성자에서 초기화한 값은 부모 클래스에게 넘겨줌
Oak::Oak(const char* _name) : Character(_name) {
	memset(brood, 0, sizeof(brood));
	memset(job, 0, sizeof(job));
}

Oak::~Oak() {

}

// 쓰기 접근자
void Oak::SetBrood(const char* _brood) {
	strcpy(brood, _brood);
}

void Oak::SetJob(const char* _job) {
	strcpy(job, _job);
}

// 읽기 접근자
const char* Oak::GetBrood() {
	return brood;
}

const char* Oak::GetJob() {
	return job;
}