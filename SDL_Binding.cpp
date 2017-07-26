#ifndef UNIX
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#else
#include "SDL/SDL.H"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#endif

#include "Revengine.h"
#include "Event.h"
#include "Player.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Blocking.h"

SDL_Window *window;
SDL_Renderer *renderer;
Hashmap<int, bool> keys;
SDL_DisplayMode displayMode;
Hashmap<string, Mix_Chunk*> sounds;
Mix_Music *music;
string song;
Sprite ***chars;
List<SDL_Joystick*> joysticks;
struct PlayerMap
{
	bool a = false, b = false, up = false, down = false, left = false, right = false;
	bool playing = false;
};

PlayerMap playerMap[MAX_PLAYERS];

bool fullscreen = true;
const int JOYSTICK_DEAD_ZONE = 12000;


class File
{
public:
	SDL_RWops *file;
};

class Texture
{
public:
	SDL_Texture *texture;
};

void setDrawColor(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void clearScreen()
{
	SDL_RenderClear(renderer);
}

void getWindowSize(int *w, int *h)
{
	SDL_GetWindowSize(window, w, h);
}

void display()
{
	SDL_RenderPresent(renderer);
}

void startDrawingPlayer(int index)
{
	if (index == -1)
	{
		SDL_SetRenderTarget(renderer, nullptr);
	}
	else {
		SDL_SetRenderTarget(renderer, players[index]->texture->texture);
	}
}

void setWindowFullscreen(bool b)
{
	SDL_SetWindowFullscreen(window, b ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void setWindowSize(int width, int height)
{
	SDL_SetWindowSize(window, width, height);
}

void centerWindow()
{
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

int getScreenWidth()
{
	return displayMode.w;
}

int getScreenHeight()
{
	return displayMode.h;
}

void drawTexture(Texture *texture, Rect *rect)
{
	SDL_Rect r;
	r.w = rect->w;
	r.h = rect->h;
	r.x = rect->x;
	r.y = rect->y;
	SDL_RenderCopy(renderer, texture->texture, nullptr, &r);
}

void drawTexture(Texture *texture, Rect *source, Rect *dest)
{
	SDL_Rect s, d;
	s.w = source->w;
	s.h = source->h;
	s.x = source->x;
	s.y = source->y;
	d.w = dest->w;
	d.h = dest->h;
	d.x = dest->x;
	d.y = dest->y;
	SDL_RenderCopy(renderer, texture->texture, &s, &d);
}

void drawTexture(Texture *texture, Rect *source, Rect *dest, int alpha)
{
	SDL_Rect s, d;
	s.w = source->w;
	s.h = source->h;
	s.x = source->x;
	s.y = source->y;
	d.w = dest->w;
	d.h = dest->h;
	d.x = dest->x;
	d.y = dest->y;
	SDL_SetTextureAlphaMod(texture->texture, (unsigned char)alpha);
	SDL_RenderCopy(renderer, texture->texture, &s, &d);
	SDL_SetTextureAlphaMod(texture->texture, 255);
}

Texture *createTexture(int width, int height)
{
	Texture * t = new Texture();
	t->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width, height);
	return t;
}

Texture *loadTexture(string filename)
{
	Texture * t = new Texture();
	t->texture = IMG_LoadTexture(renderer, ("image/" + filename + ".png").c_str());
	return t;
}

void getTextureSize(Texture *t, int *w, int *h)
{
	SDL_QueryTexture(t->texture, nullptr, nullptr, w, h);
}

void drawCharacter(char c, int x, int y, int r, int g, int b)
{
	chars[0][c]->draw(x, y, 255);
	if(r>0)chars[1][c]->draw(x, y, r);
	if(g>0)chars[2][c]->draw(x, y, g);
	if(b>0)chars[3][c]->draw(x, y, b);
}

void destroyTexture(Texture *texture)
{
	SDL_DestroyTexture(texture->texture);
	delete texture;
}
void fillRect(Rect *rect)
{
	SDL_Rect r;
	r.w = rect->w;
	r.h = rect->h;
	r.x = rect->x;
	r.y = rect->y;
	SDL_RenderFillRect(renderer, &r);
}

int ticks = SDL_GetTicks();

void delay(int millis)
{
	int ticks_ = SDL_GetTicks();
	if(ticks_-ticks<millis)
		SDL_Delay(millis-(ticks_-ticks));
	ticks = ticks_;
}

int startGame()
{
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
	{
		return -2;
	}
	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		exitGame();
		return -3;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)<0)
	{
		exitGame();
		return -4;
	}
	SDL_GetDesktopDisplayMode(0, &displayMode);
	window = SDL_CreateWindow("Revengine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (window == nullptr)
	{
		exitGame();
		return -1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		exitGame();
		return -1;
	}
	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		joysticks.add(SDL_JoystickOpen(i));
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	for (int i = SDLK_a; i <= SDLK_z; i++)
		keys.add(i, false);
	keys.add(SDLK_F4, false);
	keys.add(SDLK_F11, false);
	keys.add(SDLK_ESCAPE, false);
	playerMap[0].playing = true;
	chars = new Sprite**[4];
	for (int j = 0; j < 4; j++)
	{
		Sprite *s = new Sprite("charset" + to_string(j), 0, 0);
		chars[j] = new Sprite*[256];
		for (int i = 0; i < 256; i++)
		{
			Sprite *sprite = new Sprite(s, 8 * (i % 16), 8 * (i / 16), 8, 8, 0, 0);
			chars[j][i] = sprite;
			if(j!=0)SDL_SetTextureBlendMode(sprite->sprite->texture, SDL_BLENDMODE_ADD);
		}
	}
	return 0;
}

void exitGame()
{
	if (window != nullptr) SDL_DestroyWindow(window);
	if (renderer != nullptr) SDL_DestroyRenderer(renderer);
	for (int i = 0; i < sprites.length(); i++)
	{
		delete sprites[i];
	}
	for (int i = 0; i < numPlayers; i++)
	{
		delete players[i];
	}
	if (Mix_PlayingMusic() == 1)
	{
		Mix_HaltMusic();
	}
	Mix_FreeMusic(music);
	music = nullptr;
	List<Mix_Chunk*> soundList = sounds.values();
	for (int i = 0; i < soundList.length(); i++)
	{
		Mix_FreeChunk(soundList[i]);
	}
	for (int i = 0; i < joysticks.length(); i++)
	{
		SDL_JoystickClose(joysticks[i]);
		joysticks[i] = nullptr;
	}
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

File * openFile(string s, bool writing)
{
	File * file = new File();
	file->file = nullptr;
	if (writing)
	{
		file->file = SDL_RWFromFile(s.c_str(), "w+b");
	}
	else {
		file->file = SDL_RWFromFile(s.c_str(), "r+b");
	}
	if (file->file == nullptr)
	{
		delete file;
		return nullptr;
	}
	return file;
}

void closeFile(File *file)
{
	SDL_RWclose(file->file);
	delete file;
}

unsigned char readByte(File * file)
{
	unsigned char c;
	SDL_RWread(file->file, &c, sizeof(unsigned char), 1);
	return c;
}
char readChar(File * file)
{
	char c;
	SDL_RWread(file->file, &c, sizeof(char), 1);
	return c;
}
short readShort(File * file)
{
	return SDL_ReadBE16(file->file);
}
int readInt(File * file)
{
	return SDL_ReadBE32(file->file);
}
long long readLong(File * file)
{
	return SDL_ReadBE64(file->file);
}
unsigned short readUShort(File * file)
{
	return SDL_ReadBE16(file->file);
}
unsigned int readUInt(File * file)
{
	return SDL_ReadBE32(file->file);
}
unsigned long long readULong(File * file)
{
	return SDL_ReadBE64(file->file);
}
string readString(File * file)
{
	int length = readShort(file);
	char * c = new char[length + 1];
	c[length] = 0;
	SDL_RWread(file->file, c, length, 1);
	string s(c);
	delete[] c;
	return s;
}
void writeByte(File * file, unsigned char v)
{
	SDL_RWwrite(file->file, &v, sizeof(unsigned char), 1);
}
void writeChar(File * file, char v)
{
	SDL_RWwrite(file->file, &v, sizeof(char), 1);
}
void writeShort(File * file, short v)
{
	SDL_WriteBE16(file->file, v);
}
void writeInt(File * file, int v)
{
	SDL_WriteBE32(file->file, v);
}
void writeLong(File * file, long long v)
{
	SDL_WriteBE64(file->file, v);
}
void writeString(File * file, string v)
{
	writeShort(file, (short)v.length());
	SDL_RWwrite(file->file, v.c_str(), v.length(), 1);
}

//Key press related stuff

void switchKey(int k, bool state)
{
	switch (k)
	{
	case SDLK_w:
	case SDLK_UP:
		playerMap[0].up = state;
		break;
	case SDLK_d:
	case SDLK_RIGHT:
		playerMap[0].right = state;
		break;
	case SDLK_s:
	case SDLK_DOWN:
		playerMap[0].down = state;
		break;
	case SDLK_a:
	case SDLK_LEFT:
		playerMap[0].left = state;
		break;
	case SDLK_j:
	case SDLK_z:
		playerMap[0].a = state;
		break;
	case SDLK_k:
	case SDLK_x:
		playerMap[0].b = state;
		break;
	default:
		break;
	}
}

bool getEvent(Event *e)
{
	SDL_Event sdl;
	if (SDL_PollEvent(&sdl))
	{
		if (sdl.type == SDL_QUIT)
		{
			e->type = EVENT_QUIT;
		}
		else if (sdl.type==SDL_KEYDOWN&&!sdl.key.repeat)
		{
			if (keys.contains(sdl.key.keysym.sym))
			{
				if (sdl.key.keysym.sym == SDLK_F11&&!keys[SDLK_F11])
				{
					if (fullscreen)
					{
						setWindowFullscreen(false);
						int scaleX = getScreenWidth() / WIDTH;
						int scaleY = getScreenHeight() / HEIGHT;
						int scalar = scaleX;
						if (scaleY < scaleX) scalar = scaleY;
						setWindowSize(scalar * WIDTH, scalar * HEIGHT);
						centerWindow();
					}
					else {
						setWindowFullscreen(true);
						setWindowSize(getScreenWidth(), getScreenHeight());
					}
					fullscreen = !fullscreen;
				}
				else if (sdl.key.keysym.sym == SDLK_ESCAPE)
				{
					e->type = EVENT_QUIT;
				} else {
					switchKey(sdl.key.keysym.sym, true);
				}
				keys[sdl.key.keysym.sym] = true;
				e->keys.key = KEY_UNKNOWN;
			}
			else {
				e->keys.key = KEY_UNKNOWN;
			}
		}
		else if (sdl.type==SDL_KEYUP&&!sdl.key.repeat)
		{
			if (keys.contains(sdl.key.keysym.sym))
			{
				keys[sdl.key.keysym.sym] = false;
				e->keys.key = KEY_UNKNOWN;
				switchKey(sdl.key.keysym.sym, false);
			}
			else {
				e->keys.key = KEY_UNKNOWN;
			}
		}
		else if (sdl.type == SDL_JOYAXISMOTION)
		{
			if (sdl.jaxis.which < MAX_PLAYERS)
			{
				if (!playerMap[sdl.jaxis.which].playing)
				{
					addPlayer(sdl.jaxis.which);
					playerMap[sdl.jaxis.which].playing = true;
				}
				if (sdl.jaxis.axis == 0)
				{
					if (sdl.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						playerMap[sdl.jaxis.which].left = true;
						playerMap[sdl.jaxis.which].right = false;
					}
					else if (sdl.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						playerMap[sdl.jaxis.which].right = true;
						playerMap[sdl.jaxis.which].left = false;
					}
					else {
						playerMap[sdl.jaxis.which].right = false;
						playerMap[sdl.jaxis.which].left = false;
					}
				}
				else {
					if (sdl.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						playerMap[sdl.jaxis.which].up = true;
						playerMap[sdl.jaxis.which].down = false;
					}
					else if (sdl.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						playerMap[sdl.jaxis.which].down = true;
						playerMap[sdl.jaxis.which].up = false;
					}
					else {
						playerMap[sdl.jaxis.which].up = false;
						playerMap[sdl.jaxis.which].down = false;
					}
				}
			}
		}
		else if (sdl.type == SDL_JOYBUTTONDOWN)
		{
			if (sdl.jbutton.which < MAX_PLAYERS)
			{
				if (!playerMap[sdl.jaxis.which].playing)
				{
					addPlayer(sdl.jaxis.which);
					playerMap[sdl.jaxis.which].playing = true;
				}
				if (sdl.jbutton.button % 2 == 0)
				{
					playerMap[sdl.jbutton.which].a = true;
				}
				else {
					playerMap[sdl.jbutton.which].b = true;
				}
			}
		}
		else if (sdl.type == SDL_JOYBUTTONUP)
		{
			if (sdl.jbutton.which < MAX_PLAYERS)
			{
				if (sdl.jbutton.button % 2 == 0)
				{
					playerMap[sdl.jbutton.which].a = false;
				}
				else {
					playerMap[sdl.jbutton.which].b = false;
				}
			}
		}
		else if (sdl.type == SDL_JOYHATMOTION)
		{
			if (sdl.jhat.which < MAX_PLAYERS)
			{
				if (!playerMap[sdl.jaxis.which].playing)
				{
					addPlayer(sdl.jaxis.which);
					playerMap[sdl.jaxis.which].playing = true;
				}
				playerMap[sdl.jhat.which].up = sdl.jhat.value&1;
				playerMap[sdl.jhat.which].down = sdl.jhat.value&4;
				playerMap[sdl.jhat.which].left = sdl.jhat.value&8;
				playerMap[sdl.jhat.which].right = sdl.jhat.value&2;
			}
		}
		else {
			e->type = EVENT_UNKNOWN;
		}
		return true;
	}
	else {
		e->type = EVENT_UNKNOWN;
		return false;
	}
}

bool getKey(Player *p, EventKey key)
{
	PlayerMap* m = &playerMap[p->id];
	switch (key)
	{
	case KEY_A:
		return m->a;
	case KEY_B:
		return m->b;
	case KEY_UP:
		return m->up;
	case KEY_DOWN:
		return m->down;
	case KEY_LEFT:
		return m->left;
	case KEY_RIGHT:
		return m->right;
	default:
		break;
	}
	return false;
}

void createSound(string name)
{
	sounds.add(name, Mix_LoadWAV(("sound/" + name + ".wav").c_str()));
}

void createSong(string name)
{

}

void setBackgroundMusic(string name)
{
	if (song != name)
	{
		if (Mix_PlayingMusic() == 1)
		{
			Mix_HaltMusic();
		}
		if (music != nullptr)
		{
			Mix_FreeMusic(music);
		}
		music = Mix_LoadMUS(("sound/" + name + ".wav").c_str());
		song = name;
		if (music != nullptr)
		{
			Mix_PlayMusic(music, -1);
		}
	}
}

void playSound(string name)
{
	if (sounds[name] != nullptr)
	{
		Mix_PlayChannel(-1, sounds[name], 0);
	}
}

#ifdef main
#undef main
#endif
int main(int argc, char **argv)
{
	debug("Debugging mode active");

	if (startGame() != 0)
	{
		return -1;
	}

	addPlayer(0);

	init();

	try {
		while (theLoop());
	}
	catch (ApplicationClosingException &a)
	{
		debug("Application closing exception caught");
	}
	exitGame();
	return 0;
}