#include "CharacterMenu.h"
#include "MainManager.h"

CharacterMenu* CharacterMenu::pthis = nullptr;

CharacterMenu* CharacterMenu::GetInstance() {
	if (!pthis) {
		pthis = new CharacterMenu();
	}

	CharacterManager::GetInstance();
	return pthis;
}

void CharacterMenu::Destory() {
	CharacterManager::Destroy();
	if (pthis) {
		delete pthis;
	}
}

// �޴� �޼���
void CharacterMenu::DrawMenu(CBackBit* _bit) {

	// ���� ������ ��ġ�� ������ ������ ����
	int logout_x = 80;
	int logout_y = 520;
	int del_x = 180;
	int del_y = 520;

	int sel_x = 700;
	int sel_y = 100;

	int make_x = 700;
	int make_y = 140;

	int remove_x = 700;
	int remove_y = 180;

	int x[MAXCOUNT];
	int y[MAXCOUNT];
	
	// ��Ʈ�� ��� ����
	HBITMAP bg;
	// RECT
	RECT rt;

	// ��Ʈ�� �Ҵ�
	bg = LoadBitmap(CWindowFrame::GetInstance()->GetHInstance(), MAKEINTRESOURCE(IDB_BITMAP2));

	// ���Ʈ�� DC �� ����
	HDC hdc = _bit->GetBitmapDC();

	// RECT ȣ��
	GetClientRect(mouse->GetHWnd(), &rt);
	// ��Ʈ�� �׸���
	_bit->GdiBitMapDraw(hdc, 0, 0, rt.right, rt.bottom, bg);

	Rectangle(hdc, logout_x + 40, logout_y + 20, logout_x - 40, logout_y - 20);	// �α׾ƿ�
	Rectangle(hdc, del_x + 40, del_y + 20, del_x - 40, del_y - 20);	// ȸ��Ż��

	Rectangle(hdc, sel_x + 40, sel_y + 15, sel_x - 40, sel_y - 15);	// ����
	Rectangle(hdc, make_x + 40, make_y + 15, make_x - 40, make_y - 15);	// ����
	Rectangle(hdc, remove_x + 40, remove_y + 15, remove_x - 40, remove_y - 15);	// ����

	User* NowUser = LoginManager::GetInstance()->GetNowUser();
	// ���� �������� ���� ��ü
	Character* ptr = nullptr;
	// �ӽ� ĳ���� ��ü


	// ĳ���Ͱ� ������ ������ ĳ����â�� �����Ǵ� ĳ���� ������ ����
	for (int i = 0; i < LoginManager::GetInstance()->GetNowUser()->GetCount(); i++) { // ĳ���� ������ for ��
		// ������ �ؽ�Ʈ ��ǥ
		x[i] = 310;
		y[i] = 100 + (i * 50);

		// ��� ����
		Rectangle(hdc, x[i] + 300, y[i] + 20 , x[i] - 300, y[i] - 20);
		if (NowUser->GetCharacter(&ptr)) {
			TextOut(hdc, x[i] - 290, y[i] - 10, "Name : ", 6);
			TextOut(hdc, x[i] - 230, y[i] - 10, ptr->GetNickName(), strlen(ptr->GetNickName()));
			TextOut(hdc, x[i] + 120, y[i] - 10, "Brood :", 7);
			if (dynamic_cast<Oak*>(ptr)) {
				TextOut(hdc, x[i] + 180, y[i] - 10, "Oak", 3);
			}
			if (dynamic_cast<Elf*>(ptr)) {
				TextOut(hdc, x[i] + 180, y[i] - 10, "Elf", 3);
			}
		}
	}

	// ��� �ʱ�ȭ
	NowUser->SearchEnd();

	// �ؽ�Ʈ ���
	TextOut(hdc, logout_x - 25, logout_y - 10, TEXT("LogOut"), 6);
	TextOut(hdc, del_x - 16 , del_y - 10, TEXT("Drop"), 4);
	TextOut(hdc, sel_x - 25, sel_y - 10, TEXT("Select"), 6);
	TextOut(hdc, make_x - 25, make_y - 10, TEXT("New"), 3);
	TextOut(hdc, remove_x - 25, remove_y - 10, TEXT("Delete"), 6);

	// ���콺 Ŭ�� �� �浹 �̺�Ʈ
	if (mouse->GetClick() && !New && !Remove && !Select) {
		KillTimer(mouse->GetHWnd(), 0);
		// �α׾ƿ� ��ư�� ������ ��
		if (col->CollisionEnterinPoint(logout_x, logout_y, mouse->GetX(), mouse->Gety(), 40, 20)) {
			LoginManager::GetInstance()->LogOut(); // �α׾ƿ� ���� ��ȯ
			MainManager::GetInstance()->SetDrawMenu(LoginMenu::GetInstance()); // ���� �������� �̿��� �α��� �޴��� ����
		}

		// ���(ȸ��Ż��) ��ư�� ������ ��
		if (col->CollisionEnterinPoint(del_x, del_y, mouse->GetX(), mouse->Gety(), 40, 20)) {
			LoginManager::GetInstance()->Delete(); // ���� �������� ���� ���� ����
			MainManager::GetInstance()->SetDrawMenu(LoginMenu::GetInstance()); // ���� ����
		}

		// ĳ���� ���� ��ư
		if (col->CollisionEnterinPoint(make_x, make_y, mouse->GetX(), mouse->Gety(), 40, 15)) {
			New = true;
		}

		// ĳ���� ���� ��ư
		if (col->CollisionEnterinPoint(sel_x, sel_y, mouse->GetX(), mouse->Gety(), 40, 15)) {
			Select = true;
		}

		// ĳ���� ���� ��ư
		if (col->CollisionEnterinPoint(remove_x, remove_y, mouse->GetX(), mouse->Gety(), 40, 15)) {
			Remove = true;
		}

		SetTimer(mouse->GetHWnd(), 0, 100, nullptr);

		// ųŸ�̸ӷ� Ÿ�̸Ӹ� �����ϰ�, ��� �۾� ���� �� �ٽ� Ÿ�̸Ӹ� ������ �Ѵ�.
		// if ���� ��� ���ư��� ������ �ذ��ϰ��� �̷��� ����Ͽ���.
	}

	// ����,����,���� ��ư�� �����ٸ�
	if (New || Select || Remove) {
		this->CharacterSeMenu(hdc, _bit); // ĳ���� ���� �޴� Ȱ��ȭ
	}

	DeleteObject(bg); // ��Ʈ�� �޸� ����
	_bit->DeleteBitmapDC(hdc); // DC ����
}

void CharacterMenu::CharacterSeMenu(HDC hdc, CBackBit* _bit) {

	// ���� ������ ��ġ
	int x1 = 280;
	int y1 = 200;

	int x2 = 520;
	int y2 = 200;

	int back_x = 400;
	int back_y = 350;

	int ok_x = 360;
	int ok_y = 330;

	int no_x = 440;
	int no_y = 330;

	// ��Ʈ�� ����
	HBITMAP oak, elf;

	// ��Ʈ�� �Ҵ�
	oak = LoadBitmap(CWindowFrame::GetInstance()->GetHInstance(), MAKEINTRESOURCE(IDB_BITMAP4));
	elf = LoadBitmap(CWindowFrame::GetInstance()->GetHInstance(), MAKEINTRESOURCE(IDB_BITMAP5));

	// ���� ��� �����
	SetBkMode(hdc, TRANSPARENT);

	// ĳ���� ���� �� ĳ���� ���ÿ� ���õ� �޴�
	if (!Sel_Oak && !Sel_Elf && !Remove && !Select) {

		// ���� ����
		Rectangle(hdc, x1 + 100, y1 + 100, x1 - 100, y1 - 100);
		Rectangle(hdc, x2 + 100, y2 + 100, x2 - 100, y2 - 100);
		Rectangle(hdc, back_x + 40, back_y + 20, back_x - 40, back_y - 20);

		// ��Ʈ�� �׸���
		_bit->GdiBitMapDraw(hdc, 180, 100, 200, 200, oak);
		_bit->GdiBitMapDraw(hdc, 420, 100, 200, 200, elf);

		// �ؽ�Ʈ ���
		TextOut(hdc, x1 - 10, y1, TEXT("Oak"), 3);
		TextOut(hdc, x2 - 10, y2, TEXT("Elf"), 3);
		TextOut(hdc, back_x - 15, back_y - 10, TEXT("Back"), 4);

		// ���콺 Ŭ�� ��
		if (mouse->GetClick()) {
			// ��ũ ���� ��
			if (col->CollisionEnterinPoint(x1, y1, mouse->GetX(), mouse->Gety(), 100, 100)) {
				Sel_Oak = true;
			}
			// ���� ���� ��
			if (col->CollisionEnterinPoint(x2, y2, mouse->GetX(), mouse->Gety(), 100, 100)) {
				Sel_Elf = true;
			}
			// ��� ��ư
			if (col->CollisionEnterinPoint(back_x, back_y, mouse->GetX(), mouse->Gety(), 40, 20)) {
				New = false;
			}
		}
	}

	// ���� ��ư�� ���� �̺�Ʈ�� ������.
	if (Sel_Oak || Sel_Elf || Select || Remove) {
		if (mouse->GetClick()) {	// ���콺 Ŭ�� ��
			if (strcmp(name, "")) {	// �̸��� ������ �ƴ϶��
				if (col->CollisionEnterinPoint(ok_x, ok_y, mouse->GetX(), mouse->Gety(), 30, 15)) {

					KillTimer(mouse->GetHWnd(), 0);

					// �Էµ� �̸��� ������ �޼��带 �����Ѵ�.

					if (Sel_Oak) {	// ��ũ ����
						if (CharacterManager::GetInstance()->CreateCharacter(name, OakFactory::GetInstance())) {
							MessageBox(mouse->GetHWnd(), "��ũ ������ �����߽��ϴ�.", "", MB_OK);
						}
						else {
							MessageBox(mouse->GetHWnd(), "ĳ���� ������ �����߽��ϴ�.", "", MB_OK);
						}
					}

					if (Sel_Elf) {	// ���� ����
						if (CharacterManager::GetInstance()->CreateCharacter(name, ElfFactory::GetInstance())) {
							MessageBox(mouse->GetHWnd(), "���� ������ �����߽��ϴ�.", "", MB_OK);
						}
						else {
							MessageBox(mouse->GetHWnd(), "ĳ���� ������ �����߽��ϴ�.", "", MB_OK);
						}
					}

					if (Select) {	// ĳ���� ����
						if (CharacterManager::GetInstance()->SelectCharacter(name)) {
							MainManager::GetInstance()->SetDrawMenu(GameMenu::GetInstance());
						}
						else {
							MessageBox(mouse->GetHWnd(), "�ش� ĳ���͸� ã�� �� �����ϴ�.", "", MB_OK);
						}
					}

					if (Remove) {	// ĳ���� ����
						if (CharacterManager::GetInstance()->DropCharacter(name)) {
							MessageBox(mouse->GetHWnd(), "�ش� ĳ���͸� �����մϴ�.", "", MB_OK);
						}
						else {
							MessageBox(mouse->GetHWnd(), "�ش� ĳ���͸� ã�� �� �����ϴ�.", "", MB_OK);
						}
					}

					// �� �ʱ�ȭ �۾�

					New = false;
					Sel_Oak = false;
					Sel_Elf = false;

					Select = false;
					Remove = false;

					KeyBoard::GetInstance()->Init_Char();
					memset(name, 0, sizeof(name));

					SetTimer(mouse->GetHWnd(), 0, 100, nullptr);
				}
			}

			if (col->CollisionEnterinPoint(no_x, no_y, mouse->GetX(), mouse->Gety(), 30, 15)) {	// ��� ��ư
				Sel_Oak = false;
				Sel_Elf = false;
				Select = false;
				Remove = false;

				KeyBoard::GetInstance()->Init_Char();
				memset(name, 0, sizeof(name));
			}
		}
	}

	// ĳ���� �̸� �Է� �޴�
	if (Sel_Oak || Sel_Elf || Select || Remove) {
		Rectangle(hdc, 300, 250, 500, 350);
		Rectangle(hdc, 320, 280, 480, 300);
		TextOut(hdc, 350, 260, TEXT("Character Name"), 14);

		Rectangle(hdc, ok_x + 30 , ok_y + 15 , ok_x - 30, ok_y - 15);
		Rectangle(hdc, no_x + 30, no_y + 15, no_x - 30, no_y - 15);
		TextOut(hdc, ok_x - 10, ok_y - 10, TEXT("OK"), 2);
		TextOut(hdc, no_x - 22, no_y - 10, TEXT("Cansel"), 6);

		TextOut(hdc, 325, 280, name, strlen(name));
	}

	DeleteObject(oak);
	DeleteObject(elf);
}