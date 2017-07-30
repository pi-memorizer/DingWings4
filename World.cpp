#include "World.h"
#include "IO.h"
#include "Entity.h"
#include "Player.h"
#include "Animation.h"
#include "Blocking.h"

List<World*> worlds;
Sprite **tileset = nullptr;

void World::addEntity(Entity *e)
{
	e->world = this;
	entities.add(e);
}

StaticWorld::StaticWorld(string filename, void(*enter)(Player*p),void(*run)(), bool(*interact)(Player*, int, int))
{
	_run = run;
	_interact = interact;
	_enter = enter;
	File *file = openFile(("map/" + filename + ".rmap").c_str(), false);
	if (file != nullptr)
	{
		width = readInt(file);
		height = readInt(file);
		xOffset = readInt(file);
		yOffset = readInt(file);
		map = new StaticBlock[width*height];
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				int i = x+y*width;
				map[i].lower = readUShort(file);
				map[i].upper = readUShort(file);
				map[i].effect = readByte(file);
				map[i].collision = readByte(file);
			}
		}
		closeFile(file);
		debug("Read map " + filename);
		debug(width << ", " << height << ", " << xOffset << ", " << yOffset);
	}
	else {
		debug("Error loading map file " + filename);
		map = nullptr;
		width = 0;
		height = 0;
		xOffset = 0;
		yOffset = 0;
	}
}

void StaticWorld::run()
{
	_run();
}

void StaticWorld::enter(Player *p)
{
	_enter(p);
}

bool StaticWorld::interact(Player *p, int x, int y)
{
	return _interact(p, x, y);
}

bool rectCollides(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	if (y1 + height1 <= y2) return false;
	if (y1 >= y2 + height2) return false;
	if (x1 + width1 <= x2) return false;
	if (x1 >= x2 + width2) return false;
	return true;
}

int safeDiv(int x, int factor)
{
	if (x >= 0) return x / factor;
	//return ((-1 * x) / factor) * -1 - 1;
	return (x+1) / factor - 1;
}

int safeMod(int x, int factor)
{
	if (x >= 0) return x % factor;
	//return ((-1 * x) / factor) * -1 - 1;
	return (x) % factor;
}

bool StaticWorld::subCollision(int x, int y, int px, int py, int pwidth, int pheight)
{
	int c = getCollision(x, y);
	if (c == 0) return false;
	if (c <= 16)
	{
		return rectCollides(px, py, pwidth, pheight, x * TILE_SIZE + TILE_SIZE/2 - c, y * TILE_SIZE + TILE_SIZE/2 - c, 2 * c, 2 * c);
	}
	return false;
}

bool StaticWorld::collides(int x, int y, int width, int height)
{
	assert(width > 0);
	assert(height > 0);
	int _x = x;
	int _y = y;
	for (int cx = safeDiv(_x, TILE_SIZE); cx <= safeDiv(_x + width, TILE_SIZE); cx++)
	{
		for (int cy = safeDiv(_y, TILE_SIZE); cy <= safeDiv(_y + height, TILE_SIZE); cy++)
		{
			if (subCollision(cx, cy, _x, _y, width, height)) return true;
		}
	}
	return false;
}

int StaticWorld::getLower(int x, int y)
{
	if (x >= xOffset&&y >= yOffset&&x < xOffset + width&&y < yOffset + height)
	{
		int v = map[(x - xOffset) + (y - yOffset)*width].lower;
		return v;
	}
	else return 0;
}

void StaticWorld::setLower(int x, int y, int value)
{
	if (x >= xOffset&&y >= yOffset&&x < xOffset + width&&y < yOffset + height)
	{
		map[(x - xOffset) + (y - yOffset)*width].lower = (unsigned short)value;
	}
}

int StaticWorld::getUpper(int x, int y)
{
	if (x >= xOffset&&y >= yOffset&&x < xOffset + width&&y < yOffset + height)
	{
		return map[(x - xOffset) + (y - yOffset)*width].upper;
	}
	else return 0;
}

void StaticWorld::setUpper(int x, int y, int value)
{
	if (x >= xOffset&&y >= yOffset&&x < xOffset + width&&y < yOffset + height)
	{
		map[(x - xOffset) + (y - yOffset)*width].upper = (unsigned short)value;
	}
}

int StaticWorld::getEffect(int x, int y)
{
	if (x >= xOffset&&y >= yOffset&&x < xOffset + width&&y < yOffset + height)
	{
		return map[(x - xOffset) + (y - yOffset)*width].effect;
	}
	else return 0;
}

int StaticWorld::getCollision(int x, int y)
{
	if (x >= xOffset&&y >= yOffset&&x < xOffset + width&&y < yOffset + height)
	{
		return map[(x - xOffset) + (y - yOffset)*width].collision;
	}
	else return 0;
}

void StaticWorld::setCollision(int x, int y, int value)
{
	if (x >= xOffset&&y >= yOffset&&x < xOffset + width&&y < yOffset + height)
	{
		map[(x - xOffset) + (y - yOffset)*width].collision = (unsigned char)value;
	}
}

void teleportScript(Player*p, int x, int y, Script*s)
{
	s->blockingAnimation(new FadeAnimation(p, 255, 255, 255, 20, true));
	p->setWorldID(p->targetWorldID);
	p->x = x * 32 + 6;
	p->y = y * 32 + 6;
	s->blockingAnimation(new FadeAnimation(p, 255, 255, 255, 20, false));
}

void teleport(Player *p, int x, int y, int worldId)
{
	p->targetWorldID = worldId;
	Script::start(p, x, y, &teleportScript);
}