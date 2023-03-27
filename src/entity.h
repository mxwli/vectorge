#ifndef ENTITY_H
#define ENTITY_H

#include "game.h"
#include "advvec.h"

//to add another entity entry you must:
/*
1. add its preprocessor declaration and functional declaration to this file
2. add its functional implementation to entity.c
3. add its functionality to functionEnt in game.c
4. (optional) implement its drawing characteristics in drawFrame in main.c
*/

#define PLAYER 0
//function to create a player and automatically fill in variables with player defaults
Entity prototypePlayer(Node* loc, int team);

void functionPlayer(Entity* ent, float f);

#define SLIME 1
//function to create a slime and automatically fill in variables with slime defaults
Entity prototypeSlime(Node* loc, int team);

void functionSlime(Entity* ent, float f);

#define ROCKET 2
//function to create a rocket and automatically fill in variables with rocket defaults
Entity prototypeRocket(Node* loc, int team, float direction);

void functionRocket(Entity* ent, float f);


#endif
