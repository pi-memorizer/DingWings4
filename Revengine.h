#ifndef REVENGINE_H
#define REVENGINE_H

#define DEBUG true //turn to false before releasing game
//#define UNIX true

#include <string>
#include "Collections.h"
using namespace std;

//defines assert() and debug() based on debugging mode 
//assert() will crash the program if the value passed in is false (used for error checking)
//debug() prints stuff to the screen (use << between the arguments)
#if DEBUG == true
	#include <iostream>
	using namespace std;
	#define assert(a) if(!(a)) throw "Assertion failed";
	#define debug(a) cout << a << endl;
#else
	#define assert(a) ;;
	#define debug(a) ;;
#endif

//the resolution of the game (different from resolution of the screen)
#define WIDTH 320
#define HEIGHT 240

//the size of map tiles
#define TILE_SIZE 32

//how many frames the game has been running
extern int frames;

//tells the system "these classes exist but aren't defined yet"
class Sprite;
class GameState;
class Textbox;
class Player;
class SoundSystem;
class Sound;
class World;
class WorldState;
class Entity;
class Item;

void delay(int millis); //blocks for however many milliseconds
int startGame(); //called when the game is started
void exitGame(); //called when the game is closed

/////////////////////////////////////////////////////
//IMPORTANT IDEA:                                  //
//                                                 //
//When you have an arbitrary piece of data that    //
//pertains to the game state for all players, and  //
//that can be assigned an integer value, put it in //
//here. These are saved and loaded into the game   //
//automatically.                                   //
//                                                 //
//Use getFlag and setFlag functions, because if a  //
//flag does not exist, getFlag will return 0, and  //
//if it does setFlag will update the value, both   //
//instead of crashing the program.                 //
/////////////////////////////////////////////////////

extern Hashmap<string, int> flags;
int getFlag(string s);
void setFlag(string s, int value);

extern List<World*> worlds;//list of all the worlds
int getOnscreenX(Player *p, int x); //gives the position on the screen of a coordinate with respect to the player's camera
int getOnscreenY(Player *p, int y);
int safeDiv(int x, int factor); //divides a number "safely", in other words, integer division with negatives the way we want it
bool rectCollides(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2); //checks if two rectangles touch
bool theLoop(); //the core game loops, returns false if the game needs to close
void init(); //calls initializing code found in Init.cpp, but game scripting and stuff here
#endif