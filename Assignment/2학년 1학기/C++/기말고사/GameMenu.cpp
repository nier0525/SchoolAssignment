#include "MainManager.h"


GameMenu* GameMenu::mPthis = nullptr;

GameMenu* GameMenu::GetInstance() {
	if (!mPthis) {
		mPthis = new GameMenu();
	}
	ItemStore::GetInstance();
	return mPthis;
}

void GameMenu::Destroy() {
	ItemStore::Destroy();
	if (mPthis) {
		delete mPthis;
	}
}

void GameMenu::DrawMenu(CBackBit* _bit) {
	// �� ������ ��ǥ
	int Back_x = 80;
	int Back_y = 520;

	int inven_x = 700;
	int inven_y = 100;

	int store_x = 700;
	int store_y = 140;

	int stats_x = 700;
	int stats_y = 180;

	// ��Ʈ�� �Լ�
	HBITMAP bg;
	// RECT
	RECT rt;

	// ��Ʈ�� �Ҵ�
	bg = LoadBitmap(CWindowFrame::GetInstance()->GetHInstance(), MAKEINTRESOURCE(IDB_BITMAP3));

	HDC hdc = _bit->GetBitmapDC();
	// Rect ȣ��
	GetClientRect(mouse->GetHWnd(), &rt);
	// ��Ʈ�� �׸���
	_bit->GdiBitMapDraw(hdc, 0, 0, rt.right, rt.bottom, bg);

	// ���� ��� ����
	SetBkMode(hdc, TRANSPARENT);

	Rectangle(hdc, Back_x + 40 , Back_y + 20 , Back_x - 40 , Back_y - 20); // �������� ���ư���

	Rectangle(hdc, inven_x + 40, inven_y + 15, inven_x - 40, inven_y - 15); // �κ��丮
	Rectangle(hdc, store_x + 40, store_y + 15, store_x - 40, store_y - 15); // ����
	Rectangle(hdc, stats_x + 40, stats_y + 15, stats_x - 40, stats_y - 15); // ����

	// ���ڿ� ���
	TextOut(hdc, Back_x - 16, Back_y - 10, TEXT("Back"), 4);
	TextOut(hdc, inven_x - 35, inven_y - 10, TEXT("Inventory"), 9);
	TextOut(hdc, store_x - 23, store_y - 10, TEXT("Store"), 5);
	TextOut(hdc, stats_x - 23, stats_y - 10, TEXT("Stats"), 5);

	if (mouse->GetClick()) {	// ���콺 Ŭ�� ��
		if (col->CollisionEnterinPoint(Back_x, Back_y, mouse->GetX(), mouse->Gety(), 40, 20)) {	 // �������� ���ư���
			// �ʱ�ȭ �۾� ���� �� ���� �޴��� ���ư���.
			stats = false;
			inven = false;
			store = false;
			on = false;

			KeyBoard::GetInstance()->Init_Char();
			memset(name, 0, sizeof(name));

			// ���� ����
			MainManager::GetInstance()->SetDrawMenu(CharacterMenu::GetInstance());
		}

		if (col->CollisionEnterinPoint(inven_x, inven_y, mouse->GetX(), mouse->Gety(), 40, 15)) { // �κ��丮 �޴� ��ư
			if (!inven) {
				inven = true;
				store = false;
			}
			else if (inven && !buy && !sell && !on) {
				inven = false;
			}
		}

		if (col->CollisionEnterinPoint(store_x, store_y, mouse->GetX(), mouse->Gety(), 40, 15)) {	// ���� �޴� ��ư
			if (!store) {
				store = true;
				inven = false;
			}
			else if (store && !buy && !sell && !on) {
				store = false;
			}
		}

		if (col->CollisionEnterinPoint(stats_x, stats_y, mouse->GetX(), mouse->Gety(), 40, 15)) {	// ���� �޴� ��ư
			if (!stats) {
				stats = true;
			}
			else {
				stats = false;
			}
		}
	}
	

	// ���� �κ��丮,����,���� �޴� Ȱ��ȭ
	if (inven) {
		this->ShowInven(hdc);
	}
	if (store) {
		this->ShowStore(hdc);
	}
	if (stats) {
		this->ShowStats(hdc);
	}

	// ������ �̸� �Է� �޴� Ȱ��ȭ
	if (buy || sell || on) {
		SelectItem(hdc);
	}

	// ��Ʈ�� ����
	//DeleteObject(bg);
	// DC ����
	_bit->DeleteBitmapDC(hdc);
}

void GameMenu::SelectItem(HDC hdc) {
	Character* NowCharacter = CharacterManager::GetInstance()->GetNowCharacter();
	// ���� �������� ĳ���͸� �ӽ� ������ ����

	// ���� ��ǥ
	int ok_x = 355;
	int ok_y = 170;

	int cansel_x = 445;
	int cansel_y = 170;

	// BackGround
	Rectangle(hdc, 300, 75, 500, 200);
	TextOut(hdc, 365, 90, TEXT("Item Name"), 9);

	// ������ �̸� �Է¶�
	Rectangle(hdc, 310, 120, 490, 140);
	TextOut(hdc, 320, 125, name, strlen(name));

	Rectangle(hdc, ok_x + 40, ok_y + 20, ok_x - 40, ok_y - 20);
	Rectangle(hdc, cansel_x + 40, cansel_y + 20, cansel_x - 40, cansel_y - 20);

	TextOut(hdc, ok_x - 10, ok_y - 10, TEXT("OK"), 2);
	TextOut(hdc, cansel_x - 25, cansel_y - 10, TEXT("Cansel"), 6);
	//--------------------------------------------------------------------------

	if (mouse->GetClick()) {	// ���콺 Ŭ�� ��
		if (strcmp(name, "")) { // ������ �̸��� ������ �ƴ϶��
			if (col->CollisionEnterinPoint(ok_x, ok_y, mouse->GetX(), mouse->Gety(), 40, 20)) { // OK ��ư

				KillTimer(mouse->GetHWnd(), 0);

				if (buy) {	// ������ ����
					if (ItemStore::GetInstance()->Sell(name)) {
						MessageBox(mouse->GetHWnd(), "�������� �����߽��ϴ�.", "", MB_OK);
					}
					else {
						MessageBox(mouse->GetHWnd(), "�������� ���� �� �� �����ϴ�.", "", MB_OK);
					}
				}
				if (sell) {	// ������ �Ǹ�
					if (ItemStore::GetInstance()->Buy(name)) {
						MessageBox(mouse->GetHWnd(), "�������� �Ǹ��߽��ϴ�.", "", MB_OK);
					}
					else {
						MessageBox(mouse->GetHWnd(), "�������� �Ǹ� �� �� �����ϴ�.", "", MB_OK);
					}
				}

				if (on) {	// ��� ����
					Item* ptr = nullptr;
					bool flug = false;

					while (NowCharacter->GetItem(&ptr)) {	 // ���� �������� ĳ������ �κ��丮 ��ȸ
						if (!strcmp(ptr->GetItemName(), name)) {	// ������ �������� ã�Ҵٸ�
							NowCharacter->ChangeItem(ptr);	// ��� ����
							MessageBox(mouse->GetHWnd(), "�ش� �������� �����߽��ϴ�.", "", MB_OK);
							NowCharacter->SearchEnd();	// ��� �ʱ�ȭ
							flug = true;
							break;
						}
					}
					NowCharacter->SearchEnd();	// ��� �ʱ�ȭ

					if (!flug) {	// �������� ã�� ����.
						MessageBox(mouse->GetHWnd(), "�ش� �������� �κ��丮�� �����ϴ�.", "", MB_OK);
					}
 				}
			}

			// �ʱ�ȭ �۾�

			buy = false;
			sell = false;
			on = false;

			KeyBoard::GetInstance()->Init_Char();
			memset(name, 0, sizeof(name));

			SetTimer(mouse->GetHWnd(), 0, 100, nullptr);
		}

		if (col->CollisionEnterinPoint(cansel_x, cansel_y, mouse->GetX(), mouse->Gety(), 40, 20)) { // ��� ��ư
			// �ʱ�ȭ �۾�
			buy = false;
			sell = false;
			on = false;

			KeyBoard::GetInstance()->Init_Char();
			memset(name, 0, sizeof(name));
		}
	}
}

void GameMenu::ShowInven(HDC hdc) {

	// ���� ��ǥ
	int on_x = 460;
	int on_y = 470;

	int cansel_x = 560;
	int cansel_y = 470;

	// ���� ��� ����
	SetBkMode(hdc, TRANSPARENT);

	// BackGround
	Rectangle(hdc, 200, 200, 600, 450);
	Rectangle(hdc, 210, 210, 590, 440);
	Rectangle(hdc, 210, 210, 590, 230);

	Rectangle(hdc, on_x + 40, on_y + 15, on_x - 40, on_y - 15); // �Ǹ�
	Rectangle(hdc, cansel_x + 40, cansel_y + 15, cansel_x - 40, cansel_y - 15); // ����

	// ���ڿ� ���
	TextOut(hdc, on_x - 35, on_y - 10, TEXT("Equipment"), 9);
	TextOut(hdc, cansel_x - 25, cansel_y - 10, TEXT("Cansel"), 6);

	TextOut(hdc, 220, 210, TEXT("Name"), 4);
	TextOut(hdc, 380, 210, TEXT("Price"), 5);
	TextOut(hdc, 500, 210, TEXT("Ability"), 7);

	// ���� �������� ĳ���Ͱ� ������ ������ ���
	CharacterManager::GetInstance()->ShowNowCharacterInven(hdc, 0, 0);

	if (mouse->GetClick()) {	 // ���콺 Ŭ�� ��
		if (col->CollisionEnterinPoint(on_x, on_y, mouse->GetX(), mouse->Gety(), 40, 15)) {	// ��� ���� ��ư
			on = true;
		}

		if (col->CollisionEnterinPoint(cansel_x, cansel_y, mouse->GetX(), mouse->Gety(), 40, 15)) { // ��� ��ư
			inven = false;
		}
	}
}

void GameMenu::ShowStore(HDC hdc) {
	// ���� ��ǥ
	int buy_x = 360;
	int buy_y = 470;

	int sell_x = 460;
	int sell_y = 470;

	int cansel_x = 560;
	int cansel_y = 470;

	// ���� ��� ����
	SetBkMode(hdc, TRANSPARENT);

	// BackGround
	Rectangle(hdc, 200, 200, 600, 450);
	Rectangle(hdc, 210, 210, 590, 440);
	Rectangle(hdc, 210, 210, 590, 230);

	Rectangle(hdc, buy_x + 40, buy_y + 15, buy_x - 40, buy_y - 15); // ����
	Rectangle(hdc, sell_x + 40, sell_y + 15, sell_x - 40, sell_y - 15); // �Ǹ�
	Rectangle(hdc, cansel_x + 40, cansel_y + 15, cansel_x - 40, cansel_y - 15); // ����

	// ���ڿ� ���
	TextOut(hdc, buy_x - 20, buy_y - 10, TEXT("Buy"), 3);
	TextOut(hdc, sell_x - 20, sell_y - 10, TEXT("Sell"), 4);
	TextOut(hdc, cansel_x - 25, cansel_y - 10, TEXT("Cansel"), 6);

	TextOut(hdc, 220, 210, TEXT("Name"), 4);
	TextOut(hdc, 380, 210, TEXT("Price"), 5);
	TextOut(hdc, 500, 210, TEXT("Ability"), 7);

	// ������ �Ǹ� ������ ����Ʈ ���
	ItemStore::GetInstance()->ShowItemInfo(hdc, 0, 0);

	if (mouse->GetClick() && !buy && !sell) {	// ���콺 Ŭ�� and ����,�Ǹ� ȥ�� ����
		if (col->CollisionEnterinPoint(buy_x, buy_y, mouse->GetX(), mouse->Gety(), 40, 15)) {	// ���� ��ư
			buy = true;
		}

		if (col->CollisionEnterinPoint(sell_x, sell_y, mouse->GetX(), mouse->Gety(), 40, 15)) {	// �Ǹ� ��ư
			sell = true;
		}

		if (col->CollisionEnterinPoint(cansel_x, cansel_y, mouse->GetX(), mouse->Gety(), 40, 15)) {	// ��� ��ư
			store = false;
		}
	}
}

void GameMenu::ShowStats(HDC hdc) {

	// ���� ��ǥ
	int esc_x = 140;
	int esc_y = 270;

	// ���� ��� ����
	SetBkMode(hdc, TRANSPARENT);

	// ���� ���ӵ� ĳ���� ����
	CharacterManager::GetInstance()->PrintCharacter(hdc, 0, 0);

	// �ݱ� ��ư
	Rectangle(hdc, esc_x + 30, esc_y + 15, esc_x - 30, esc_y - 15);

	TextOut(hdc, esc_x - 22, esc_y - 10, TEXT("Cansel"), 6);

	if (mouse->GetClick()) {	// ���콺 Ŭ�� ��
		if (col->CollisionEnterinPoint(esc_x, esc_y, mouse->GetX(), mouse->Gety(), 30, 15)) {	// ��� ��ư
			stats = false;
		}
	}
}
