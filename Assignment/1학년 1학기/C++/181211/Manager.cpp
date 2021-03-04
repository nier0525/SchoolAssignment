#include "Manager.h"

cManager::cManager() {
	for (int i = 0; i < ASIZE; i++) {
		mFruit[i] = new cFruit(); // 객체 생성
	}
}

cManager::~cManager() {
	for (int i = 0; i < ASIZE; i++) {
		delete mFruit[i]; // 해제
	}
}

void cManager::Input() {
	int sel;
	int Mnum;
	int Cnum;
	int index;
	int dex;

	cout << "<제품리스트>" << endl;
	cout << "1. 사과" << endl << "2. 배" << endl << "3. 바나나" << endl
		<< "4. 자몽" << endl << "5. 포도" << endl << "번호 : ";
	cin >> sel; // 번호

	cout << "개수 : ";	cin >> Cnum; // 개수
	dex = Cnum;

	if (mFruit[sel]->GetMoney() == 0) { // 단가 입력이 처음인 경우
		cout << "단가 : ";	cin >> Mnum;
		mFruit[sel]->SetMoney(Mnum);
	}
	index = mFruit[sel]->GetCount(); // index 변수 초기화

	if (Cnum && mFruit[sel]->GetCount() <= 10) { // 개수값이 10 보다 낮으면
		index += Cnum; // 변수값만큼 증가
		mFruit[sel]->SetCount(index); // 증가 된 변수값 객체 함수에 저장
	}

	if (Cnum && mFruit[sel]->GetCount() > 10 ) { // 개수값이 10 보다 크면
		Cnum = mFruit[sel]->GetCount() - (Cnum = 10); // 어거지로 만든 공식..

		index -= Cnum; // 10을 초과하는 정수만큼 감소 = 10
		mFruit[sel]->SetCount(index); // 결과 값 객체 함수에 저장

		dex -= Cnum; // x + y 중 y 값 구하는 공식
		cout << "재고 개수 초과! 더 이상 입고가 불가능합니다." << endl;
	}
	cout << dex << " 개가 입고되었습니다." << endl;
}

void cManager::Output() {
	int sel;
	int Cnum;
	int index;
	int dex;

	cout << "<제품리스트>" << endl;
	cout << "1. 사과" << endl << "2. 배" << endl << "3. 바나나" << endl
		<< "4. 자몽" << endl << "5. 포도" << endl << "번호 : ";
	cin >> sel; // 번호

	cout << "개수 : ";	cin >> Cnum; // 개수
	dex = Cnum;
	index = mFruit[sel]->GetCount(); // 변수 초기화

	index -= Cnum; // 변수값만큼 감소
	mFruit[sel]->SetCount(index); // 결가 값 객체 함수에 저장

	if (Cnum && mFruit[sel]->GetCount() <= 0) { // 0 보다 작으면
		dex = index += Cnum; // 나머지 값 구할 공식
		mFruit[sel]->SetCount(0); // 0 으로 초기화
		cout << "재고가 없습니다! 더 이상 출고가 불가능합니다." << endl;
	}

	cout << dex << " 개가 출고되었습니다." << endl;
}

void cManager::Show() {
	for (int i = 0; i < ASIZE; i++) { // 출력을 위한 for 문
		switch (i) {
		case APPLE:
			if (mFruit[APPLE]->GetCount() != 0) {
				cout << "사과" << endl;
				cout << "개수 : " << mFruit[APPLE]->GetCount() << "\t" << "단가 : " <<
					mFruit[APPLE]->GetMoney() << endl;
			}
			break;
		case BAE:
			if (mFruit[BAE]->GetCount() != 0) {
				cout << "배" << endl;
				cout << "개수 : " << mFruit[BAE]->GetCount() << "\t" << "단가 : " <<
					mFruit[BAE]->GetMoney() << endl;
			}
			break;
		case BANANA:
			if (mFruit[BANANA]->GetCount() != 0) {
				cout << "바나나" << endl;
				cout << "개수 : " << mFruit[BANANA]->GetCount() << "\t" << "단가 : " <<
					mFruit[BANANA]->GetMoney() << endl;
			}
			break;
		case JAMONG:
			if (mFruit[JAMONG]->GetCount() != 0) {
				cout << "자몽" << endl;
				cout << "개수 : " << mFruit[JAMONG]->GetCount() << "\t" << "단가 : " <<
					mFruit[JAMONG]->GetMoney() << endl;
			}
			break;
		case PODO:
			if (mFruit[PODO]->GetCount() != 0) {
				cout << "포도" << endl;
				cout << "개수 : " << mFruit[PODO]->GetCount() << "\t" << "단가 : " <<
					mFruit[PODO]->GetMoney() << endl;
			}
			break;

		} // 까지 출력
	}
}
 
void cManager::ShowMenu() {
	cout << "<제품관리>" << endl;
	cout << "1. 상품입고\t 2. 상품출고\t 3. 상품출력\t 4. 종료 " << endl;
	cout << "선택 : ";
	// 메뉴 출력값
}

void cManager::Run() {
	int sel;
	while (1) {
		ShowMenu();
		cin >> sel;
		switch (sel) {
		case INPUT:
			Input();
			break;
		case OUTPUT:
			Output();
			break;
		case SHOWDATA:
			Show();
			break;
		case EXIT:
			return;
		default:
			cout << "잘못 입력했습니다. 다시 입력하세요." << endl;
		}
	}
}