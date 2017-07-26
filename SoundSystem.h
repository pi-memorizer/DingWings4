#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H
#include "Revengine.h"

//NOTE: Generally, sounds are loaded into memory and need to be short,
//while songs are streams and are the background music

void createSound(string name); //loads a sound effect into memory
void createSong(string name); //loads (or queues) a song
void setBackgroundMusic(string name); //sets which song is the background music
void playSound(string name); //plays a sound effect

#endif