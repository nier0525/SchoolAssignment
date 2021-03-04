#pragma once

#include "GameManager.h"

// �α��� �޴� Ŭ����
// ȸ������,�α���,���� ���� ����ϴ� ���� ���� Ÿ��Ʋ ȭ�� ������ ����ϴ� Ŭ����
// �̱��� ����

class LoginMenu : public GameManager {
private:
	static LoginMenu* pthis;
	// ������ �ʱ�ȭ �۾�
	LoginMenu() {
		memset(id, 0, sizeof(id));
		memset(pass, 0, sizeof(pass));
	}
	~LoginMenu() {}

	char id[CHAR];	// ID 
	char pass[CHAR];	// PASSWORD

	bool ID = true;
	bool PASS = false;
public:
	static LoginMenu* GetInstance();
	static void Destory();

	// �α��� �޴�
	void DrawMenu(CBackBit* _bit);

	// ���� ������
	void SetId(const char* _id) { strcpy(id,_id); }
	void SetPass(const char* _pass) { strcpy(pass, _pass); }

	// �б� ������
	const char* GetId() { return id; }
	const char* GetPass() { return pass; }

	bool GetIdState() { return ID; }
	bool GetPassState() { return PASS; }
};