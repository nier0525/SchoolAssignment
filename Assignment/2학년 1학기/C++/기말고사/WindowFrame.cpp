#include "WindowFrame.h"
#include "MainManager.h"
#include "FileDataBase.h"

CWindowFrame* CWindowFrame::mPthis=NULL;

CWindowFrame::CWindowFrame()
{	
	mBackbit = nullptr;
	mHinstance = nullptr;
	mHWnd = nullptr;
}

CWindowFrame* CWindowFrame::Create(HINSTANCE _hinstance)
{
	if(!mPthis)
	{
		mPthis=new CWindowFrame();
	}

	CWindowFrame::GetInstance()->mHinstance=_hinstance;

	CMouse::Create();
	KeyBoard::GetInstance();
	Collision::GetInstance();

	MainManager::GetInstance();
	FileDataSystem::GetInstance();

	return mPthis;
}

CWindowFrame* CWindowFrame::GetInstance()
{	
	return mPthis;
}

void CWindowFrame::Destroy()
{
	FileDataSystem::GetInstance()->DataSave();	// ������ �ı� ���� ���Ͽ� ������ ������ ����

	CMouse::Destroy();
	KeyBoard::Destory();
	Collision::Destory();

	MainManager::Destory();
	FileDataSystem::Destroy();

	if(mPthis)
	{
		delete mPthis;
		mPthis=NULL;
	}	
}

LRESULT CALLBACK CWindowFrame::WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;	
	RECT crt;
	
	switch (iMessage) {

		// �ʱ�ȭ �۾�
	case WM_CREATE:
		CMouse::GetInstance()->Initialize(hWnd);
		KeyBoard::GetInstance()->Init(hWnd, wParam);
		GetClientRect(hWnd, &crt);
	    mPthis->mBackbit=new CBackBit(hWnd, crt.right , crt.bottom );
		MainManager::GetInstance()->SetDrawMenu(LoginMenu::GetInstance());
		SetTimer(hWnd, 0, 10, nullptr);
		return 0;

	case WM_LBUTTONDOWN:
		CMouse::GetInstance()->SetclickDown();	
		CMouse::GetInstance()->SetXY(lParam);
		// ���콺�� Ŭ�� �Ǿ��� �� ���콺 ��ǥ�� �޴´�.
		return 0;

	case WM_LBUTTONUP:	
		CMouse::GetInstance()->SetclickUp();
		// ���콺 ��
		return 0;

	case WM_CHAR:
		// ���ڿ� �Է�
		KeyBoard::GetInstance()->SetKey(wParam);	// wParam �� �Ѱ���
		if (KeyBoard::GetInstance()->GetKeyCode() != VK_RETURN) {	// Enter Ű�� ������ ���ڿ��� ���� ����
			if (LoginMenu::GetInstance()->GetIdState()) {	 // ID �Է�
				LoginMenu::GetInstance()->SetId(KeyBoard::GetInstance()->GetKeyChar());	// Ű����� �Էµ� ���ڿ��� �Ѱ��ش�.
			}
			if (LoginMenu::GetInstance()->GetPassState()) {		// PASSWORD �Է�
				LoginMenu::GetInstance()->SetPass(KeyBoard::GetInstance()->GetKeyChar());	// ''
			}

			// ĳ���� ����, ���� , ���� ��
			if (CharacterMenu::GetInstance()->GetSelect_Elf() || CharacterMenu::GetInstance()->GetSelect_Oak()
				|| CharacterMenu::GetInstance()->GetSelect() || CharacterMenu::GetInstance()->GetRemove()) {
				CharacterMenu::GetInstance()->SetName(KeyBoard::GetInstance()->GetKeyChar()); // ĳ���� �̸� �Է�
			}

			// ������ ����,�Ǹ�,��� ��
			if (GameMenu::GetInstance()->Getbuystate() || GameMenu::GetInstance()->Getsellstate() || GameMenu::GetInstance()->Getonstate()) {
				GameMenu::GetInstance()->SetName(KeyBoard::GetInstance()->GetKeyChar()); // ������ �̸� �Է�
			}
		}
		break;

	case WM_KEYDOWN:
		KeyBoard::GetInstance()->SetKey(wParam);	// wParam �� �Ѱ���
		break;

	case WM_TIMER:		
		InvalidateRect(hWnd, nullptr, false);	// 0.1�ʿ� �� ���� WM_PAINT ȣ���ϸ鼭 ȭ���� �׷��ִ� ���� �� �� �ڿ������� ���ִ� ����
		break;

	case WM_CLOSE:
		FileDataSystem::GetInstance()->DataSave();	// ������ �ı� ���� ���Ͽ� ������ ������ ����
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 0);	// Ÿ�̸� ����
		if (mPthis->mBackbit!= NULL)
		{
			delete mPthis->mBackbit;	// ���Ʈ �޸� ����
		}
		PostQuitMessage(0);	// ������ ���ν��� ����
		return 0;

	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);	
		mPthis->mBackbit->ClearBitMap();	// ȭ���� �� ȭ���� ���� �׷������ν� �ʱ�ȭ���ִ� ��Ȱ

		MainManager::GetInstance()->DrawMenu(mPthis->mBackbit);	// ���� ������ �̿��� ��Ȳ�� �´� �޴��� ����Ѵ�.

		mPthis->mBackbit->SCreanDraw(hdc, 0,0);	// ���Ʈ���� �׷����� DC �� hdc �� ������ش�.
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void CWindowFrame::Run(MSG _msg)
{	
	TranslateMessage(&_msg);
	DispatchMessage(&_msg);	
}

void CWindowFrame::Initialize()
{
	this->BuildWindow();
	ItemStore::GetInstance()->Init();
	FileDataSystem::GetInstance()->DataLoad();	// ������ ���� �Ŀ� �ؽ�Ʈ�� ����� ������ ����Ʈ�� �����Ѵ�.
}

void CWindowFrame::BuildWindow()
{	
	WNDCLASS WndClass;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance = mHinstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=TEXT("exam");
	WndClass.lpszMenuName = nullptr;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	mHWnd = CreateWindow(TEXT("exam"), TEXT("exam"), WS_OVERLAPPEDWINDOW,
		550, 300, 800 , 600,
		NULL,(HMENU)NULL,mHinstance,NULL);

	ShowWindow(mHWnd, SW_SHOW);
}
