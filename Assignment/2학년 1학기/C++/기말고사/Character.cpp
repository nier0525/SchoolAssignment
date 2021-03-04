#include "Character.h"

Character::Character() {
	memset(Name, 0, sizeof(Name));
	
	Level = NODATA;
	Hp = NODATA;
	Mp = NODATA;
	Att = NODATA;
	Def = NODATA;

	m_wenpon = nullptr;
	m_armor = nullptr;
	m_accessories = nullptr;

	money = NODATA;
	m_count = NODATA;

	Inven = new ListNode<Item*>();
	SearchItem = nullptr;
}

Character::Character(const char* _name, KindFactory* _kind) {
	// �ʱ�ȭ
	strcpy(Name, _name);

	Level = 1;
	Hp = 500;
	Mp = 200;
	Att = 20;
	Def = 10;

	// ���丮 ���Ͽ��� ���� ���ڰ�
	kind = _kind;

	// ���丮 ���� Ŭ�������� ������ ������ ���� ���� ��� �� ����
	m_wenpon = kind->CreateWenpon();
	m_wenpon->OnItem(this);
	m_armor = kind->CreateArmor();
	m_armor->OnItem(this);
	m_accessories = kind->CreteAccessories();
	m_accessories->OnItem(this);

	// �� �ʱ�ȭ
	money = GAMEMONEY;
	m_count = NODATA;

	// �κ��丮 �ʱ�ȭ �� ����Ʈ ���� �Ҵ�
	Inven = new ListNode<Item*>();
	SearchItem = nullptr;
}

// ���� �Ҵ� �� ��ü�� �޸� ����
Character::~Character() {
	if (m_wenpon != nullptr) {
		delete m_wenpon;
	}

	if (m_armor != nullptr) {
		delete m_armor;
	}

	if (m_accessories != nullptr) {
		delete m_accessories;
	}

	if (Inven != nullptr) {
		delete Inven;
	}
}

//------------------------------------------------------ ���� ������

void Character::SetNickName(const char* _name) {
	strcpy(Name, _name);
}

void Character::SetLevel(int _level) {
	Level = _level;
}

void Character::SetHp(int _hp) {
	Hp = _hp;
}

void Character::SetMp(int _mp) {
	Mp = _mp;
}

void Character::SetAtt(int _att) {
	Att = _att;
}

void Character::SetDef(int _def) {
	Def = _def;
}

void Character::LevelUp() {
	Level++;
}

void Character::SetMoney(int _money) {
	money = _money;
}

//---------------------------------------------------- �б� ������

const char* Character::GetNickName() {
	return Name;
}

int Character::GetLevel() {
	return Level;
}

int Character::GetHp() {
	return Hp;
}

int Character::GetMp() {
	return Mp;
}

int Character::GetAtt() {
	return Att;
}

int Character::GetDef() {
	return Def;
}

int Character::GetMoney() {
	return money;
}

//----------------------------------------------------


bool Character::Sell(Item* _item) {
	SearchItem = Inven->GetHead()->GetNext();
	while (SearchItem) {	// ����Ʈ ��ȸ
		if (SearchItem->GetData() == _item) {	// �Ű� ����(ã�� ������) �� ��ġ�Ѵٸ�
			Item* ptr = SearchItem->GetData();	// �ӽ� ������ ������ ����
			this->InCreseGameMoney(ptr->GetPrice());	// ���� ���
			Inven->Remove(SearchItem->GetData());		// �ش� ������ ����Ʈ���� ����
			m_count--;
			this->SearchEnd();							// ��ȸ�� ��� ��ġ �ʱ�ȭ
			return true;
		}
		SearchItem = SearchItem->GetNext();				// ���� ����
	}
	return false;
}

bool Character::Buy(Item* _item) {
	if (_item != nullptr) {					
		if (this->DeCreseGameMoney(_item->GetPrice())) {	// ���� ��� �� ����
			Inven->Insert(_item);	// ����Ʈ�� �߰�
			m_count++;
			return true;
		}
	}
	else {
		return false;
	}
}

// �� ����� ���� �޼ҵ�

bool Character::InCreseGameMoney(int _money) {
	money += _money;
	return true;
}

bool Character::DeCreseGameMoney(int _money) {
	if (money - _money < NODATA) {
		return false;
	}
	else {
		money -= _money;
		return true;
	}
}

// ----------------------


// ��� ����
bool Character::ChangeItem(Item* _item) {
	if (!Inven->Delete(_item)) {	// �κ��丮�� �������� ����.
		return false;
	}

	// ���̳��� ĳ��Ʈ�� �ش� �������� Ŭ�������� �����ϰ�, ���� ��Ʈ�� ��� �����Ѵ�.
	if (dynamic_cast<Wenpon*>(_item)) {
		m_wenpon->OffItem(this);	// ���� �����ϰ� �ִ� ���� ����
		Inven->Insert(m_wenpon);	// �ٽ� �κ��丮�� �ִ´�.
		m_wenpon = dynamic_cast<Wenpon*>(_item);	// �׸��� ���ο� ��� ����

		m_wenpon->OnItem(this);	// ��� ���� �� ���� ����
		return true;
	}

	// ���� ����

	if (dynamic_cast<Armor*>(_item)) {
		m_armor->OffItem(this);
		Inven->Insert(m_armor);
		m_armor = dynamic_cast<Armor*>(_item);

		m_armor->OnItem(this);
		return true;
	}

	if (dynamic_cast<Accessories*>(_item)) {
		m_accessories->OffItem(this);
		Inven->Insert(m_accessories);
		m_accessories = dynamic_cast<Accessories*>(_item);

		m_accessories->OnItem(this);
		return true;
	}

	return false;
}

//----------------------------------------------------

// ��ȸ ��� �ʱ�ȭ
void Character::SearchEnd() {
	SearchItem = nullptr;
}

// ��� ��ȸ �޼ҵ�
bool Character::GetItem(Item** temp) {
	if (SearchItem == nullptr) {	// ��ȸ ��尡 nullptr �̶��
		SearchItem = Inven->GetHead();	// ��ȸ ����� ��ġ�� ���� ó������ �̵�
	}

	SearchItem = SearchItem->GetNext();	// ��ȸ ����

	if (SearchItem != nullptr) {	// ��ȸ ��尡 nullptr �� �ƴ� �����͸� ������
		*temp = SearchItem->GetData();	// �ش� ��忡 ���� ĳ������ ��ȸ ��� ��ġ�� �������� �������ش�.
		return true;
	}
	return false;
}
