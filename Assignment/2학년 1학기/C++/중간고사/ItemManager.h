#pragma once

#include "Portion.h"
#include "Armor.h"
#include "Wenpon.h"

#include "ListNode.h"
#include "User.h"

// ������ ����,�ǸŸ� ����ϴ� �Ŵ��� Ŭ����
// ���� �Ŵ��� Ŭ�������� ���� �������� ���� ���� ����
// ���� ���� ��ü�� ������ �Ŵ����� ���� ��ü�� ������ ����
class ItemManager {
private:
	static const char* ItemName[]; // ������ ���

	static const char* PortionList[]; // ���� ����
	static const char* ArmorList[];	  // �� ����
	static const char* WenponList[];  // ���� ����

	ListNode<Item*>* Itemlist[4];	// ����,��,���� ������ ������ ����Ʈ
	Node<Item*>* NowItem;			// �������� ��ȸ�� ���
public:
	ItemManager();
	~ItemManager();

	int Menu();	// ����,��,���� ���� �޴�
	const char* ItemMenu(int);	// ������ �� ���� �޴�

	void Init();	// ��ü ������ ���� �ʱ�ȭ �Լ�
	void Buy(Node<CUser*>*);	// ������ ����
	void Sell(Node<CUser*>*);	// ������ �Ǹ�
};