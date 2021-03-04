#include "CharacterManager.h"

CharacterManager* CharacterManager::mPthis = nullptr;

CharacterManager* CharacterManager::GetInstance() {
	if (!mPthis) {
		mPthis = new CharacterManager();
	}

	OakFactory::GetInstance();
	ElfFactory::GetInstance();

	return mPthis;
}

void CharacterManager::Destroy() {
	OakFactory::Destroy();
	ElfFactory::Destroy();

	if (mPthis) {
		delete mPthis;
	}
}

void CharacterManager::Init() {

}

// ĳ���� ���� ���
bool CharacterManager::CreateCharacter(const char* _name, CharacterFactory* charater) {
	User* user = LoginManager::GetInstance()->GetNowUser(); // ���� �������� ������ ����

	if (!user->CharacterCountCheak()) { // ������ �ִ� ĳ���� �������� �ʰ����� ���
		return false;
	}

	Character* ptr = nullptr;	// ĳ���� �ӽ� ������ ����

	// �Ű� ������ ���� �̸��� ���� ������ �����ϰ� �ִ� ĳ���Ϳ� �ߺ��� �ִ� �� �˻�
	while (user->GetCharacter(&ptr)) {
		if (!strcmp(ptr->GetNickName(), _name)) {	// �ߺ��� �ִٸ�
			user->SearchEnd();	// ��� ��ġ �ʱ�ȭ ��
			return false;	// false ����
		}
	}

	user->SearchEnd();	// ��� ��ġ �ʱ�ȭ

	user->NewCharacter(charater->MakeCharacter(_name));	
	// �ߺ��� ���ٸ� �Ű� ������ ���� �̸����� ĳ���� ���丮 Ŭ�������� ĳ���͸� �������ش�.
	// ���� ������ UI ���� ���丮 ������ ���� �Ű� ������ �Ѱ��ֱ� ������ ���⼭ �� �ʿ䰡 ����.
	return true;
}


// ĳ���� ������ ����ϴ� �޼ҵ�
bool CharacterManager::SelectCharacter(const char* _name) {
	User* user = LoginManager::GetInstance()->GetNowUser();	// ���� �������� ������ ����

	if (user->GetCount() == NODATA) {	// ������ ĳ���͸� �ϳ��� �����ϰ� ���� �ʴٸ�
		return false;
	}

	Character* ptr = nullptr;	// ĳ���� �ӽ� ������ ����

	// �Ű� ������ ���� �̸����� ������ �������� ĳ���� �� ���� �� ĳ���͸� ã�´�.
	while (user->GetCharacter(&ptr)) {
		if (!strcmp(ptr->GetNickName(), _name)) {	// ĳ���͸� ã�Ҵٸ�
			NowCharacter = ptr;	// ĳ���͸� ���� ���°� �ǹǷ� ���� ������ ĳ���͸� ���� ������ ����
			user->SearchEnd();	// ��� ��ġ �ʱ�ȭ
			return true;
		}
	}
	// ������ ���Ҵٸ� ��ã�� ��.
	user->SearchEnd();	// ��� ��ġ �ʱ�ȭ
	return false;
}

// ĳ���� ���� ��� �޼ҵ�
bool CharacterManager::DropCharacter(const char* _name) {
	User* user = LoginManager::GetInstance()->GetNowUser();	// ���� �������� ������ ����

	if (user->GetCount() == NODATA) {	// ������ ĳ���͸� �ϳ��� �����ϰ� ���� �ʴٸ�
		return false;
	}

	Character* ptr = nullptr;	// ĳ���� �ӽ� ������ ����

	// �Ű� ������ ���� �̸����� ������ �������� ĳ���� �� ���� �� ĳ���͸� ã�´�.
	while (user->GetCharacter(&ptr)) {
		if (!strcmp(ptr->GetNickName(), _name)) {	// ĳ���͸� ã�Ҵٸ�
			user->DelCharacter(ptr); // ������ ĳ���� ����Ʈ���� �ش� ĳ���͸� �����Ѵ�.
			user->SearchEnd();		// ��� ��ġ �ʱ�ȭ
			return true;
		}
	}
	// ������ ���Ҵٸ� ��ã�� ��.
	user->SearchEnd();	// ��� ��ġ �ʱ�ȭ
	return false;
}

// �Ʒ��� UI ���� ĳ������ �ɷ�ġ�� ĳ������ �κ��丮�� ����ϱ� ���� �޼ҵ��̴�.
// ���̳��� ĳ��Ʈ �˻縦 ���� Ŭ�������� �к��ϰ� �׿� ���� ������ ���� �����.

void CharacterManager::PrintCharacter(HDC hdc, int x, int y) {
	char level[CHAR];
	char att[CHAR];
	char def[CHAR];
	char hp[CHAR];
	char mp[CHAR];
	char money[CHAR];

	// int -> char ��ȯ
	// TextOut �� ������ ���ڿ��� �����..
	wsprintf(level, "%d", NowCharacter->GetLevel());
	wsprintf(att, "%d", NowCharacter->GetAtt());
	wsprintf(def, "%d", NowCharacter->GetDef());
	wsprintf(hp, "%d", NowCharacter->GetHp());
	wsprintf(mp, "%d", NowCharacter->GetMp());
	wsprintf(money, "%d", NowCharacter->GetMoney());

	Rectangle(hdc, 30, 30, 180, 300);

	// �̸�
	TextOut(hdc, 35, 45, TEXT("Name : "), 7);
	TextOut(hdc, 90, 45, NowCharacter->GetNickName(), strlen(NowCharacter->GetNickName()));

	// ����
	TextOut(hdc, 35, 65, TEXT("Brood : "), 8);
	if (dynamic_cast<Oak*>(NowCharacter)) {
		TextOut(hdc, 90, 65, TEXT("Oak"), 3);
	}
	if (dynamic_cast<Elf*>(NowCharacter)) {
		TextOut(hdc, 90, 65, TEXT("Elf"), 3);
	}

	// ����
	TextOut(hdc, 35, 85, TEXT("Level : "), 8);
	TextOut(hdc, 90, 85, level, strlen(level));

	// ���ݷ�
	TextOut(hdc, 35, 105, TEXT("Attack : "), 9);
	TextOut(hdc, 90, 105, att, strlen(att));

	// ����
	TextOut(hdc, 35, 125, TEXT("Defence : "), 10);
	TextOut(hdc, 100, 125, def, strlen(def));

	// ü��
	TextOut(hdc, 35, 145, TEXT("Health : "), 9);
	TextOut(hdc, 90, 145, hp, strlen(hp));

	// ����
	TextOut(hdc, 35, 165, TEXT("Magic : "), 8);
	TextOut(hdc, 90, 165, mp, strlen(mp));

	// �����ϰ� �ִ� ��
	TextOut(hdc, 35, 185, TEXT("Money : "), 8);
	TextOut(hdc, 90, 185, money, strlen(money));


	// �������� ����
	Rectangle(hdc, 180, 30, 450, 150);

	TextOut(hdc, 200, 40, TEXT("On Equipment"), 12);
	TextOut(hdc, 200, 80, TEXT("Wenpon : "), 9);
	TextOut(hdc, 200, 100, TEXT("Armor : "), 8);
	TextOut(hdc, 200, 120, TEXT("Accessories : "), 14);

	// �������� ���� �̸�
	TextOut(hdc, 270, 80, NowCharacter->GetMyWenpon()->GetItemName(), strlen(NowCharacter->GetMyWenpon()->GetItemName()));
	TextOut(hdc, 260, 100, NowCharacter->GetMyArmor()->GetItemName(), strlen(NowCharacter->GetMyArmor()->GetItemName()));
	TextOut(hdc, 300, 120, NowCharacter->GetMyAccessories()->GetItemName(), strlen(NowCharacter->GetMyAccessories()->GetItemName()));
}

void CharacterManager::ShowNowCharacterInven(HDC hdc, int x, int y) {
	char name[CHAR];
	char price[CHAR];
	char ability[CHAR];
	char ability2[CHAR];
	int i = 0;

	Item* ptr = nullptr;
	
	// ���� �������� ĳ������ �κ��丮�� ������ ��ȸ�Ѵ�.
	// �������� ���ٸ� �翬�� �ƹ��͵� ��µ��� �ʴ´�.
	while(NowCharacter->GetItem(&ptr)) {

		// ���ڿ� ��ȯ�� ���� �Լ�
		wsprintf(name, ptr->GetItemName());
		wsprintf(price, "%d", ptr->GetPrice());
		wsprintf(ability, "%d", ptr->GetFirstAbility());
		wsprintf(ability2, "%d", ptr->GetSecordAbility());

		// ���̳��� ĳ��Ʈ�� ���� �к��� �� ������ ���� ��񸶴� ���Ǵ� �ɷ��� �ٸ��� ������ �к��ߴ�.
		if (dynamic_cast<Wenpon*>(ptr)) {
			TextOut(hdc, 220, 230 + (20 * i), name, strlen(name));
			TextOut(hdc, 380, 230 + (20 * i), price, strlen(price));
			TextOut(hdc, 510, 230 + (20 * i), TEXT("Att + "), 6);
			TextOut(hdc, 540, 230 + (20 * i), ability, strlen(ability));
		}
		if (dynamic_cast<Armor*>(ptr)) {
			TextOut(hdc, 220, 230 + (20 * i), name, strlen(name));
			TextOut(hdc, 380, 230 + (20 * i), price, strlen(price));
			TextOut(hdc, 500, 230 + (20 * i), TEXT("Def + "), 6);
			TextOut(hdc, 540, 230 + (20 * i), ability, strlen(ability));
		}
		if (dynamic_cast<Accessories*>(ptr)) {
			TextOut(hdc, 220, 230 + (20 * i), name, strlen(name));
			TextOut(hdc, 380, 230 + (20 * i), price, strlen(price));
			TextOut(hdc, 450, 230 + (20 * i), TEXT("HP / MP + "), 10);
			TextOut(hdc, 520, 230 + (20 * i), ability, strlen(ability));
			TextOut(hdc, 550, 230 + (20 * i), TEXT(" / "), 3);
			TextOut(hdc, 560, 230 + (20 * i), ability2, strlen(ability2));
		}
		// i ������ while ���� �� �� �� ���Ŀ� ����ϴ� ���ڿ����� y ���� ���� ��Ű�� ����.
		i++;
	}

	// ��� �ʱ�ȭ
	NowCharacter->SearchEnd();
}