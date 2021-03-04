#pragma once

#include "LoginManager.h"
#include "CharacterManager.h"
#include "ItemManager.h"

// �ֻ��� �Ŵ��� Ŭ����
class GameManager {
private:
	CLoginManager* LM;		// �α��� �Ŵ���
	CharacterManager* CM;	// ĳ���� �Ŵ���
	ItemManager* IM;		// ������ �Ŵ���
public:
	GameManager();
	~GameManager();

	void Init(); // �ʱ�ȭ �۾�
	void Run();  // ��ü���� ���� �۾�

	int LoginMenu(); // �α��� �޴�
	int GameMenu();  // ���� �޴�
};