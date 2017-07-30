#ifndef ENTITY_H
#define ENTITY_H
#include "Revengine.h"

//Class for any entities that appear on the screen
//If your entity does not move and can be displayed on a block instead of a sprite,
//please using an interaction script instead (World.h)

class Entity
{
	void(*_run)();
	bool(*_interact)(Player *p);
public:
	bool isAlive = true; //entity will be removed once this is false
	bool underground = false;
	bool makeMudslidesWork = false;
	int x, y, width, height; //coordinates and size in pixel coordinates

	//Creates an entity in a given world at the specified coordinates with a certain size and sprite
	//function pointers pass in are used for run and interact methods, for convenience
	//if you don't want to use function pointers, this class will have to be extended
	Entity(World *world, int x, int y, int width, int height, Sprite *sprite, void(*_run)(), bool(*_interact)(Player *p));
	virtual void run();
	virtual bool interact(Player *p); //called when the player "clicks" on the entity, i.e. with talking
	Sprite * sprite = nullptr; //if set to nullptr, no sprite will be diplayed
	virtual Sprite * getSprite();
	World *world = nullptr;
	virtual void save();
	virtual void load();
};

//If an entity needs to run special data when saved and loaded (such as position or item stock if a shopkeeper) use this class instead
class SaveableEntity : public Entity
{
	void(*_save)();
	void(*_load)();
public:
	SaveableEntity(World *world, int x, int y, int width, int height, Sprite *sprite, void(*_run)(), bool(*_interact)(Player *p), void(*_save)(), void(*load)());
	virtual void save();
	virtual void load();
};

class MudslideEntity : public Entity
{
public:
	MudslideEntity(World *world, int x, int y);
	virtual void run();
};

class EagleEntity : public Entity
{
	int pos = 0;
	int numPos = 0;
	int dir;
	int *dirPos;
public:
	EagleEntity(World*world, int x, int y, int numPos, int *dirPos);
	virtual void run();
	virtual Sprite * getSprite();
};

class PushRock : public Entity
{
	int dir = 0;
public:
	PushRock(World *world, int x, int y);
	virtual void run();
	virtual bool interact(Player *p);
};

class PushRockHole : public Entity
{
	int dir = 0;
public:
	PushRockHole(World *world, int x, int y);
	virtual void run();
	virtual bool interact(Player *p);
};

class Wormhole : public Entity
{
public:
	bool otherColor = false;
	int destWorld, destX, destY;
	Wormhole(World *world, int x, int y, int destWorld, int destX, int destY);
	virtual bool interact(Player *p);
	virtual Sprite * getSprite();
};

class BigWormhole :public Entity
{
public:
	bool activated = false;
	BigWormhole(World *world, int x, int y);
	virtual bool interact(Player *p);
	virtual Sprite *getSprite();
};

class WormholeMachine : public Entity
{
public:
	Item *neededItem;
	Wormhole *wormhole;
	int destX, destY, destWorld;
	bool used = false;
	WormholeMachine(World *world, int x, int y, int destWorld, int destX, int destY, Item *neededItem, Wormhole *wormhole);
	virtual bool interact(Player *p);
	virtual Sprite *getSprite();
};

#endif