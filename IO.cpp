#include "IO.h"
#include "Revengine.h"
#include "Entity.h"
#include "Player.h"
#include "World.h"

void loadPlayer(File * file, Player *p)
{
	p->dir = readByte(file);
	p->setWorldID(readShort(file));
	p->x = readInt(file);
	p->y = readInt(file);
}

void savePlayer(File *file, Player *p)
{
	writeByte(file, (char)p->dir);
	writeShort(file, (short)p->getWorldID());
	writeInt(file, p->x);
	writeInt(file, p->y);
}

void save()
{
	File * file = openFile("save.bin",true);
	if (file == nullptr)
	{
		debug("Unable to write file");
		return;
	}
	for (int i = 0; i < worlds.length(); i++)
	{
		List<Entity*> &entities = worlds[i]->entities;
		for (int j = 0; j < entities.length(); j++)
		{
			entities[j]->save();
		}
	}
	List<string> flagKeys = flags.keys();
	writeInt(file, flagKeys.length());
	for (int i = 0; i < flagKeys.length(); i++)
	{
		writeString(file, flagKeys[i]);
		writeInt(file, flags[flagKeys[i]]);
	}
	Player *p = getPlayer(0);
	assert(p != nullptr);
	savePlayer(file, p);
	closeFile(file);
}

void load()
{
	File * file = openFile("save.bin", false);
	if (file == nullptr)
	{
		debug("Unable to read file");
		return;
	}
	for (int i = 0; i < worlds.length(); i++)
	{
		List<Entity*> &entities = worlds[i]->entities;
		for (int j = 0; j < entities.length(); j++)
		{
			entities[j]->load();
		}
	}
	int numFlags = readInt(file);
	for (int i = 0; i < numFlags; i++)
	{
		flags.add(readString(file), readInt(file));
	}
	Player *p = getPlayer(0);
	assert(p != nullptr);
	loadPlayer(file, p);
	closeFile(file);
}