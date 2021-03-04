#include <iostream>
using namespace std;


// Unit 객체의 추상 클래스
// 자식 객체의 값을 받아서 생성해준다.
class Unit {
private:

public:
	Unit() {}
	virtual ~Unit() {}

	virtual void Attack() = 0;
	virtual void HP() = 0;
};

class Zealot : public Unit {
private:
	int Att;
	int Hp;
public:
	Zealot() { Att = 16; Hp = 160; }
	virtual ~Zealot() {}

	virtual void Attack() { cout << "공격력 : " << Att << endl; }
	virtual void HP() { cout << "체력 : " << Hp << endl; }
};

class Dragon : public Unit {
private:
	int Att;
	int Hp;
public:
	Dragon() { Att = 20; Hp = 200; }
	virtual~Dragon() { }

	virtual void Attack() { cout << "공격력 : " << Att << endl; }
	virtual void HP() { cout << "체력 : " << Hp << endl; }
};


//-------------------------------------------------------------------

// 팩토리 패턴을 구현할 클래스
class Bild {

private:

public:
	Bild() {}
	virtual ~Bild() {}

	virtual Unit* CreateUnit1() = 0;
	virtual Unit* CreateUnit2() = 0;
	// 순수 가상 함수로 자식 객체에 따른 값으로 객체를 만들어준다.
};


// 팩토리 패턴을 상속 받은 클래스
// 공장에서 물건을 만들듯이 함수를 사용하여 객체를 생성한다.
class GateWay : public Bild {
private:

public:
	virtual Unit* CreateUnit1() { return new Zealot; } // Zealot 이라는 객체를 생성
	virtual Unit* CreateUnit2() { return new Dragon; } // Dragon 이라는 객체를 생성
};

// 스타크래프트의 건물에서 유닛을 생성하는 것을 모티브로 구현해 보았다.

int main() {
	Bild* bild = new GateWay(); // 업캐스팅

	Unit* Unit1 = bild->CreateUnit1(); // Unit 추상 객체에 bild 에서 만들어낸 객체를 생성
	// 만들어낸 객체 정보
	Unit1->Attack();
	Unit1->HP();

	Unit* Unit2 = bild->CreateUnit2();
	Unit2->Attack();
	Unit2->HP();

	delete bild , Unit1, Unit2;
}