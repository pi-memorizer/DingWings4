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

FadeAnimation::FadeAnimation(Player *p, int r, int g, int b, int length, bool fadeIn) : Animation(p)
{
	frame = frames;
	this->r = r;
	this->g = g;
	this->b = b;
	this->fadeIn = fadeIn;
	this->length = length;
	caller = p->getState();
	blendMode = BLENDMODE_BLEND;
}

FadeAnimation::FadeAnimation(Player *p, int r, int g, int b, int length, bool fadeIn, int blendMode) : FadeAnimation(p, r, g, b, length, fadeIn)
{
	this->blendMode = blendMode;
}

bool FadeAnimation::isDone()
{
	return frames - frame >= length;
}
void FadeAnimation::draw()
{
	caller->draw();
	int a = (frames - frame)*255 / length;
	if (a > 255) a = 255;
	if (!fadeIn) a = 255 - a;
	setDrawColor(r, g, b,a);
	Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = WIDTH;
	rect.h = HEIGHT;
	fillRect(&rect,blendMode);
}
void FadeAnimation::run()
{
	if (isDone())
	{
		GameState *c = caller;
		p->popState();
		c->run();
	} else caller->run();
}