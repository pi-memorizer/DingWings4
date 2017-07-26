#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Revengine.h"

class Animation;

//GameState superclass for the game state stack
class GameState
{
protected:
	Player *p;
	bool a, b, up, down, left, right; //what the buttons were last frame
public:
	List<Animation*> animations; //any animations associated with the GameState
	GameState(Player *player); //game states are always bound to a specific player
	virtual void draw() = 0; //called every frame to draw to the screen, has potential to be skipped
	virtual void run() = 0; //called every frame to run state logic (i.e. UI interactions)
	void startMenu(); //if you want to use the last frame's buttons, call this at the start of your run() function
	void endMenu(); //call this at the end of your run function if you want to use last frame's buttons
};

//GameState that runs the overworld
class WorldState : public GameState
{
public:
	WorldState(Player *player);
	virtual void draw();
	virtual void run();
};

#endif