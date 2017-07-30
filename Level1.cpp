#include "Level1.h"
#include "Player.h"
#include "SoundSystem.h"

void level1Enter(Player*p)
{
}

void level1Run()
{
	if (players[0]->getWorldID() == 0)setBackgroundMusic("level1");
}

bool level1Interact(Player *p, int x, int y)
{
	return false;
}