#include "Sprite.h"
#include "Graphics.h"

Sprite::Sprite(string filename, int xOffset, int yOffset)
{
	sprite = loadTexture(filename);
	if (sprite != nullptr)
	{
		getTextureSize(sprite, &source.w, &source.h);
		source.x = 0;
		source.y = 0;
		this->xOffset = xOffset;
		this->yOffset = yOffset;
		this->hasOwnership = true;
		sprites.add(this);
	}
	else {
		throw - 1;
	}
}

Sprite::Sprite(Sprite *spritesheet, int x, int y, int width, int height, int xOffset, int yOffset)
{
	sprite = spritesheet->sprite;
	this->hasOwnership = false;
	source.h = height;
	source.w = width;
	source.x = x;
	source.y = y;
	this->xOffset = xOffset;
	this->yOffset = yOffset;
	sprites.add(this);
}

Sprite::~Sprite()
{
	if (hasOwnership)
	{
		destroyTexture(sprite);
	}
}

void Sprite::draw(int x, int y)
{
	Rect d;
	if (this == nullptr)
	{
		cout << "wat" << endl;
		return;
	}
	d.h = source.h;
	d.w = source.w;
	d.x = x + xOffset;
	d.y = y + yOffset;
	drawTexture(sprite, &source, &d);
}

void Sprite::draw(int x, int y, int alpha)
{
	Rect d;
	if (this == nullptr)
	{
		cout << "wat" << endl;
		return;
	}
	d.h = source.h;
	d.w = source.w;
	d.x = x + xOffset;
	d.y = y + yOffset;
	drawTexture(sprite, &source, &d, alpha);
}

List<Sprite*> sprites;