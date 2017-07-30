#include "Level2.h"
#include "Player.h"
#include "SoundSystem.h"

void level2Enter(Player*p)
{

}

void level2Run()
{
	if (players[0]->getWorldID() == 1)setBackgroundMusic("level2");
}

bool level2Interact(Player *p, int x, int y)
{
	return false;
}