#include "Revengine.h"
#include "Player.h"
#include "Sprite.h"
#include "Event.h"

#ifdef MOBILE
string to_string(int value);
#endif

int frames = 0;
Hashmap<string, int> flags;
int screenWidth = WIDTH, screenHeight = HEIGHT;

bool theLoop()
{
	Event e;
	while (getEvent(&e))
	{
		if (e.type == EVENT_QUIT)
		{
			return false;
		}
	}

	setDrawColor(0, 0, 0, 0);
	clearScreen();

	int width;
	int height;
	getWindowSize(&width, &height);
	float wRat = width / (float)screenWidth;
	float hRat = height / (float)screenHeight;
	if (numPlayers == 2)
	{
		startDrawingPlayer(0);
		players[0]->draw();
		startDrawingPlayer(1);
		players[1]->draw();
		startDrawingPlayer(-1);

		Rect rect;
		width /= 2;
		wRat /= 2;
		if (wRat < hRat)
		{
			int newHeight = (int)(width / (float)(WIDTH) * (float)HEIGHT);
			rect.h = newHeight;
			rect.w = width;
			rect.x = 0;
			rect.y = (height - newHeight) / 2;
			drawTexture(players[0]->texture, &rect);
			rect.x = width;
			drawTexture(players[1]->texture, &rect);
		}
		else
		{
			int newWidth = (int)(height / (float)HEIGHT * (float)(WIDTH*2));
			rect.w = newWidth;
			rect.h = height;
			rect.x = (width - newWidth) / 2;
			rect.y = 0;
			drawTexture(players[0]->texture, &rect);
			rect.y = height;
			drawTexture(players[1]->texture, &rect);
		}
	}
	else if (numPlayers == 1)
	{
		Rect rect;
		if (wRat < hRat)
		{
			int newHeight = (int)(width / (float)WIDTH * (float)HEIGHT);
			rect.h = newHeight;
			rect.w = width;
			rect.x = 0;
			rect.y = (height - newHeight) / 2;
		}
		else
		{
			int newWidth = (int)(height / (float)HEIGHT * (float)WIDTH);
			rect.w = newWidth;
			rect.h = height;
			rect.x = (width - newWidth) / 2;
			rect.y = 0;
		}
		startDrawingPlayer(0);
		players[0]->draw();
		startDrawingPlayer(-1);
		drawTexture(players[0]->texture, &rect);
	}
	else if (numPlayers <= 4)
	{
		Rect rect;
		if (wRat < hRat)
		{
			int newHeight = (int)(width / (float)WIDTH * (float)HEIGHT);
			rect.h = newHeight;
			rect.w = width;
			rect.x = 0;
			rect.y = (height - newHeight) / 2;
		}
		else
		{
			int newWidth = (int)(height / (float)HEIGHT * (float)WIDTH);
			rect.w = newWidth;
			rect.h = height;
			rect.x = (width - newWidth) / 2;
			rect.y = 0;
		}
		startDrawingPlayer(0);
		players[0]->draw();
		startDrawingPlayer(1);
		players[1]->draw();
		startDrawingPlayer(2);
		players[2]->draw();
		if (numPlayers == 4)
		{
			startDrawingPlayer(3);
			players[3]->draw();
		}
		startDrawingPlayer(-1);
		rect.w /= 2;
		rect.h /= 2;
		Rect rect2 = rect;
		drawTexture(players[0]->texture, &rect2);
		rect2.x = rect.x + rect.w;
		drawTexture(players[1]->texture, &rect2);
		rect2.y = rect.y + rect.h;
		if(numPlayers==4) drawTexture(players[3]->texture, &rect2);
		rect2.x = rect.x;
		drawTexture(players[2]->texture, &rect2);
	}
	display();

	for (int i = 0; i < numPlayers; i++)
	{
		players[i]->run();
	}

	delay(16);
	frames++;
	return true;
}

int getOnscreenX(Player *p, int x)
{
	if (p->cameraLocked)
	{
		return WIDTH / 2 - TILE_SIZE / 2 + (x - p->x);
	}
	else {
		return WIDTH / 2 - TILE_SIZE / 2 + (x - p->cameraX);
	}
}

int getOnscreenY(Player *p, int y)
{
	if (p->cameraLocked)
	{
		return HEIGHT / 2 - TILE_SIZE / 2 + (y - p->y);
	}
	else {
		return HEIGHT / 2 - TILE_SIZE / 2 + (y - p->cameraY);
	}
}

int getFlag(string s)
{
	if (flags.contains(s))
	{
		return flags[s];
	}
	else return 0;
}

void setFlag(string s, int v)
{
	if (flags.contains(s))
	{
		flags[s] = v;
	}
	else {
		flags.add(s, v);
	}
}