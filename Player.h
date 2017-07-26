#ifndef PLAYER_H
#define PLAYER_H
#include "Revengine.h"
#include "Graphics.h"

//Anything dealing with individual players is here

#define INVENTORY_SLOTS 3 //the max amount of inventory slots a player can have

extern Sprite ** guy; //the current spritesheet for the main character

struct ItemNumberPair //holds items with their quantities, for use mainly in inventories
{
	Item *item;
	int number;
};

//class that holds a player object
class Player
{
	Stack<GameState*> states;
	int worldID;
	bool cameraLocked = true;
public:
	friend int getOnscreenX(Player *p, int x); //allowed to access private data
	friend int getOnscreenY(Player *p, int y); //allowed to access private data 
	int cameraX, cameraY; //the position of the camera (if not locked to player)
	ItemNumberPair inventory[INVENTORY_SLOTS]; //the player's inventory
	int width, height; //the dimensions of the player as far as collision is concerned
	//directions are 0 to the right, 1 upwards, 2 to the left, 3 downwards
	int dir = 3; int wait = 0; //the direction the player is facing and how many frames they have to wait before moving
	Texture *texture; //the players texture
	int x, y; //the player's coordinates
	int id; //the players id (different from index within **players
	Player(int id);
	~Player();
	GameState *getState(); //the player's current state
	void pushState(GameState *state); //adds a state to the stack
	void popState(); //removes and cleans up a state from the stack
	void setState(GameState *state); //clears the stack and places this new state on top
	Sprite *getSprite(); //gets the current sprite for the player, animation dependent
	void draw();
	void run();
	bool hasItem(Item *item); //checks to see if the player has an item in their inventory
	bool hasItem(unsigned long long flag); //checks to see if the player has a category of item
	ItemNumberPair* getItem(Item *item); //gets a random stack of items of a given type from the inventory
	ItemNumberPair* getItem(unsigned long long flag); //gets a random item-number pair with a given category from the inventory
	int itemCount(Item *item); //how many of an item the player has
	int getWorldID(); //gets the id of the world the player is in
	void setWorldID(int id); //changes the world the player is in, running the enter code for the world entered
	void lockCamera(); //makes it so the camera follows the player
	void unlockCamera(); //makes it so the camera can be controlled programmatically
	int getCameraCenterX();
	int getCameraCenterY();
};

const int MAX_PLAYERS = 4; //the maximum amount of players for our game
Player* getPlayer(int id); //gets a player based on id (not index in **players)
void addPlayer(int id); //adds a player with a given id
void removePlayer(int id); //removes a player based on id
extern int numPlayers; //the number of players currently playing
extern Player **players; //holds all the players (is a dynamic array of Player pointers)

#endif