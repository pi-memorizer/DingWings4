#include "HH1.h"
#include "Player.h"
#include "SoundSystem.h"
#include "Blocking.h"
#include "Item.h"

bool gotit = false;

void getHammer1(Player *p, int x, int y, Script *s)
{
	if (!gotit)
	{
		s->textBox("You're looking to turn off the eagle's machine? I have just the tool for you.", false);
		s->textBox("Just be careful you don't worm yourself into some trouble now!", false);
		s->textBox("The shark offers you a hammer. You gratefully accept it, storing it in your personal wormhole backpack.", false);
		p->inventory[1].item = items["hammer1"];
		gotit = true;
	}
	else {
		s->textBox("I've given you all I've got. Go forth and wreak havoc!",true);
	}
}

void hh1Enter(Player*p)
{

}
void hh1Run()
{
	if (players[0]->getWorldID() == 2)setBackgroundMusic("wetterGroundThanUsual");
}
bool hh1Interact(Player *p, int x, int y)
{
	if ((x == 16 || x == 17) && y == 4)
	{
		Script::start(p, 0, 0, &getHammer1);
		return true;
	}
	return false;
}