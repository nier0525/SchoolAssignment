#include "LoginMenu.h"
#include "MainManager.h"

LoginMenu* LoginMenu::pthis = nullptr;

LoginMenu* LoginMenu::GetInstance() {
	if (!pthis) {
		pthis = new LoginMenu();
	}
	LoginManager::GetInstance();
	return pthis;
}

void LoginMenu::Destory() {
	LoginManager::Destory();

	if (pthis) {
		delete pthis;
	}
}

void LoginMenu::DrawMenu(CBackBit* _bit) {

	// ���� ��ǥ
	int id_x = 400;
	int id_y = 280;
	int pass_x = 400;
	int pass_y = 320;

	int new_x = 340;
	int new_y = 380;
	int login_x = 460;
	int login_y = 380;
	int Exit_x = 400;
	int Exit_y = 420;

	// Rect
	RECT rt;

	// ��Ʈ�� ����
	HBITMAP bg;
	// ��Ʈ�� �Ҵ�
	bg = LoadBitmap(CWindowFrame::GetInstance()->GetHInstance(), MAKEINTRESOURCE(IDB_BITMAP1));

	HDC hdc = _bit->GetBitmapDC();

	// Rect ȣ��
	GetClientRect(mouse->GetHWnd(), &rt);
	// ��Ʈ�� �׸���
	_bit->GdiBitMapDraw(hdc, 0, 0, rt.right, rt.bottom, bg);

	Rectangle(hdc, id_x + 100, id_y + 10, id_x - 100, id_y - 10); // ID �Է¶�
	Rectangle(hdc, pass_x + 100, pass_y + 10, pass_x - 100, pass_y - 10); // PASS �Է¶�

	Rectangle(hdc, new_x + 40, new_y + 20, new_x - 40, new_y - 20); // ȸ������
	Rectangle(hdc, login_x + 40, login_y + 20, login_x - 40, login_y - 20); // �α���
	Rectangle(hdc, Exit_x + 100, Exit_y + 10, Exit_x - 100, Exit_y - 10); // ����

	// ���� ��� ����
	SetBkMode(hdc, TRANSPARENT);

	// �� UI �ؽ�Ʈ
	TextOut(hdc, id_x - 120, id_y - 10, TEXT("ID"), 2);
	TextOut(hdc, pass_x - 185, pass_y - 10, TEXT("PASSWORD"), 8);
	TextOut(hdc, new_x - 15, new_y - 10, TEXT("New"), 3);
	TextOut(hdc, login_x - 17, login_y - 10, TEXT("Login"), 5);
	TextOut(hdc, Exit_x - 16, Exit_y - 10, TEXT("EXIT"), 4);

	if (ID) {
		Ellipse(hdc, id_x + 130 , id_y + 10, id_x + 110, id_y - 10);	// ID ����
	}
	if (PASS) {
		Ellipse(hdc, pass_x + 130, pass_y + 10, pass_x + 110, pass_y - 10); // PASSWORD ����
	}

	// id, pass ���
	TextOut(hdc, id_x - 90, id_y - 10, id, strlen(id));	
	TextOut(hdc, pass_x - 90, pass_y - 10, pass, strlen(pass));
	
	if (mouse->GetClick()) {	// ���콺 Ŭ�� ��
		if (col->CollisionEnterinPoint(id_x, id_y, mouse->GetX(), mouse->Gety(), 100, 10)) {	// ID �Է¶�
			ID = true;
			PASS = false;
			memset(id, 0, sizeof(id));
			KeyBoard::GetInstance()->Init_Char();
		}

		else if (col->CollisionEnterinPoint(pass_x, pass_y, mouse->GetX(), mouse->Gety(), 100, 10)) {	// PASSWORD �Է¶�
			ID = false;
			PASS = true;
			memset(pass, 0, sizeof(pass));
			KeyBoard::GetInstance()->Init_Char();
		}

		else {	// �� �ܿ� ���� Ŭ���Ѵٸ� �ʱ�ȭ
			ID = false;
			PASS = false;
		}

		if (col->CollisionEnterinPoint(new_x, new_y, mouse->GetX(), mouse->Gety(), 40, 20) && strcmp(id, "") && strcmp(pass, "")) {	// ȸ������ ��ư
			
			KillTimer(mouse->GetHWnd(), 0);

			if (LoginManager::GetInstance()->New(id, pass)) {
				MessageBox(mouse->GetHWnd(), "ȸ������ �Ǿ����ϴ�.", "", MB_OK);
			}
			else {
				MessageBox(mouse->GetHWnd(), "�ߺ��� ID �Դϴ�.", "", MB_OK);
			}
		}
	
		if (col->CollisionEnterinPoint(login_x, login_y, mouse->GetX(), mouse->Gety(), 40, 20) && strcmp(id, "") && strcmp(pass, "")) {	// �α��� ��ư
		
			KillTimer(mouse->GetHWnd(), 0);

			if (LoginManager::GetInstance()->Login(id, pass)) {
				MainManager::GetInstance()->SetDrawMenu(CharacterMenu::GetInstance());	// �α��� ���� �� ĳ���� �޴��� ��ȯ
				KeyBoard::GetInstance()->Init_Char();
			}
			else {
				MessageBox(mouse->GetHWnd(), "ID �Ǵ� PASSWORD �� ��ġ���� �ʽ��ϴ�.", "", MB_OK);
			}
		}

		if (col->CollisionEnterinPoint(Exit_x, Exit_y, mouse->GetX(), mouse->Gety(), 100, 10)) {	// ���� ��ư
			PostQuitMessage(0); // ������ ���ν��� ����
		}

		mouse->SetclickUp();
		SetTimer(mouse->GetHWnd(), 0, 10, nullptr);
	}

	// ��Ʈ��,DC ����
	DeleteObject(bg);
	_bit->DeleteBitmapDC(hdc);
}