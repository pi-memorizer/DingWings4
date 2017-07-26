#ifndef ITEM_H
#define ITEM_H
#include "Revengine.h"

//Holds data about items

//The different Item categories, expressed with values that are exponents of two (bit flags)
#define ITEM_FOOD 1
#define ITEM_FURNITURE 2

///////////////////////////////////////////////////////
//IMPORTANT IDEA                                     //
//                                                   //
//Items can be initialized using the following       //
//syntax, using the functions that start with        //
//underscores:                                       //
//                                                   //
//Item *i = addItem("Apple")                         //
//          ->_flags(ITEM_FOOD|ITEM_OTHER_CATEGORY)  //
//          ->_sprite(spriteForInventory);           //
//                                                   //
///////////////////////////////////////////////////////

//Generic item class that holds the item's sprite, internal name, display name, and categories
class Item
{
public:
	string name;
	Sprite *sprite = nullptr;
	string displayName;
	unsigned long long flags;
	Item(string name);
	bool containsFlag(unsigned long long flag);
	Item* _flags(unsigned long long flags);
	Item* _displayName(string name);
	Item* _sprite(Sprite *sprite);
};

//a map of all the items in use
//all items created with the addItem function will be added automatically to this
//access items via their internal name (which can be different than the display name,
//especially if multiple items have the same display name)
extern Hashmap<string, Item*> items;

//creates an item given a certain name and adds it to the item list
Item* addItem(string name);

#endif