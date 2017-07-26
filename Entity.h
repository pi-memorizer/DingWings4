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
	int x, y, width, height; //coordinates and size in pixel coordinates

	//Creates an entity in a given world at the specified coordinates with a certain size and sprite
	//function pointers pass in are used for run and interact methods, for convenience
	//if you don't want to use function pointers, this class will have to be extended
	Entity(World *world, int x, int y, int width, int height, Sprite *sprite, void(*_run)(), bool(*_interact)(Player *p));
	virtual void run();
	virtual bool interact(Player *p); //called when the player "clicks" on the entity, i.e. with talking
	Sprite * sprite = nullptr; //if set to nullptr, no sprite will be diplayed
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

#endif