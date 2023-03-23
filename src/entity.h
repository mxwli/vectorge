#ifndef ENTITY_H
#define ENTITY_H

#include "game.h"
#include "advvec.h"



#define PLAYER 0
//function to create a player and automatically fill in variables with player defaults
Entity prototypePlayer(Node* loc, int team);

void functionPlayer(Entity* ent, float f);

#define SLIME 1
//function to create a slime and automatically fill in variables with slime defaults
Entity prototypeSlime(Node* loc, int team);

void functionSlime(Entity* ent, float f);



#endif
