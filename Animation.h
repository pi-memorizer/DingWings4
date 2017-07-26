#ifndef ANIMATION_H
#define ANIMATION_H

#include "Player.h"
#include "GameState.h"

//Animations need to subclass this class
class Animation : public GameState
{
public:
	Animation(Player *p);
	virtual bool isDone() = 0; //returns true if the animation can be removed from the system
	virtual void draw() = 0; //called every frame to draw to the screen
	virtual void run() = 0; //called every frame to do any state calculations
};

void bAnimation(Player *p, Animation *animation); //runs an animation, restricting player movement
void nbAnimation(Player *p, Animation *animation); //runs an animation without restricting player movement

class FadeAnimation : public Animation
{
	int frame, length;
	int r, g, b;
	bool fadeIn;
	GameState *caller;
	int blendMode;
public:
	FadeAnimation(Player *p, int r, int g, int b, int length, bool fadeIn);
	FadeAnimation(Player *p, int r, int g, int b, int length, bool fadeIn, int blendMode);
	virtual bool isDone();
	virtual void draw();
	virtual void run();
};

#endif