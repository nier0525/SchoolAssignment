#pragma once

#include "ListNode.h"

#include "Wenpon.h"
#include "Armor.h"
#include "Accessories.h"

#include "KindFactory.h"

class Character {
private:
	char Name[CHAR];	// ĳ���� �̸�

	int Level;			// ĳ���� ����
	int Hp;				// ĳ���� ü��
	int Mp;				// ĳ���� ����
	int Att;			// ĳ���� ���ݷ�
	int Def;			// ĳ���� ����

	int money;			// ĳ���Ͱ� �����ϰ� �ִ� ��
	int m_count;			// �κ��丮 ���� ������ ����

	Wenpon* m_wenpon;	// ���� �������� ����
	Armor* m_armor;		// ���� �������� ��
	Accessories* m_accessories;	// ���� �������� �Ǽ��縮

	ListNode<Item*>* Inven;	// ĳ���� �κ��丮
	Node<Item*>* SearchItem;	// �κ��丮 ��ġ ���

	KindFactory* kind;
	// ó�� ������ �������� �ٸ� ���⸦ �����ϱ� ���� ���丮 ���� Ŭ����
public:
	Character();
	Character(const char*, KindFactory*);	// �Ű� ������ �̸��� ���� ���丮 ����
	virtual ~Character();					

	// �б� ������
	const char* GetNickName();

	int GetLevel();
	int GetHp();
	int GetMp();
	int GetAtt();
	int GetDef();

	int GetMoney();
	int GetItemCount() {
		return m_count;
	}

	void SetMyWenpon(const char* name, int price, int att) {
		m_wenpon = new Wenpon(name, price, att);
	}

	void SetMyArmor(const char* name, int price, int def) {
		m_armor = new Armor(name, price, def);
	}

	void SetMyAccessories(const char* name, int price, int hp, int mp) {
		m_accessories = new Accessories(name, price, hp, mp);
	}

	Wenpon* GetMyWenpon() { return m_wenpon; }
	Armor* GetMyArmor() { return m_armor; }
	Accessories* GetMyAccessories() { return m_accessories; }

	// ���� ������
	void SetNickName(const char*);

	void LevelUp();
	void SetLevel(int);
	void SetHp(int);
	void SetMp(int);
	void SetAtt(int);
	void SetDef(int);
	void SetMoney(int);

	void SetItemCount(int _count) {
		m_count = _count;
	}

	// ������ ����,�Ǹ�
	bool Sell(Item*);
	bool Buy(Item*);

	// ���� ���� �ܾ� ���
	bool InCreseGameMoney(int);
	bool DeCreseGameMoney(int);

	// ���� ��� ��ü
	bool ChangeItem(Item*);

	// �ܺη� �ڷᱸ���� �������� �ʱ� ���� ��ü������ ����Ʈ�� ��ȸ�ϴ� �Լ�
	void SearchEnd();
	bool GetItem(Item**);

	bool LoadInvenItem(Item* item) {
		if (item != nullptr) {
			Inven->Insert(item);
			return true;
		}
		return false;
	}
};