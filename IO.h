#ifndef IO_H
#define IO_H
#include "Revengine.h"

#include <string>
using namespace std;

//Holds all file IO stuff

void save(); //save the game
void load(); //loads the game from a save file

class File; //hardware defined File class
File * openFile(string s, bool writing); //opens a file with a specified filename and whether it is reading or writing
void closeFile(File *file); //closes a file

//lots of functions to read and write specific data types
unsigned char readByte(File*file);
unsigned short readUShort(File * file);
unsigned int readUInt(File * file);
unsigned long long readULong(File * file);
char readChar(File * file);
short readShort(File * file);
int readInt(File * file);
long long readLong(File * file);
string readString(File * file);
void writeByte(File *file, unsigned char v);
void writeChar(File * file, char v);
void writeShort(File * file, short v);
void writeInt(File * file, int v);
void writeLong(File * file, long long v);
void writeString(File * file, string v);

#endif