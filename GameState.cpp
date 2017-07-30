#include "GameState.h"
#include "Entity.h"
#include "Player.h"
#include "World.h"
#include "Sprite.h"
#include "Event.h"
#include "Animation.h"
#include "Revengine.h"
#include "Blocking.h"

GameState::GameState(Player *player)
{
	p = player;
	a = true;
	b = true;
	c = true;
	up = true;
	down = true;
	left = true;
	right = true;
}

void GameState::startMenu()
{

}

void GameState::endMenu()
{
	a = getKey(p,KEY_A);
	b = getKey(p,KEY_B);
	c = getKey(p, KEY_C);
	up = getKey(p,KEY_UP);
	down = getKey(p,KEY_DOWN);
	left = getKey(p,KEY_LEFT);
	right = getKey(p,KEY_RIGHT);
}

WorldState::WorldState(Player *player) : GameState(player)
{

}

extern Sprite**lifeSprites;

void WorldState::draw()
{
	setDrawColor(0x0, 0x0, 0x0, 0);
	clearScreen();
	for (int i = 0; i < animations.length(); i++)
	{
		if (!animations[i]->isDone()) animations[i]->draw();
	}
	World *world = worlds[p->getWorldID()];
	int alpha = p->underground ? 0 : 255;
	if (p->underground)
	{
		for (int i = p->getCameraCenterX() - WIDTH / 2 - TILE_SIZE * 2; i <= p->getCameraCenterX() + WIDTH / 2 + TILE_SIZE * 2; i += TILE_SIZE)
		{
			for (int j = p->getCameraCenterY() - HEIGHT / 2 - TILE_SIZE * 2; j <= p->getCameraCenterY() + HEIGHT / 2 + TILE_SIZE * 2; j += TILE_SIZE)
			{
				int index = world->getLower(safeDiv(i, TILE_SIZE), safeDiv(j, TILE_SIZE));
				if (tileset[index] != nullptr&&index != 0) tileset[index]->draw(getOnscreenX(p, safeDiv(i, TILE_SIZE)*TILE_SIZE), getOnscreenY(p, safeDiv(j, TILE_SIZE)*TILE_SIZE));
			}
		}
	}
	List<Entity*> &entities = worlds[p->getWorldID()]->entities;
	for (int i = p->getCameraCenterX() - WIDTH / 2 - TILE_SIZE * 2; i <= p->getCameraCenterX() + WIDTH / 2 + TILE_SIZE * 2; i += TILE_SIZE)
	{
		for (int j = p->getCameraCenterY() - HEIGHT / 2 - TILE_SIZE * 2; j <= p->getCameraCenterY() + HEIGHT / 2 + TILE_SIZE * 2; j += TILE_SIZE)
		{
			int index = world->getUpper(safeDiv(i, TILE_SIZE), safeDiv(j, TILE_SIZE));
			if (tileset[index] != nullptr&&index != 0) tileset[index]->draw(getOnscreenX(p, safeDiv(i, TILE_SIZE)*TILE_SIZE), getOnscreenY(p, safeDiv(j, TILE_SIZE)*TILE_SIZE), alpha);
		}
	}
	for (int i = 0; i < entities.length(); i++)
	{
		Entity *e = entities[i];
		assert(e != nullptr);
		int alpha2 = e->underground == p->underground ? 255 : 0;
		if (e->isAlive)
		{
			if (e->getSprite() != nullptr) e->getSprite()->draw(getOnscreenX(p, e->x), getOnscreenY(p, e->y), alpha2);
		}
	}
	for (int i = 0; i < numPlayers; i++)
	{
		int alpha2 = 255;
		if (p->underground != players[i]->underground) alpha2 = 64;
		Sprite * s = players[i]->getSprite();
		if (s!=nullptr&&p->getWorldID() == players[i]->getWorldID()) s->draw(getOnscreenX(p, players[i]->x), getOnscreenY(p, players[i]->y),alpha2);
	}
	for (int i = 0; i < 3; i++)
	{
		if (i < p->lives)
		{
			lifeSprites[0]->draw(16 * i, 0);
		}
		else {
			lifeSprites[1]->draw(16 * i, 0);
		}
	}
}

bool collidesWithEntity(Player *p)
{
	World *w = worlds[p->getWorldID()];
	List<Entity*> &entities = w->entities;
	for (int i = 0; i < entities.length(); i++)
	{
		Entity *e = entities[i];
		if (!e->makeMudslidesWork&&e->underground==p->underground&&rectCollides(p->x, p->y, p->width, p->height, e->x, e->y, e->width, e->height))
			return true;
	}
	return false;
}

void attemptMove(Player *p, int dx, int dy, int speed)
{
	for (int i = 0; i < speed; i++)
	{
		p->x += dx;
		p->y += dy;
		if (p->underground)
		{
			if (worlds[p->getWorldID()]->getLower(safeDiv(p->x+p->width/2,TILE_SIZE), safeDiv(p->y+p->height/2,TILE_SIZE))==21|| worlds[p->getWorldID()]->getLower(safeDiv(p->x + p->width/2, TILE_SIZE), safeDiv(p->y + p->height/2, TILE_SIZE)) == 59)
			{
				p->x -= dx;
				p->y -= dy;
				break;
			}
		} else if (worlds[p->getWorldID()]->collides(p->x, p->y, p->width, p->height)|| collidesWithEntity(p))
		{
			p->x -= dx;
			p->y -= dy;
			break;
		}
	}
}

bool firstWithWorldId(int worldId, int playerId)
{
	for (int i = 0; i < numPlayers; i++)
	{
		if (getPlayer(i)->getWorldID() == worldId)
			return i == playerId;
	}
	return false;
}

void WorldState::run()
{
	for (int i = 0; i < animations.length(); i++)
	{
		if (animations[i]->isDone())
		{
			animations.removeAt(i);
			i--;
		}
		else {
			animations[i]->run();
		}
	}
	startMenu();
	if (this == p->getState())
	{
		if (p->invincibleTime > 0) p->invincibleTime--;
		if (p->lives <= 0)
		{
			debug("Losing?!");
			Script::start(p, 0, 0, &loseGame);
		}
		int dx = 0;
		int dy = 0;
		int _dir = p->dir;
		if (getKey(p,KEY_UP)) dy--;
		if (getKey(p,KEY_DOWN)) dy++;
		if (getKey(p,KEY_LEFT)) dx--;
		if (getKey(p,KEY_RIGHT)) dx++;
		if (p->dir == 0 && dx > 0)
		{
		}
		else if (p->dir == 1 && dy < 0)
		{
		}
		else if (p->dir == 2 && dx < 0)
		{
		}
		else if (p->dir == 3 && dy > 0)
		{
		}
		else {
			if (dx > 0)
				p->dir = 0;
			else if (dy < 0)
				p->dir = 1;
			else if (dx < 0)
				p->dir = 2;
			else if (dy > 0)
				p->dir = 3;
		}
		if (_dir != p->dir) p->wait = 0;
		if ((dx != 0 || dy != 0) && p->wait >= 3) attemptMove(p, dx, dy, 2);
		else if (dx == 0 && dy == 0)
			p->wait = 0;
		p->wait++;
		int sight = 1;
		if (getKey(p,KEY_A) && !a)
		{
			bool mapInteract = false;
			if (p->dir == 0)
			{
				if (worlds[p->getWorldID()]->interact(p, safeDiv(p->x + p->width + sight, TILE_SIZE), safeDiv(p->y + p->height / 2, TILE_SIZE)))
					mapInteract = true;
			}
			if (p->dir == 1)
			{
				if (worlds[p->getWorldID()]->interact(p, safeDiv(p->x + p->width / 2, TILE_SIZE), safeDiv(p->y - sight, TILE_SIZE)))
					mapInteract = true;
			}
			if (p->dir == 2)
			{
				if (worlds[p->getWorldID()]->interact(p, safeDiv(p->x - sight, TILE_SIZE), safeDiv(p->y + p->height / 2, TILE_SIZE)))
					mapInteract = true;
			}
			if (p->dir == 3)
			{
				if (worlds[p->getWorldID()]->interact(p, safeDiv(p->x + p->width / 2, TILE_SIZE), safeDiv(p->y + p->height + sight, TILE_SIZE)))
					mapInteract = true;
			}
			if (!mapInteract)
			{
				//try interacting with entities
				List<Entity*> entities = worlds[p->getWorldID()]->entities;
				int x = 0;
				int y = 0;
				if (p->dir == 0)
				{
					x = p->x + p->width + sight;
					y = p->y + p->height / 2;
				}
				else if (p->dir == 1)
				{
					x = p->x + p->width / 2;
					y = p->y - sight;
				}
				else if (p->dir == 2)
				{
					x = p->x - sight;
					y = p->y + p->height / 2;
				}
				else if (p->dir == 3)
				{
					x = p->x + p->width / 2;
					y = p->y + p->height + sight;
				}
				for (int i = 0; i < entities.length(); i++)
				{
					Entity *e = entities[i];
					if (e->isAlive)
					{
						if (rectCollides(x, y, 2, 2, e->x, e->y, e->width, e->height))
						{
							if (e->interact(p))
							{
								break;
							}
						}
					}
				}
			}
		}
		if (getKey(p, KEY_B) && !b)
		{
			if (p->underground)
			{
				if(!worlds[p->getWorldID()]->collides(p->x, p->y, p->width, p->height))
					p->underground = !p->underground;
			}
			else {
				if (worlds[p->getWorldID()]->getLower(safeDiv(p->x + p->width / 2, 32), safeDiv(p->y + p->height / 2, 32))!=21&& worlds[p->getWorldID()]->getLower(safeDiv(p->x + p->width / 2, 32), safeDiv(p->y + p->height / 2, 32)) != 59)
				{
					p->underground = !p->underground;
				}
			}
		}
	}
	if (firstWithWorldId(p->getWorldID(), p->id))
	{
		worlds[p->getWorldID()]->run();
		List<Entity*> &entities = worlds[p->getWorldID()]->entities;
		for (int i = 0; i < entities.length(); i++)
		{
			if (entities[i]->isAlive)
			{
				entities[i]->run();
			}
			if (!entities[i]->isAlive)
			{
				entities.removeAt(i);
				i--;
			}
		}
	}
	if (worlds[p->getWorldID()]->getLower(safeDiv(p->x+p->width/2, 32), safeDiv(p->y+p->height/2, 32))==18|| worlds[p->getWorldID()]->getLower(safeDiv(p->x + p->width / 2, 32), safeDiv(p->y + p->height / 2, 32)) ==47)
	{
		if (p->invincibleTime == 0)
		{
			p->lives--;
			p->invincibleTime = INVINCIBLE_TIME;
		}
	}
	endMenu();
}

ImageScreen::ImageScreen(Player *p, Sprite *sprite, GameState *nextState) : GameState(p)
{
	this->sprite = sprite;
	this->nextState = nextState;
}
void ImageScreen::draw()
{
	sprite->draw(0, 0);
}
void ImageScreen::run()
{
	startMenu();
	if (getKey(p, KEY_A) && !a)
	{
		Player *player = p;
		GameState *s = nextState;
		if (nextState == nullptr)
		{
			throw ApplicationClosingException();
		}
		player->popState();
		player->pushState(s);
		return;
	}
	endMenu();
}