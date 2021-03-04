#include "OakFactory.h"

OakFactory* OakFactory::mPthis = nullptr;

OakFactory* OakFactory::GetInstance() {
	if (!mPthis) {
		mPthis = new OakFactory();
	}

	OakItemFactory::GetInstance();

	return mPthis;
}

void OakFactory::Destroy() {
	OakItemFactory::Destroy();
	if (mPthis) {
		delete mPthis;
	}
}

// ��ũ Ŭ������ ��ĳ���� ���ְ� ��ȯ�Ѵ�.
Character* OakFactory::CreateCharacter(const char* _name, KindFactory* _kind) {
	Character* ptr = new Oak(_name, _kind);
	return ptr;
}

KindFactory* OakFactory::SelectKind() {
	KindFactory* kind = OakItemFactory::GetInstance();	// ���丮 ���� ��ĳ���� �� ��ȯ
	return kind;
}