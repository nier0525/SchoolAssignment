#pragma once

#include "Wenpon.h"

class Character {
private:
	Wenpon*  m_wenpon;
public:
	Character(Wenpon* _wenpon) {
		m_wenpon = _wenpon;
	}
	
	~Character() {
		delete m_wenpon;
	}

	void GetWenpon() {
		m_wenpon->SetWenpon();
	}

	void ChangeWenpon(Wenpon* _wenpon) {
		if (m_wenpon) {
			delete m_wenpon;
			m_wenpon = nullptr;
		}
		m_wenpon = _wenpon;
	}
};