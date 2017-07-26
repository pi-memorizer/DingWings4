#include "Blocking.h"
#include "Player.h"
#include "GameState.h"
#include "Sprite.h"
#include "Item.h"
#include "Event.h"
#include "Animation.h"
#include <cmath>


#ifdef MOBILE
string to_string(int value);
#endif

ApplicationClosingException::ApplicationClosingException()
{

}
ApplicationClosingException::~ApplicationClosingException()
{

}

class TextBox : public GameState
{
	string msg;
	GameState *caller;
	int count = 0;
	bool skippable = true;
	bool done = false;
public:
	TextBox(Player *p, string _msg);
	TextBox(Player *p, string _msg, bool skippable);
	void run();
	void draw();
};

TextBox::TextBox(Player *p, string _msg) : GameState(p)
{
	msg = _msg;
	caller = p->getState();
}

TextBox::TextBox(Player *p, string _msg, bool skippable) : GameState(p)
{
	msg = _msg;
	caller = p->getState();
	this->skippable = skippable;
}

void TextBox::run()
{
	caller->run();
	startMenu();
	if (getKey(p,KEY_A) && !a)
	{
		if (done)
		{
			p->popState();
			return;
		}
	}
	if (getKey(p, KEY_B) && !b&&skippable)
	{
		count = 1000000;
	}
	count++;
	endMenu();
}

int findNextSpace(string msg, int position)
{
	int j = 0;
	for (int i = position; i < msg.length(); i++)
	{
		if (msg[i] == '<')
		{
			if (msg[i + 1] == '<')
			{
				i++;
				j++;
			}
			else {
				while (msg[i] != '>') i++;
			}
		} else 
		if (msg[i] == ' ')
		{
			return j;
		}
		else {
			j++;
		}
	}
	return msg.length()-position;
}

int hexChar(char c)
{
	if (c >= 'A'&&c <= 'F')
		return c - 'A' + 10;
	else if (c >= '0'&&c <= '9')
		return c - '0';
	return 0;
}

bool drawTextBox(string msg, int count)
{
	const int MAX_LINES = 4;
	setDrawColor(0xFF, 0xFF, 0xFF, 0);
	int r = 0;
	int g = 0;
	int b = 0;
	Rect rect;
	rect.x = 0;
	rect.w = WIDTH;
	rect.h = 8 + 2 * MAX_LINES * 8;
	rect.y = HEIGHT - rect.h;
	fillRect(&rect);

	int line = 1;
	int x = 0;
	int j = 0;
	float f = 0.0;
	float df = 1.0;
	int width = WIDTH / 8 - 2;
	int wave = 0;
	int nextSpace = findNextSpace(msg, 0);
	while (j < msg.length()&& (int)f < count)
	{
		if (msg[j] == '<')
		{
			if (msg[j + 1] != '<')
			{
				j++;
				while (msg[j] != '>')
				{
					switch (msg[j])
					{
					case 's':
						df = (float)(msg[j + 1] - '0') + .1*(msg[j + 2] - '0') + .01*(msg[j + 3] - '0');
						df = 1 / df;
						j += 4;
						break;
					case 'c':
						r = hexChar(msg[j + 1]) * 16 + hexChar(msg[j + 2]);
						g = hexChar(msg[j + 3]) * 16 + hexChar(msg[j + 4]);
						b = hexChar(msg[j + 5]) * 16 + hexChar(msg[j + 6]);
						j += 7;
						break;
					case 'w':
						wave = (msg[j+1]-'0');
						j += 2;
						break;
					default:
						j++;
						break;
					}
				}
			}
			j++;
		}
		if (msg[j] == '\n')
		{
			line++;
			x = 0;
			j++;
		}
		if (nextSpace > width - x)
		{
			line++;
			x = 0;
		}

		int y = rect.y + 8 * line;
		if (wave!=0) y += sin(((frames + (int)f) / 5.0))*wave;
		drawCharacter(msg[j], 8 + 8 * x, y,r,g,b);

		j++;
		nextSpace = findNextSpace(msg, j);
		x++;
		f += df;
	}
	return j >= msg.length();
}

void TextBox::draw()
{
	caller->draw();
	if (drawTextBox(msg, count)) done = true;
}

class OptionPane : public GameState
{
	string msg;
	string *choices;
	int numChoices;
	string *output;
	int choice = 0;
	int count = 0;
	GameState *caller;
	bool done = false;
public:
	OptionPane(Player *p, string msg, string choices[], int numChoices, string *output);
	virtual void run();
	virtual void draw();
};

OptionPane::OptionPane(Player *p, string msg, string choices[], int numChoices, string *output) : GameState(p)
{
	this->msg = msg;
	this->choices = choices;
	this->numChoices = numChoices;
	this->output = output;
	caller = p->getState();
}

void OptionPane::run()
{
	caller->run();
	startMenu();
	if (getKey(p,KEY_UP) && !up)
	{
		choice--;
		if (choice < 0) choice = numChoices - 1;
	}
	if (getKey(p,KEY_DOWN) && !down)
	{
		choice++;
		if (choice >= numChoices) choice = 0;
	}
	if (getKey(p,KEY_A) && !a)
	{
		if (done)
		{
			*output = choices[choice];
			delete[] choices;
			p->popState();
			return;
		}
	}
	if (getKey(p,KEY_B) && !b)
	{
		count = 1000000;
		/**output = "null";
		p->popState();
		return;*/
	}
	count++;
	endMenu();
}

void OptionPane::draw()
{
	caller->draw();
	if (drawTextBox(msg, count)) done = true;
	drawCharacter('>', 0, choice * 8, 0,0,0);
	for (int i = 0; i < numChoices; i++)
	{
		int length = choices[i].length();
		for (int j = 0; j < length; j++)
		{
			drawCharacter(choices[i][j],WIDTH - 8 * length + j * 8, i * 8,0,0,0);
		}
	}
}

void bTextbox(Player *p, string s, bool skippable)
{
	TextBox * text = new TextBox(p, s, skippable);
	p->pushState(text);
	while (text == p->getState())
	{
		if (!theLoop())
		{
			throw ApplicationClosingException();
		}
	}
}

void bTextbox(Player *p, string s)
{
	bTextbox(p, s, true);
}

string bOptionPane(Player *p, string msg, string choices[], int numChoices)
{
	string output;
	OptionPane * pane = new OptionPane(p, msg, choices, numChoices, &output);
	p->pushState(pane);
	while (pane == p->getState())
	{
		if (!theLoop())
		{
			throw ApplicationClosingException();
		}
	}
	return output;
}

string bOptionPane(Player *p,string msg, string choice1, string choice2)
{
	string *choices = new string[2]{
		choice1, choice2
	};
	return bOptionPane(p,msg, choices, 2);
}
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3)
{
	string *choices = new string[3]{
		choice1, choice2, choice3
	};
	return bOptionPane(p,msg, choices, 3);
}
string bOptionPane(Player *p,string msg, string choice1, string choice2, string choice3, string choice4)
{
	string *choices = new string[4]{
		choice1, choice2, choice3, choice4
	};
	return bOptionPane(p,msg, choices, 4);
}
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3, string choice4, string choice5)
{
	string *choices = new string[5]{
		choice1, choice2, choice3, choice4, choice5
	};
	return bOptionPane(p,msg, choices, 5);
}
string bOptionPane(Player *p,string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6)
{
	string *choices = new string[6]{
		choice1, choice2, choice3, choice4, choice5, choice6
	};
	return bOptionPane(p,msg, choices, 6);
}
string bOptionPane(Player *p,string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6, string choice7)
{
	string *choices = new string[7]{
		choice1, choice2, choice3, choice4, choice5, choice6, choice7
	};
	return bOptionPane(p,msg, choices, 7);
}
string bOptionPane(Player *p,string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6, string choice7, string choice8)
{
	string *choices = new string[8]{
		choice1, choice2, choice3, choice4, choice5, choice6, choice7, choice8
	};
	return bOptionPane(p,msg, choices, 8);
}

class NumberPane : public GameState
{
	int delay = 0;
	string msg;
	int choice, min, max;
	int *output;
	GameState*caller;
	int count = 0;
	bool done = false;
public:
	NumberPane(Player *p, string msg, int start, int min, int max, int*output);
	virtual void run();
	virtual void draw();
};

NumberPane::NumberPane(Player *p, string msg, int start, int min, int max, int *output) : GameState(p)
{
	this->msg = msg;
	choice = start;
	this->min = min;
	this->max = max;
	this->output = output;
	caller = p->getState();
}

void NumberPane::draw()
{
	caller->draw();
	if (drawTextBox(msg, count)) done = true;
	string num = to_string(choice);
	for (int i = 0; i < num.length(); i++)
	{
		drawCharacter(num[i], i * 8, 0, 0, 0, 0);
	}
}

void NumberPane::run()
{
	caller->run();
	startMenu();
	if (getKey(p,KEY_A) && !a)
	{
		if (done)
		{
			*output = choice;
			p->popState();
			return;
		}
	}
	if (getKey(p,KEY_B) && !b)
	{
		count = 1000000;
		/**output = 0;
		p->popState();
		return;*/
	}
	if (getKey(p,KEY_UP))
	{
		if (!up)
		{
			delay = 0;
			choice--;
			if (choice < min) choice = max;
		}
		else {
			if (delay >= 40)
			{
				choice--;
				if (choice < min) choice = max;
			}
			else delay++;
		}
	} else if (getKey(p,KEY_DOWN))
	{
		if (!down)
		{
			delay = 0;
			choice++;
			if (choice >max) choice =  min;
		}
		else {
			if (delay >= 40)
			{
				choice++;
				if (choice > max) choice = min;
			}
			else delay++;
		}
	}
	else delay = 0;
	count++;
	endMenu();
}

int bNumberPane(Player *p, string msg, int start, int min, int max)
{
	int output;
	NumberPane *pane = new NumberPane(p, msg, start, min, max, &output);
	p->pushState(pane);
	while (pane == p->getState())
	{
		if (!theLoop())
		{
			throw ApplicationClosingException();
		}
	}
	return output;
}

class InventoryDialogue : public GameState
{
	int pointerX, pointerY;
	static int positionPairs[];
	static Sprite *inventoryUI, *inventoryPointer, *inventoryBadSpot;
	Item **output;
	int moveToNearest();
	unsigned long long categories;
public:
	InventoryDialogue(Player *p, unsigned long long categories, Item**output);
	virtual void run();
	virtual void draw();
};

Sprite *InventoryDialogue::inventoryUI = nullptr;
Sprite *InventoryDialogue::inventoryPointer = nullptr;
Sprite *InventoryDialogue::inventoryBadSpot = nullptr;

int InventoryDialogue::positionPairs[2*INVENTORY_SLOTS] = {
	21,15,
	117,28,
	64,90
};

InventoryDialogue::InventoryDialogue(Player *p, unsigned long long categories, Item **output) : GameState(p)
{
	this->categories = categories;
	this->output = output;
}

void InventoryDialogue::run()
{
	int index = 0;
	const int MOVE_AMOUNT = 100;
	startMenu();
	if (getKey(p,KEY_A) && !a)
	{
		if (p->inventory[index].item != nullptr)
		{
			if (categories == (p->inventory[index].item->flags&categories))
			{
				*output = p->inventory[index].item;
				p->popState();
				return;
			}
		}
		else {
			*output = nullptr;
			p->popState();
			return;
		}
	}
	if (getKey(p,KEY_B) && !b)
	{
		*output = nullptr;
		p->popState();
		return;
	}
	if (getKey(p,KEY_UP) && !up)
	{
		pointerY -= MOVE_AMOUNT;
	}
	if (getKey(p,KEY_DOWN) && !down)
	{
		pointerY += MOVE_AMOUNT;
	}
	if (getKey(p,KEY_LEFT) && !left)
	{
		pointerX -= MOVE_AMOUNT;
	}
	if (getKey(p,KEY_RIGHT) && !right)
	{
		pointerX += MOVE_AMOUNT;
	}
	bool found = false;
	for (int i = 0; i < 2 * INVENTORY_SLOTS; i += 2)
	{
		if (pointerX == positionPairs[i] && pointerY == positionPairs[i + 1])
		{
			found = true;
			index = i / 2;
			break;
		}
	}
	if (!found)
	{
		index = moveToNearest();
	}
	endMenu();
}

int InventoryDialogue::moveToNearest()
{
	int minD = 999999999;
	int x = pointerX;
	int y = pointerY;
	int index = -1;
	for (int i = 0; i < 2 * INVENTORY_SLOTS; i += 2)
	{
		int _x = positionPairs[i];
		int _y = positionPairs[i + 1];
		int d = (_x - pointerX)*(_x - pointerX) + (_y - pointerY)*(_y - pointerY);
		if (d < minD)
		{
			minD = d;
			x = _x;
			y = _y;
			index = i/2;
		}
	}
	pointerX = x;
	pointerY = y;
	return index;
}

void InventoryDialogue::draw()
{
	if (inventoryUI == nullptr)
	{
		inventoryUI = new Sprite("inventoryUI", 0, 0);
		inventoryPointer = new Sprite("inventoryPointer", 0, 0);
		inventoryBadSpot = new Sprite("inventoryBadSpot", 0, 0);
	}
	inventoryUI->draw(0, 0);
	inventoryPointer->draw(pointerX, pointerY);
	for (int i = 0; i < 2 * INVENTORY_SLOTS; i += 2)
	{
		int x = positionPairs[i];
		int y = positionPairs[i + 1];
		Item *item = p->inventory[i / 2].item;
		if (item != nullptr)
		{
			Sprite *s = item->sprite;
			if (s != nullptr)
			{
				s->draw(x, y);
				if (categories != (categories&item->flags))
				{
					inventoryBadSpot->draw(x, y);
				}
			}
		}
	}
}

Item* bSelectItem(Player *p)
{
	return bSelectItem(p,~((unsigned long long)0));
}

Item* bSelectItem(Player *p, unsigned long long categories)
{
	Item *output = nullptr;
	InventoryDialogue *state = new InventoryDialogue(p, categories, &output);
	p->pushState(state);
	while (state == p->getState())
	{
		if (!theLoop())
		{
			throw ApplicationClosingException();
		}
	}
	return output;
}

Script::Script(Player *p,GameState*caller) : GameState(p)
{
	this->caller = caller;
}

void Script::start(Player *p, int x, int y, void(*script)(Player*,int,int,Script*))
{
	Script *s = new Script(p,p->getState());
	s->x = x;
	s->y = y;
	p->pushState(s);
	s->_script = script;
}

void Script::run()
{
	caller->run();
	if (this != p->getState()) return;
	currentState = 0;
	try {
		_script(p,x,y,this);
		p->popState();
	} catch(int a) { }
}

void Script::draw()
{
	caller->draw();
}

Script::Return::Return()
{
}
Script::Return::Return(string s)
{
	this->s = s;
}
Script::Return::Return(int i)
{
	this->i = i;
}
Script::Return::Return(Item* item)
{
	this->item = item;
}

void Script::textBox(string s, bool skippable)
{
	if (currentState == states.length())
	{
		states.add(Return());
		TextBox *t = new TextBox(p, s, skippable);
		p->pushState(t);
		assert(this != p->getState());
		currentState++;
		throw 0;
	}
	else {
		currentState++;
	}
}

string Script::optionPane(string msg, string choice1, string choice2)
{
	if (currentState == states.length())
	{
		states.add(Return(""));
		string *s = new string[2] {
			choice1, choice2
		};
		OptionPane *o = new OptionPane(p, msg, s, 2, &states[states.length() - 1].s);
		p->pushState(o);
		currentState++;
		throw 0;
	}
	else {
		return states[currentState++].s;
	}
}

int Script::numberPane(string msg, int start, int min, int max)
{
	if (currentState == states.length())
	{
		states.add(Return(0));
		NumberPane *n = new NumberPane(p, msg, start, min, max, &states[states.length() - 1].i);
		p->pushState(n);
		currentState++;
		throw 0;
	}
	else {
		return states[currentState++].i;
	}
}

Item* Script::selectItem()
{
	return selectItem(~(unsigned long long)0);
}

Item* Script::selectItem(unsigned long long categories)
{
	if (currentState == states.length())
	{
		states.add(Return(nullptr));
		InventoryDialogue *i = new InventoryDialogue(p, categories, &states[states.length() - 1].item);
		p->pushState(i);
		currentState++;
		throw 0;
	}
	else {
		return states[currentState++].item;
	}
}

void Script::blockingAnimation(Animation *animation)
{
	if (currentState == states.length())
	{
		states.add(Return());
		p->pushState(animation);
		assert(this != p->getState());
		currentState++;
		throw 0;
	}
	else {
		delete animation;
		currentState++;
	}
}