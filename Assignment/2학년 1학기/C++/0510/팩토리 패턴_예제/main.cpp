#include <iostream>
using namespace std;


// Unit ��ü�� �߻� Ŭ����
// �ڽ� ��ü�� ���� �޾Ƽ� �������ش�.
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

	virtual void Attack() { cout << "���ݷ� : " << Att << endl; }
	virtual void HP() { cout << "ü�� : " << Hp << endl; }
};

class Dragon : public Unit {
private:
	int Att;
	int Hp;
public:
	Dragon() { Att = 20; Hp = 200; }
	virtual~Dragon() { }

	virtual void Attack() { cout << "���ݷ� : " << Att << endl; }
	virtual void HP() { cout << "ü�� : " << Hp << endl; }
};


//-------------------------------------------------------------------

// ���丮 ������ ������ Ŭ����
class Bild {

private:

public:
	Bild() {}
	virtual ~Bild() {}

	virtual Unit* CreateUnit1() = 0;
	virtual Unit* CreateUnit2() = 0;
	// ���� ���� �Լ��� �ڽ� ��ü�� ���� ������ ��ü�� ������ش�.
};


// ���丮 ������ ��� ���� Ŭ����
// ���忡�� ������ ������� �Լ��� ����Ͽ� ��ü�� �����Ѵ�.
class GateWay : public Bild {
private:

public:
	virtual Unit* CreateUnit1() { return new Zealot; } // Zealot �̶�� ��ü�� ����
	virtual Unit* CreateUnit2() { return new Dragon; } // Dragon �̶�� ��ü�� ����
};

// ��Ÿũ����Ʈ�� �ǹ����� ������ �����ϴ� ���� ��Ƽ��� ������ ���Ҵ�.

int main() {
	Bild* bild = new GateWay(); // ��ĳ����

	Unit* Unit1 = bild->CreateUnit1(); // Unit �߻� ��ü�� bild ���� ���� ��ü�� ����
	// ���� ��ü ����
	Unit1->Attack();
	Unit1->HP();

	Unit* Unit2 = bild->CreateUnit2();
	Unit2->Attack();
	Unit2->HP();

	delete bild , Unit1, Unit2;
}