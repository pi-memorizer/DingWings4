#include "Item.h"

Hashmap<string, Item*> items;

Item::Item(string name)
{
	this->name = name;
	this->displayName = name;
}

bool Item::containsFlag(unsigned long long flag)
{
	return (flag & flags)!=0;
}

Item* Item::_flags(unsigned long long flags)
{
	flags |= flags;
	return this;
}

Item* Item::_displayName(string name)
{
	displayName = name;
	return this;
}

Item* Item::_sprite(Sprite *s)
{
	sprite = s;
	return this;
}

Item * addItem(string name)
{
	if (items.contains(name))
	{
		debug("Duplicate item " << name);
		return items[name];
	}
	else {
		Item *i = new Item(name);
		items.add(name, i);
		return i;
	}
}