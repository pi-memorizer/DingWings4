#ifndef SPRITE_H
#define SPRITE_H
#include "Revengine.h"
#include "Graphics.h"
using namespace std;

//The class you'll want to use to wrap images that are displayed on the screen
class Sprite
{
	Rect source;
	int xOffset = 0, yOffset = 0;
	bool hasOwnership = true;

public:
	Texture *sprite = nullptr;
	Sprite(string filename, int xOffset, int yOffset); //load from file
	Sprite(Sprite *spritesheet, int x, int y, int width, int height, int xOffset, int yOffset); //load from spreadsheet
	~Sprite();

	void draw(int x, int y);
	void draw(int x, int y, int alpha);
};

extern List<Sprite*> sprites; //list of sprites, for resource clearing purposes

#endif