#include "HH2.h"
#include "Player.h"
#include "SoundSystem.h"
#include "Item.h"
#include "Blocking.h"

bool gotit2 = false;

void getHammer2(Player *p, int x, int y, Script *s)
{
	if (!gotit2)
	{
		s->textBox("Need some way to disrupt the eagle's wormhole machine? I can help with that.", false);
		s->textBox("Take this. You'll know what to do with it.", false);
		s->textBox("You take the hammer placed in front of you. You definitely know what to do with it.", false);
		p->inventory[3].item = items["hammer2"];
		gotit2 = true;
	}
	else {
		s->textBox("Don't thank me for that hammer. You're helping me as much as I'm helping you. We sharks <cFF0000>will<c000000> have our revenge.", true);
	}
}

void hh2Enter(Player*p)
{

}
void hh2Run()
{
	if(players[0]->getWorldID()==3) setBackgroundMusic("wetterGroundThanUsual");
}
bool hh2Interact(Player *p, int x, int y)
{
	if ((x == 7 || x == 8) && y == -3)
	{
		Script::start(p, 0, 0, &getHammer2);
		return true;
	}
	return false;
}