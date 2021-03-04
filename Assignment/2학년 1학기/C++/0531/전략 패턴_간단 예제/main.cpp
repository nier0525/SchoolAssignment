#include "Character.h"

int main() {
	Character* player = new Character(new Axe());

	player->GetWenpon();
	player->ChangeWenpon(new Bow());
	player->GetWenpon();
	player->ChangeWenpon(new Sword());
	player->GetWenpon();

	delete player;
}