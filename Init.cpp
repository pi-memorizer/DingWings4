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
#include "Animation.h"
#include "Level1.h"
#include "Level2.h"
#include "HH1.h"
#include "HH2.h"
#include "Prison.h"

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
	s->blockingAnimation(new FadeAnimation(p, 255, 0, 0, 120, true));
	s->blockingAnimation(new FadeAnimation(p, 255, 0, 0, 120, false));
}

void useItemScript(Player *p, int x, int y, Script *s)
{
	if (p->machine->used)
	{
		s->textBox("Well, that's one way to turn it off!",true);
		return;
	}
	s->textBox("This seems to be controlling the wormhole to Hammerhead Planet.", false);
	s->textBox("You don't know how to turn it off. Maybe there's someone around who does?", true);
	if (s->optionPane("Use an item?", "Yes", "No") == "Yes")
	{
		Item *i = s->selectItem();
		if (p->machine->neededItem == i)
		{
			s->blockingAnimation(new FadeAnimation(p, 255, 255, 128, 60, true, BLENDMODE_ADD));
			Wormhole *w = p->machine->wormhole;
			w->destWorld = p->machine->destWorld;
			w->destX = p->machine->destX;
			w->destY = p->machine->destY;
			w->otherColor = true;
			p->machine->used = true;
			playSound("break");
			s->blockingAnimation(new FadeAnimation(p, 255, 255, 128, 60, false, BLENDMODE_ADD));
		}
		else {
			s->textBox("That doesn't seem to work...",true);
		}
	}
}

void nothing() {} //run variant
bool nothing(Player *p, int x, int y) { //interact variant
	if (x == -1 && y == 2)
	{
		Script::start(p, 0, 0, &testScript);
		//teleport(p, -2, 1, 0);
		return true;
	}
	if (x == -1 && y == 1&&!p->underground)
	{
		//bTextbox(p,"<j51>Hello this a test <j00s010>that I <f1>hope<f0> <s100r99>goes<r00> <w1>well bye", false);
		if (p->invincibleTime == 0)
		{
			p->lives--;
			p->invincibleTime = INVINCIBLE_TIME;
		}
	}
	return false;
}
bool nothing(Player *p) //interact entity variant
{
	return true;
}
void enterNothing(Player *p)
{
	setBackgroundMusic("wetterGroundThanUsual");
}

bool getLockpick(Player *p)
{
	worlds[4]->entities[0]->isAlive = false;
	p->inventory[0].item = items["lockpick"];
	p->inventory[0].number = 1;
	bTextbox(p, "You find a lockpick. Good thing you took that skullduggery class in high school.");
	return true;
}

bool getItem1(Player *p)
{
	worlds[0]->entities[24]->isAlive = false;
	p->inventory[2].item = items["item1"];
	bTextbox(p, "You find an odd shaped pendant inscribed with the world 'Best.' You wonder if that means you're the best.");
	return true;
}

bool getItem2(Player *p)
{
	worlds[1]->entities[14]->isAlive = false;
	p->inventory[4].item = items["item2"];
	bTextbox(p,"You find a strange necklace boasting the word 'Friends.' You sure wish you had some of those. Maybe this will help you get them?");
	return true;
}

Sprite **eagleSheet;
Sprite **lifeSprites;
Sprite **wormholeSprites;
Sprite **bigWormholeSprites;
Sprite *startScreen;
Sprite *winScreen;

bool resourcesInit = false;

void init()
{
	if (!resourcesInit)
	{
		createSound("break");
		createSound("mudslide");
		createSound("rock scrape");
		createSound("wormhole");
		//load sprites
		startScreen = new Sprite("titlescreen",0,0);
		winScreen = new Sprite("Planet Worm", 0, 0);
		Sprite *guySheet = new Sprite("worm", 0, 0);
		guy = new Sprite*[12];
		for (int i = 0; i < 12; i++)
		{
			guy[i] = new Sprite(guySheet, 32 * (i % 4), 48 * (i / 4), 32, 48, 0, -38);
		}
		guy[0]->xOffset = -1;
		guy[1]->xOffset = -1;
		guy[2]->xOffset = -6;
		guy[3]->xOffset = -1;
		guy[4]->xOffset = -1;
		guy[5]->xOffset = -6;
		guy[6]->xOffset = -6;
		guy[7]->xOffset = -6;
		guy[9]->xOffset = -1;
		guy[11]->xOffset = -1;
		for (int i = 0; i < 12; i++)
		{
			guy[i]->xOffset -= 5;
		}
		Sprite *eagleSpritesheet = new Sprite("eagle spritemap", 0, 0);
		eagleSheet = new Sprite*[12];
		for (int i = 0; i < 12; i++)
		{
			eagleSheet[i] = new Sprite(eagleSpritesheet, 32 * (i % 4), 32 * (i / 4), 32, 32, 0, 0);
		}
		Sprite *lifeSheet = new Sprite("lives", 0, 0);
		lifeSprites = new Sprite*[2];
		lifeSprites[0] = new Sprite(lifeSheet, 0, 0, 16, 16, 0, 0);
		lifeSprites[1] = new Sprite(lifeSheet, 16, 0, 16, 16, 0, 0);
		wormholeSprites = new Sprite*[8];
		Sprite *wormholeSheet = new Sprite("wormhole", 0, 0);
		wormholeSprites[0] = new Sprite(wormholeSheet, 0, 0, 32, 32, 0, 0);
		wormholeSprites[1] = new Sprite(wormholeSheet, 32, 0, 32, 32, 0, 0);
		wormholeSprites[2] = new Sprite(wormholeSheet, 0, 32, 32, 32, 0, 0);
		wormholeSprites[3] = new Sprite(wormholeSheet, 32, 32, 32, 32, 0, 0);
		wormholeSprites[4] = new Sprite(wormholeSheet, 0, 64, 32, 32, 0, 0);
		wormholeSprites[5] = new Sprite(wormholeSheet, 32, 64, 32, 32, 0, 0);
		wormholeSprites[6] = new Sprite(wormholeSheet, 0, 32+64, 32, 32, 0, 0);
		wormholeSprites[7] = new Sprite(wormholeSheet, 32, 32+64, 32, 32, 0, 0);
		Sprite *bigWormholeSheet = new Sprite("bigwormhole", 0, 0);
		bigWormholeSprites = new Sprite*[5];
		bigWormholeSprites[0] = new Sprite(bigWormholeSheet, 0, 0, 64, 64, 0, 0);
		bigWormholeSprites[1] = new Sprite(bigWormholeSheet, 64, 0, 64, 64, 0, 0);
		bigWormholeSprites[2] = new Sprite(bigWormholeSheet, 0, 64, 64, 64, 0, 0);
		bigWormholeSprites[3] = new Sprite(bigWormholeSheet, 64, 64, 64, 64, 0, 0);
		bigWormholeSprites[4] = new Sprite("greywormhole",0,0);

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
		resourcesInit = true;
		addItem("null")
			->_flags(ITEM_FOOD | ITEM_FURNITURE)
			->_displayName("Nothing");
		addItem("lockpick")->_sprite(tileset[17]);
		addItem("hammer1")->_sprite(tileset[35]);
		addItem("item1")->_sprite(tileset[33]);
		addItem("hammer2")->_sprite(tileset[36]);
		addItem("item2")->_sprite(tileset[34]);
	}
	else {
		while (worlds.length() > 0)
		{
			delete worlds.removeAt(0);
		}
		debug("No more worlds");
	}

	World *level1 = new StaticWorld("level1", &level1Enter, &level1Run, &level1Interact);
	Wormhole *wormhole1 = new Wormhole(level1, 32 * 13, 32 * 10, 2, 17, 16);
	level1->addEntity(new PushRock(level1, 11, -7));
	level1->addEntity(new PushRock(level1, 12, -8));
	level1->addEntity(new PushRock(level1, 13, -8));
	level1->addEntity(new PushRock(level1, 13, -9));
	level1->addEntity(new PushRock(level1, 15, -9));
	level1->addEntity(new PushRock(level1, 16, -8));
	level1->addEntity(new PushRock(level1, 12, -6));
	level1->addEntity(new PushRock(level1, 9, -4));
	level1->addEntity(new PushRock(level1, 17, -6));
	level1->addEntity(new PushRock(level1, 18, -5));
	level1->addEntity(new PushRock(level1, 19, -6));
	level1->addEntity(new PushRock(level1, 13, -5));
	level1->addEntity(new PushRock(level1, 14, -5));
	level1->addEntity(new PushRock(level1, 12, -4));
	level1->addEntity(new PushRock(level1, 13, -4));
	level1->addEntity(new PushRock(level1, 14, -4));
	level1->addEntity(new PushRock(level1, 12, -3));
	int*eagle1Path = new int[6] {0,9*32,5*32,2,32 * 24,32 * 5};
	level1->addEntity(new EagleEntity(level1, 32 * 24, 32 * 5, 2, eagle1Path));
	int *eagle4Path = new int[6]{ 0,32 * 16,32 * -2,2,32 * 20,32 * -2 };
	level1->addEntity(new EagleEntity(level1, 0, 0, 2, eagle4Path));
	level1->addEntity(new MudslideEntity(level1, 32 * 34, 32 * 20));
	level1->addEntity(new MudslideEntity(level1, 32 * 36, 32 * 2));
	level1->addEntity(new Wormhole(level1, 32*35, 32*23, 4, -5, -28));
	level1->addEntity(wormhole1);
	level1->addEntity(new WormholeMachine(level1, 22 * 32, 32 * 16, 0, 11, 0, items["hammer1"],wormhole1));
	level1->addEntity(new Entity(level1, 34 * 32, 32 * 7, 32, 32, tileset[33], nullptr,  &getItem1));
	Wormhole *wormhole2 = new Wormhole(level1, 32 * 11, 32 * 1, 0, 14, 10);
	wormhole2->otherColor = true;
	level1->addEntity(wormhole2);
	worlds.add(level1);

	World *level2 = new StaticWorld("level2", &level2Enter, &level2Run, &level2Interact);
	level2->addEntity(new PushRockHole(level2, 10, 5));
	level2->addEntity(new PushRockHole(level2, 12, 6));
	level2->addEntity(new PushRockHole(level2, 13, 8));
	level2->addEntity(new PushRockHole(level2, 11, 8));
	level2->addEntity(new PushRockHole(level2, 10, 7));
	level2->addEntity(new PushRockHole(level2, 32, 8));
	level2->addEntity(new PushRockHole(level2, 29, 7));
	level2->addEntity(new PushRockHole(level2, 28, 9));
	int *eagle2Path = new int[3 * 4]{0,26*32,11*32,3,36*32,11*32,2,36*32,18*32,1,26 * 32,18 * 32 };
	level2->addEntity(new EagleEntity(level2, 32 * 26, 32 * 11, 4, eagle2Path));
	int *eagle3Path = new int[3 * 4]{ 0,28*32,12*32,3,34*32,12*32,2,34*32,17*32,1,28*32,17*32 };
	level2->addEntity(new EagleEntity(level2, 32 * 28, 32 * 12, 4, eagle3Path));
	Wormhole *wormhole3 = new Wormhole(level2, 32 * 22, 32 * 11, 3, 16, 20);
	level2->addEntity(new Wormhole(level2, 32 * 6, 32 * 5, 4, 8, -28));
	level2->addEntity(wormhole3);
	level2->addEntity(new WormholeMachine(level2, 32 * 8, 32 * 22, 1, 33, 20, items["hammer2"], wormhole3));
	Wormhole *wormhole4 = new Wormhole(level2, 32 * 33, 32 * 21, 1, 23, 11);
	wormhole4->otherColor = true;
	level2->addEntity(wormhole4);
	level2->addEntity(new Entity(level2, 32 * 31, 32 * 14, 32, 32, tileset[34], nullptr, &getItem2));
	worlds.add(level2);

	World *hh1 = new StaticWorld("hh1", &hh1Enter, &hh1Run, &hh1Interact);
	hh1->addEntity(new Wormhole(hh1, 32 * 18, 32 * 16, 0, 14, 10));
	worlds.add(hh1);

	World *hh2 = new StaticWorld("hh2", &hh2Enter, &hh2Run, &hh2Interact);
	hh2->addEntity(new Wormhole(hh2, 32 * 16, 32 * 21, 1, 23, 11));
	worlds.add(hh2);

	World *prison = new StaticWorld("prison", &prisonEnter, &prisonRun, &prisonInteract);
	prison->addEntity(new Entity(prison, 32 * 9, 32 * 3, 32, 32, tileset[17], nullptr, &getLockpick));
	prison->entities[0]->underground = true;
	prison->addEntity(new Wormhole(prison, 32 * 9, 32 * -28, 1, 7, 5));
	prison->addEntity(new Wormhole(prison, 32 * -6, 32 * -28, 0, 34, 23));
	prison->addEntity(new MudslideEntity(prison, 32 * 1, 32 * -10));
	int *eagle5Path = new int[6] {0, 32 * -11, 32 * -2, 2, 32 * 15, 32 * -2};
	prison->addEntity(new EagleEntity(prison, 0, 0, 2, eagle5Path));
	prison->addEntity(new BigWormhole(prison, 32 * 1, 32 * -33));
	for (int x = -3; x <= 6; x++)
		prison->addEntity(new PushRock(prison, x, -18));
	worlds.add(prison);

	//load();

	//setBackgroundMusic("PossibleTheme");
}

void loseGame(Player*p, int x, int y, Script *s)
{
	setBackgroundMusic("lose");
	p->invincibleTime = 0;
	if (s->optionPane("You lose! Continue?", "Yes", "No") == "Yes")
	{
		reset();
	}
	else {
		throw ApplicationClosingException();
	}
	p->lives = 3;
	p->setWorldID(p->getWorldID());
}