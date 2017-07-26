#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <string>
using namespace std;

//generic rectangle class with x and y coordinates as well as width and height
struct Rect
{
	int w, h, x, y;
};
//Texture class defined by hardware
class Texture;

void setDrawColor(int r, int g, int b, int a); //sets the color that clearScreen() calls will use
void clearScreen(); //clears the screen. If an alpha is specified, things under it will be able to be seen
void getWindowSize(int *w, int *h); //returns the game window's size in pixels
void display(); //commits draw changes to the screen
void startDrawingPlayer(int index); //specifies which player to draw to, passing in the index of the player withing players[] or -1 if you want to draw to the screen
int getScreenWidth(); //gets the width of the entire screen
int getScreenHeight(); //gets the height of the entire screen
void drawTexture(Texture *texture, Rect *rect); //draws a texture in a given rectangle
void drawTexture(Texture *texture, Rect *source, Rect *dest); //draws a textures from a portion of the texture, as with tilemaps
void drawTexture(Texture *texture, Rect *source, Rect *dest, int alpha); //same as before with optional alpha from 0 to 255
Texture *createTexture(int width, int height); //creates a blank texture with given width and height, used for individual player screens
void fillRect(Rect *r); //fills a small area of the screen akin to clearScreen()
Texture *loadTexture(string filename); //loads a texture from a file
void getTextureSize(Texture *texture, int *w, int *h); //gets the dimensions of a texture
void destroyTexture(Texture *texture); //removes a texture from memory
void drawCharacter(char c, int x, int y, int r, int g, int b); //draws a character with a given color and coordinate from the bitmap font
#endif