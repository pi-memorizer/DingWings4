#include "Entity.h"

Entity::Entity(World *w, int _x, int _y, int _width, int _height, Sprite *s, void(*_run)(), bool(*_interact)(Player *p))
{
	this->_run = _run;
	this->_interact = _interact;
	world = w;
	x = _x;
	y = _y;
	width = _width;
	height = _height;
	sprite = s;
}

void Entity::run()
{
	_run();
}

bool Entity::interact(Player *p)
{
	return _interact(p);
}

void Entity::save()
{

}

void Entity::load()
{

}

SaveableEntity::SaveableEntity(World *world, int x, int y, int width, int height, Sprite *sprite, void(*_run)(), bool(*_interact)(Player *p), void(*_save)(), void(*_load)()) : Entity(world,x,y,width,height,sprite,_run,_interact)
{
	this->_save = _save;
	this->_load = _load;
}

void SaveableEntity::save()
{
	_save();
}

void SaveableEntity::load()
{
	_load();
}