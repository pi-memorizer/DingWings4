#include "Entity.h"
#include "GameState.h"
#include "Player.h"
#include "World.h"
#include "Blocking.h"
#include "Item.h"
#include "SoundSystem.h"

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
	if(_run!=nullptr)_run();
}

bool Entity::interact(Player *p)
{
	if (_interact != nullptr)
		return _interact(p);
	else
		return false;
}

Sprite * Entity::getSprite()
{
	return sprite;
}

void Entity::save()
{

}

void Entity::load()
{

}

SaveableEntity::SaveableEntity(World *world, int x, int y, int width, int height, Sprite *sprite, void(*_run)(), bool(*_interact)(Player *p), void(*_save)(), void(*_load)()) : Entity(world, x, y, width, height, sprite, _run, _interact)
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

class MudslideAnimation : public GameState
{
	GameState *caller;
	int x, y;
public:
	MudslideAnimation(Player*p, int x, int y);
	virtual void draw();
	virtual void run();
};

MudslideAnimation::MudslideAnimation(Player *p, int x, int y) : GameState(p)
{
	playSound("mudslide");
	caller = p->getState();
	this->x = x;
	this->y = y;
}

void MudslideAnimation::run()
{
	caller->run();
	if (rectCollides(x, y, TILE_SIZE, TILE_SIZE, p->x, p->y, p->width, p->height))
	{
		p->y += 3;
	}
	else {
		p->crappyFlag = false;
		p->popState();
	}
}

void MudslideAnimation::draw()
{
	caller->draw();
}

MudslideEntity::MudslideEntity(World *world, int x, int y)
	:Entity(world, x, y, TILE_SIZE, TILE_SIZE, nullptr, nullptr, nullptr)
{
	this->x = x;
	this->y = y;
	makeMudslidesWork = true;
}

void MudslideEntity::run()
{
	for (int i = 0; i < numPlayers; i++)
	{
		Player *p = players[i];
		if (!p->crappyFlag&&rectCollides(x, y, TILE_SIZE, TILE_SIZE, p->x, p->y, p->width, p->height))
		{
			p->crappyFlag = true;
			p->pushState(new MudslideAnimation(p, x, y));
		}
	}
}

EagleEntity::EagleEntity(World*world, int x, int y, int numPos, int *dirPos) : Entity(world, x, y, 32, 32, nullptr, nullptr, nullptr)
{
	this->numPos = numPos;
	this->dirPos = dirPos;
	dir = dirPos[0];
	this->x = dirPos[1];
	this->y = dirPos[2];
	pos = 1;
}

void EagleEntity::run()
{
	
	int speed = 4;
	switch (dir)
	{
	case 0:
		x += speed;
		break;
	case 1:
		y -= speed;
		break;
	case 2:
		x -= speed;
		break;
	case 3:
		y += speed;
		break;
	}
	if (dirPos[3 * pos + 1] == x&&dirPos[3 * pos + 2]==y)
	{
		dir = dirPos[3 * pos];
		pos++;
		if (pos == numPos)
		{
			pos = 0;
		}
	}
	for (int i = 0; i < numPlayers; i++)
	{
		Player *p = players[i];
		if (!p->crappyFlag&&!p->underground&&rectCollides(x, y, TILE_SIZE, TILE_SIZE, p->x, p->y, p->width, p->height) && p->invincibleTime == 0 && p->lives > 0)
		{
			p->lives--;
			p->invincibleTime = INVINCIBLE_TIME;
		}
	}
}

extern Sprite **eagleSheet;

Sprite * EagleEntity::getSprite()
{
	return eagleSheet[dir+4*((frames/20)%2)];
}

PushRock::PushRock(World *world, int x, int y) : Entity(world,x*32,y*32,32,32,tileset[28],nullptr,nullptr)
{
}
void PushRock::run()
{
	if (safeMod(x, 32) != 0 || safeMod(y, 32) != 0)
	{
		int speed = 1;
		switch (dir)
		{
		case 0:
			x += speed;
			break;
		case 1:
			y -= speed;
			break;
		case 2:
			x -= speed;
			break;
		case 3:
			y += speed;
			break;
		}
	}
}
bool PushRock::interact(Player *p)
{
	if (safeMod(x, 32) != 0 || safeMod(y, 32) != 0) return false;
	dir = p->dir;
	int dx = 0;
	int dy = 0;
	int speed = 1;
	switch (dir)
	{
	case 0:
		dx = speed;
		break;
	case 1:
		dy = -speed;
		break;
	case 2:
		dx = -speed;
		break;
	case 3:
		dy = speed;
		break;
	}
	if (world->collides(x + TILE_SIZE*dx, y + TILE_SIZE*dy, 32, 32))
	{
		return false;
	}
	List<Entity*> entities = world->entities;
	for (int i = 0; i < entities.length(); i++)
	{
		Entity * e = entities[i];
		if (rectCollides(x+TILE_SIZE*dx, y+TILE_SIZE*dy, width, height, e->x, e->y, e->width, e->height)) return false;
	}
	x += dx;
	y += dy;
	playSound("rock scrape");
}

PushRockHole::PushRockHole(World *world, int x, int y) : Entity(world, x * 32, y * 32, 32, 32, tileset[28], nullptr, nullptr)
{
}
void PushRockHole::run()
{
	if (safeMod(x, 32) != 0 || safeMod(y, 32) != 0)
	{
		int speed = 1;
		switch (dir)
		{
		case 0:
			x += speed;
			break;
		case 1:
			y -= speed;
			break;
		case 2:
			x -= speed;
			break;
		case 3:
			y += speed;
			break;
		}
	}
	else if (world->getUpper(safeDiv(x, 32), safeDiv(y, 32)) == 19)
	{
		isAlive = false;
		world->setUpper(safeDiv(x, 32), safeDiv(y, 32), 31);
		world->setLower(safeDiv(x, 32), safeDiv(y, 32), 21);
		world->setCollision(safeDiv(x, 32), safeDiv(y, 32), 0);
	}
}
bool PushRockHole::interact(Player *p)
{
	if (safeMod(x, 32) != 0 || safeMod(y, 32) != 0) return false;
	dir = p->dir;
	int dx = 0;
	int dy = 0;
	int speed = 1;
	switch (dir)
	{
	case 0:
		dx = speed;
		break;
	case 1:
		dy = -speed;
		break;
	case 2:
		dx = -speed;
		break;
	case 3:
		dy = speed;
		break;
	}
	if (world->collides(x + TILE_SIZE*dx, y + TILE_SIZE*dy, 32, 32))
	{
		return false;
	}
	List<Entity*> entities = world->entities;
	for (int i = 0; i < entities.length(); i++)
	{
		Entity * e = entities[i];
		if (rectCollides(x + TILE_SIZE*dx, y + TILE_SIZE*dy, width, height, e->x, e->y, e->width, e->height)) return false;
	}
	x += dx;
	y += dy;
	playSound("rock scrape");
}

Wormhole::Wormhole(World *world, int x, int y, int destWorld, int destX, int destY) : Entity(world, x, y, 32, 32, nullptr, nullptr, nullptr)
{
	this->destWorld = destWorld;
	this->destX = destX;
	this->destY = destY;
}

bool Wormhole::interact(Player *p)
{
	teleport(p, destX, destY, destWorld);
	playSound("wormhole");
	return true;
}

extern Sprite **wormholeSprites;

Sprite * Wormhole::getSprite()
{
	return wormholeSprites[(frames / 10) % 4 + (otherColor?4:0)];
}


BigWormhole *bw;

BigWormhole::BigWormhole(World *world, int x, int y) : Entity(world,x,y,64,64,nullptr,nullptr,nullptr)
{
	bw = this;
}

extern Sprite * winScreen;
void bigWormholeScript(Player *p, int x, int y, Script *s)
{
	if (!bw->activated)
	{
		bool item1 = false, item2 = false;
		for (int i = 0; i < numPlayers; i++)
		{
			Player *p2 = players[i];
			if (p2->inventory[2].item == items["item1"]) item1 = true;
			if (p2->inventory[4].item == items["item2"]) item2 = true;
		}
		if (item1&&item2)
		{
			bw->activated = true;
			s->textBox("You combine the two necklaces to form a 'Best Friends' heart. The wormhole in front of you blazes to life.", false);
			//s->textBox("You win!", false);
			//throw ApplicationClosingException();
		}
		else {
			s->textBox("This seems to be a wormhole leading to Worm Planet. It looks like it needs something to activate.", true);
		}
	}
	else {
		for (int i = 0; i < numPlayers; i++)
		{
			ImageScreen *s = new ImageScreen(players[i], winScreen, nullptr);
			players[i]->pushState(s);
			bTextbox(players[i], "You win!");
		}
	}
}

bool BigWormhole::interact(Player *p)
{
	Script::start(p, 0, 0, &bigWormholeScript);
	return true;
}
extern Sprite **bigWormholeSprites;

Sprite *BigWormhole::getSprite()
{
	if (activated)
	{
		return bigWormholeSprites[(frames / 20) % 4];
	}
	else return bigWormholeSprites[4];
}

WormholeMachine::WormholeMachine(World *world, int x, int y, int destWorld, int destX, int destY, Item *neededItem, Wormhole *wormhole) : Entity(world, x, y, 32, 32, nullptr, nullptr, nullptr)
{
	this->destWorld = destWorld;
	this->destX = destX;
	this->destY = destY;
	this->neededItem = neededItem;
	this->wormhole = wormhole;
}

bool WormholeMachine::interact(Player *p)
{
	p->machine = this;
	Script::start(p, x, y, &useItemScript);
	return true;
}

Sprite *WormholeMachine::getSprite()
{
	if (used)
	{
		return tileset[30];
	}
	else
		return tileset[29];
}