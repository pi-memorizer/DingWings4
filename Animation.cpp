#include "Animation.h"
#include "Blocking.h"
#include "GameState.h"

Animation::Animation(Player *p) : GameState(p)
{

}

void bAnimation(Player *p, Animation *animation)
{
	p->pushState(animation);
	while (p->getState() == animation)
	{
		if (!theLoop())
		{
			throw ApplicationClosingException();
		}
	}
}

void nbAnimation(Player *p, Animation *animation)
{
	p->getState()->animations.add(animation);
}