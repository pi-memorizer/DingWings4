#include "Prison.h"
#include "Player.h"
#include "SoundSystem.h"
#include "World.h"
#include "Blocking.h"
#include "Item.h"

void unlockedScript(Player *p, int x, int y, Script *s)
{
	s->textBox("You pick the lock easily. The lock fades away mysteriously.", false);
	s->textBox("Hey Obi-Worm, you'd better be careful trying to escape.", false);
	s->textBox("You can't dig on rocky ground, and cracked ground might fall out from under you.", false);
	s->textBox("This place is guarded by an eagle you'll need to avoid and some rocks you should be able to push out of the way.", false);
	s->textBox("Their master wormhole back to Worm Planet is up ahead, but you'll need the Amulet of Bender's Fist to activate it.", false);
	s->textBox("Maybe you can find it through other womrholes. Good luck.", false);
}

void startTextScript(Player *p, int x, int y, Script *s)
{
	s->textBox("To interact, press J or Z on a keyboard, or A on a controller.",false);
}

void startWormScript(Player *p, int x, int y, Script *s)
{
	//s->textBox("To interact, press J or Z on a keyboard, or A on a controller.");
}

void prisonEnter(Player*p)
{
}
void prisonRun()
{
	for (int i = 0; i < numPlayers; i++)
	{
		Player *p = players[i];
		if (!p->startText)
		{
			p->startText = true;
			Script::start(p, 0, 0, &startTextScript);
		}
	}
	if (players[0]->getWorldID() == 4) setBackgroundMusic("wormPrison");
}

bool unlocked = false;

bool prisonInteract(Player *p, int x, int y)
{
	if (!unlocked&&x==5&&y==1&&p->inventory[0].item==items["lockpick"])
	{
		unlocked = true;
		World *w = worlds[p->getWorldID()];
		w->setCollision(5, 1, 0);
		w->setUpper(5, 1, 46);
		Script::start(p, 0, 0, &unlockedScript);
	}
	if (x == 1 && y == 3)
	{
		bTextbox(p, "Welcome to Worm Prison. I haven't seen you before; you must have been kidnapped recently. What's your name?");
		bTextbox(p, "Obi-Worm? Good name. Well, Obi-Worm, you should try digging around using K or X on keyboard or B on a controller.");
		bTextbox(p, "Maybe someone left something useful behind.");
	}
	if (x == -5 && y == 5)
	{
		bTextbox(p, "What's your name?");
		bTextbox(p, "Obi-Worm?");
		bTextbox(p, "Obi-Worm, you're my only hope!");
		bTextbox(p, "If you're going to try and escape, clear the way for the rest of us.");
		bTextbox(p, "If you can get that big wormhole working then we'll meet you on the other side.");
	}
	if (x == 13 && y == 3)
	{
		bTextbox(p, "The space eagles have been stealing us space worms from Worm Planet for generations.");
		bTextbox(p, "No one has ever made it back.");
		bTextbox(p, "Maybe you'll be the first?");
		bTextbox(p, "If you're ever unsure of where to go, just dig. It might reveal a new path.");
	}
	return false;
}