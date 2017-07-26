#ifndef BLOCKING_H
#define BLOCKING_H
#include "Revengine.h"
#include "GameState.h"

///////////////////////////////////////////////
//IMPORTANT (possibly made up) TERMINOLOGY   //
//                                           //
//Blocking - Function takes an indefinite    //
//           amount of time executing, via   //
//           running a version of the game   //
//           engine within itself, then      //
//           returns its value to the        //
//           original spot in the code.      //
//                                           //
//           Similar to:                     //
//           OpenFileDialog.ShowDialog()     //
//                                           //
//Non-Blocking - Any function that doesn't   //
//           block (aka almost every         //
//           function ever.                  //
///////////////////////////////////////////////

//allows for multiplayer blocking functions
class Script : public GameState
{
public: //is set as public, though you will likely never need it
	struct Return{ //holds various return types of scripting functions
		Return(string s);
		Return(int i);
		Return(Item* item);
		Return();
		string s = "";
		int i = 0;
		Item* item = nullptr;
	};
private:
	Script(Player *p,GameState*caller);
	List <Return> states;
	GameState*caller;
	int currentState = 0;
	void(*_script)(Player*,int x, int y,Script*);
	int x, y;
public:
	//Starts a new script
	//*p is the current player
	//x is the coordinate in map coordinates (if any) that the player clicked on to initiate event
	//y is the same but for the y direction
	//*script is a function pointer to a valid script function (the function that holds your scripting functions)
	static void start(Player *p, int x, int y, void(*script)(Player*,int,int,Script*));
	
	void run();
	void draw();
	void textBox(string s, bool skippable); //scripting textbox
	string optionPane(string msg, string choice1, string choice2); //gives you options between two strings, returning the choice made
	int numberPane(string msg, int start, int min, int max); //lets the player choose a number within a range, returning that number
	Item* selectItem(); //Selects an item from the inventory, regardless of category
	Item* selectItem(unsigned long long categories); //Selects an item from the inventory if it is part of the given categories, return the Item chosen
	void blockingAnimation(Animation *animation); //runs a blocking animation
};

/////////////////////////////////////
//All of these are non-blocking    //
//so ONLY use them in SINGLEPLAYER //
/////////////////////////////////////

void bTextbox(Player *p, string s);
void bTextbox(Player *p, string s, bool skippable);
string bOptionPane(Player *p, string msg, string choices[], int numChoices);
string bOptionPane(Player *p, string msg, string choice1, string choice2);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3, string choice4);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3, string choice4, string choice5);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6, string choice7);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6, string choice7, string choice8);
int bNumberPane(Player *p, string msg, int start, int min, int max);
Item *bSelectItem(Player *p);
Item *bSelectItem(Player *p, unsigned long long categories);


//Special exception type to be thrown if the application needs to close during a blocked call
struct ApplicationClosingException
{
	ApplicationClosingException();
	~ApplicationClosingException();
};

#endif