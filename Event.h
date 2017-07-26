#ifndef EVENT_H
#define EVENT_H

#include "Player.h"

//Used in the event handling system

enum EventType { EVENT_QUIT, EVENT_KEY_DOWN, EVENT_KEY_UP, EVENT_UNKNOWN };
enum EventKey {
	KEY_A, KEY_B, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_UNKNOWN
};

//Event data structure that holds the type and any special arguments
struct Event
{
	EventType type;
	union {
		struct { EventKey key; } keys;
	};
};

//returns true if a player has a given key pressed
bool getKey(Player *p, EventKey key);

//pools the OS for any events, returning true if it finds one
bool getEvent(Event *e);

#endif