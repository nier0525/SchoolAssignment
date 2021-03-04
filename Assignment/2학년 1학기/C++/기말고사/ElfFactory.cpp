#include "ElfFactory.h"
#include "ElfItemFactory.h"

ElfFactory* ElfFactory::mPthis = nullptr;

ElfFactory* ElfFactory::GetInstance() {
	if (!mPthis) {
		mPthis = new ElfFactory();
	}

	ElfItemFactory::GetInstance();

	return mPthis;
}

void ElfFactory::Destroy() {
	ElfItemFactory::Destroy();
	
	if (mPthis) {
		delete mPthis;
	}
}

// ���� Ŭ������ ��ĳ���� ���ְ� ��ȯ�Ѵ�.
Character* ElfFactory::CreateCharacter(const char* _name, KindFactory* _kind) {
	Character* ptr = new Elf(_name, _kind);
	return ptr;
}


KindFactory* ElfFactory::SelectKind() {
	KindFactory* ptr = ElfItemFactory::GetInstance(); // ���丮 ���� ��ĳ���� �� ��ȯ
	return ptr;
}