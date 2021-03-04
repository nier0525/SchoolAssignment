#include "Human.h"

// 생성자에서 초기화한 값은 부모 클래스에게 넘겨줌
Human::Human(const char* _name) : Character(_name) {
	memset(brood, 0, sizeof(brood));
	memset(job, 0, sizeof(job));
}

Human::~Human() {

}

// 쓰기 접근자
void Human::SetBrood(const char* _brood) {
	strcpy(brood, _brood);
}

void Human::SetJob(const char* _job) {
	strcpy(job, _job);
}

// 읽기 접근자
const char* Human::GetBrood() {
	return brood;
}

const char* Human::GetJob() {
	return job;
}