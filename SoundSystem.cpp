#include "SoundSystem.h"
/*
SoundSystem *soundSystem;
Sound *sounds[1024];

SoundSystem::SoundSystem()
{
	song = "";
	music = nullptr;
}

SoundSystem::~SoundSystem()
{
	if (Mix_PlayingMusic() == 1)
	{
		Mix_HaltMusic();
	}
	Mix_FreeMusic(music);
	music = nullptr;
}

void SoundSystem::setBackgroundMusic(string name)
{
	if (name != song)
	{
		if (Mix_PlayingMusic() == 1)
		{
			Mix_HaltMusic();
		}
		if (music != nullptr)
		{
			Mix_FreeMusic(music);
		}
		music = Mix_LoadMUS(("sound/" + name+".wav").c_str());
		song = name;
		if (music != nullptr)
		{
			Mix_PlayMusic(music, -1);
		}
	}
}

Sound::Sound(string name)
{
	chunk = Mix_LoadWAV(("sound/" +name+".wav").c_str());
}

Sound::~Sound()
{
	Mix_FreeChunk(chunk);
	chunk = nullptr;
}

void Sound::play()
{
	if (chunk != nullptr)
	{
		Mix_PlayChannel(-1, chunk, 0);
	}
	else {
#ifdef DEBUG
		cout << "Error playing sound" << endl;
#endif
	}
}*/