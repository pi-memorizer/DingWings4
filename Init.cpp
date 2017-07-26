#include "Revengine.h"
#include "Blocking.h"
#include "World.h"
#include "Player.h"
#include "Entity.h"
#include "Item.h"
#include "Sprite.h"
#include "IO.h"
#include "SoundSystem.h"
#include "GameState.h"

void testScript(Player*p, int x, int y, Script *s)
{
	s->textBox("Hello there", true);
	s->textBox("I'm a test", true);
	if (s->optionPane("Do you like me?", "Yes", "No") == "Yes")
	{
		s->textBox("Yay!", true);
	}
	else {
		s->textBox("You suck!", true);
	}
}

void nothing() {} //run variant
bool nothing(Player *p, int x, int y) { //interact variant
	if (x == -1 && y == 2)
	{
		Script::start(p, x, y, &testScript);
		return true;
	}
	if (x == -1 && y == 1)
	{
		bTextbox(p,"Hello this a test <s010>that I hope <s100c00FF00>goes<c000000> <w1>well bye", false);
	}
	return false;
}
bool nothing(Player *p) //interact entity variant
{
	return true;
}
void enterNothing(Player *p)
{

}

void init()
{
	createSound("door");

	//load sprites
	Sprite *guySheet = new Sprite("guy", 0, 0);
	guy = new Sprite*[12];
	for (int i = 0; i < 12; i++)
	{
		guy[i] = new Sprite(guySheet, 16 * (i % 4), 24 * (i / 4), 16, 24, 0, -8);
	}
	tileset = new Sprite*[65536];
	for (int i = 0; i < 65536; i++)
	{
		tileset[i] = nullptr;
	}
	for (int i = 0; i < 256; i++)
	{
		try {
			Sprite *s = new Sprite("tileset" + to_string(i + 1), 0, 0);
			for (int j = 0; j < 256; j++)
			{
				tileset[i * 256 + j] = new Sprite(s, TILE_SIZE * (j % 16), TILE_SIZE * (j / 16), TILE_SIZE, TILE_SIZE, 0, 0);
			}
		}
		catch (int) {
		}
	}

	//add entities to worlds and stuff here preferably
	World *test = new StaticWorld("test", &enterNothing,&nothing, &nothing);
	worlds.add(test);
	Entity *testEntity = new Entity(test, -4, 9, 32, 32, nullptr, &nothing, &nothing);
	test->addEntity(testEntity);
	addItem("null")
		->_flags(ITEM_FOOD | ITEM_FURNITURE)
		->_displayName("Nothing");

	load();

	//setBackgroundMusic("PossibleTheme");
}