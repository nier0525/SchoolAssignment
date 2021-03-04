#include "CharacterFactory.h"

CharacterFactory* CharacterFactory::pthis = nullptr;

CharacterFactory* CharacterFactory::GetInstance() {
	if (!pthis) {
		pthis = new CharacterFactory();
	}
	return pthis;
}

void CharacterFactory::Destory() {
	if (pthis) {
		delete pthis;
		pthis = nullptr;
	}
}

CharacterFactory::CharacterFactory() {

}

CharacterFactory::~CharacterFactory() {

}

Character* CharacterFactory::NewCharacter(const char* _name, int _brood , int _job) {
	Character* ptr = nullptr;

	switch (_brood)
	{
	case ELF:
		ptr = new Elf(_name);
		break;

	case OAK:
		ptr = new Oak(_name);
		break;

	case HUMAN:
		ptr = new Human(_name);
		break;
	}
	ptr->SetJobCode(_job);
	return ptr;
}