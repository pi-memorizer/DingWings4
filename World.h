#ifndef WORLD_H
#define WORLD_H
#include "Revengine.h"

extern Sprite **tileset; //array of size 65536 that holds all of the tiles

//Generic world class, currently only extended by StaticWorld, but could be extended for more dynanmic worlds
class World
{
public:
	List<Entity*> entities; //list of entities in the world
	void virtual run() = 0; //run every frame, good for testing if players step on things
	void virtual enter(Player *p) = 0;
	bool virtual interact(Player *p, int x, int y) = 0;
	bool virtual collides(int x, int y, int width, int height) = 0;
	int virtual getLower(int x, int y) = 0;//lower map layer
	int virtual getUpper(int x, int y) = 0;//upper map layer
	int virtual getEffect(int x, int y) = 0; //effect layer
	int virtual getCollision(int x, int y) = 0; //collision layer
	void addEntity(Entity *e);
};

//class that holds the data for individual spaces in StaticWorld
struct StaticBlock
{
	unsigned short lower, upper;
	unsigned char effect, collision;

	StaticBlock(int _lower, int _upper, int _effect, int _collision)
	{
		lower = (unsigned short)_lower;
		upper = (unsigned short)_upper;
		effect = (unsigned char)_effect;
		collision = (unsigned char)_collision;
	}
	StaticBlock() {}
};

//Worlds that do not change (much) and can be loaded from a file
class StaticWorld : public World
{
private:
	void(*_run)();
	bool(*_interact)(Player*,int,int);
	void(*_enter)(Player *p);
	int width, height, xOffset, yOffset;
	StaticBlock *map = nullptr;
	bool subCollision(int x, int y, int px, int py, int pwidth, int pheight);
public:
	StaticWorld(string filename, void (*enter)(Player *p), void (*run)(), bool(*interact)(Player*,int,int));
	
	void run();
	void enter(Player *p);
	bool interact(Player *p, int x, int y);
	bool virtual collides(int x, int y, int width, int height);
	int virtual getLower(int x, int y);
	int virtual getUpper(int x, int y);
	int virtual getEffect(int x, int y);
	int virtual getCollision(int x, int y);
};

#endif