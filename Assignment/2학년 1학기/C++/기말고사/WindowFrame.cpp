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
	FileDataSystem::GetInstance()->DataSave();	// 윈도우 파괴 전에 파일에 유저의 정보를 저장

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

		// 초기화 작업
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
		// 마우스가 클릭 되었을 때 마우스 좌표를 받는다.
		return 0;

	case WM_LBUTTONUP:	
		CMouse::GetInstance()->SetclickUp();
		// 마우스 업
		return 0;

	case WM_CHAR:
		// 문자열 입력
		KeyBoard::GetInstance()->SetKey(wParam);	// wParam 값 넘겨줌
		if (KeyBoard::GetInstance()->GetKeyCode() != VK_RETURN) {	// Enter 키를 눌러도 문자열을 받지 않음
			if (LoginMenu::GetInstance()->GetIdState()) {	 // ID 입력
				LoginMenu::GetInstance()->SetId(KeyBoard::GetInstance()->GetKeyChar());	// 키보드로 입력된 문자열을 넘겨준다.
			}
			if (LoginMenu::GetInstance()->GetPassState()) {		// PASSWORD 입력
				LoginMenu::GetInstance()->SetPass(KeyBoard::GetInstance()->GetKeyChar());	// ''
			}

			// 캐릭터 생성, 삭제 , 선택 시
			if (CharacterMenu::GetInstance()->GetSelect_Elf() || CharacterMenu::GetInstance()->GetSelect_Oak()
				|| CharacterMenu::GetInstance()->GetSelect() || CharacterMenu::GetInstance()->GetRemove()) {
				CharacterMenu::GetInstance()->SetName(KeyBoard::GetInstance()->GetKeyChar()); // 캐릭터 이름 입력
			}

			// 아이템 구입,판매,장비 시
			if (GameMenu::GetInstance()->Getbuystate() || GameMenu::GetInstance()->Getsellstate() || GameMenu::GetInstance()->Getonstate()) {
				GameMenu::GetInstance()->SetName(KeyBoard::GetInstance()->GetKeyChar()); // 아이템 이름 입력
			}
		}
		break;

	case WM_KEYDOWN:
		KeyBoard::GetInstance()->SetKey(wParam);	// wParam 값 넘겨줌
		break;

	case WM_TIMER:		
		InvalidateRect(hWnd, nullptr, false);	// 0.1초에 한 번씩 WM_PAINT 호출하면서 화면을 그려주는 것을 좀 더 자연스럽게 해주는 역할
		break;

	case WM_CLOSE:
		FileDataSystem::GetInstance()->DataSave();	// 윈도우 파괴 전에 파일에 유저의 정보를 저장
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 0);	// 타이머 중지
		if (mPthis->mBackbit!= NULL)
		{
			delete mPthis->mBackbit;	// 백비트 메모리 해제
		}
		PostQuitMessage(0);	// 윈도우 프로시저 종료
		return 0;

	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);	
		mPthis->mBackbit->ClearBitMap();	// 화면을 흰 화면을 새로 그려줌으로써 초기화해주는 역활

		MainManager::GetInstance()->DrawMenu(mPthis->mBackbit);	// 전략 패턴을 이용해 상황에 맞는 메뉴를 출력한다.

		mPthis->mBackbit->SCreanDraw(hdc, 0,0);	// 백비트에서 그려지는 DC 를 hdc 에 출력해준다.
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
	FileDataSystem::GetInstance()->DataLoad();	// 윈도우 시작 후에 텍스트에 저장된 내용을 리스트에 저장한다.
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
