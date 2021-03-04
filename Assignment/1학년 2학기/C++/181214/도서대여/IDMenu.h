#ifndef IDMENU_H
#define IDMENU_H
#include "Base.h"

class cIDMenu {
private:
	char mId[ID];
	char mPass[PASSWORD];
public:
	cIDMenu(const char*, const char*);
	~cIDMenu();

	void SetId(const char*);
	void SetPass(const char*);

	const char* GetId();
	const char* GetPass();
};

#endif