#ifndef PRISON_H
#define PRISON_H

class Player;

void prisonEnter(Player*p);
void prisonRun();
bool prisonInteract(Player *p, int x, int y);

#endif