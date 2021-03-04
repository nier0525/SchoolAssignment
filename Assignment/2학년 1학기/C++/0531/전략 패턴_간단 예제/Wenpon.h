#pragma once

#include "Global.h"

class Wenpon {
private:

public:
	Wenpon() {}
	~Wenpon() {}

	virtual void SetWenpon() = 0;
};

class Axe : public Wenpon {
private:

public:
	Axe() {}
	~Axe() {}

	void SetWenpon() {
		cout << "µµ³¢ ÀåÂø" << endl;
	}
};

class Sword : public Wenpon {
private:

public:
	Sword() {}
	~Sword() {}

	void SetWenpon() {
		cout << "°Ë ÀåÂø" << endl;
	}
};

class Bow : public Wenpon {
private:

public:
	Bow() {}
	~Bow() {}

	void SetWenpon() {
		cout << "È° ÀåÂø" << endl;
	}
};