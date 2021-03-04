#include "Food.h"

CFood::CFood(const char* _name, const char* _makeday,const char* _mid, int _price) 
: CProduct(_name, _makeday,_mid, _price) {
	memset(gram, 0, sizeof(gram));
}

CFood::~CFood() {

}

void CFood::Setgram(const char* _gram) {
	strcpy(gram, _gram);
}

const char* CFood::Getgram() {
	return gram;
}