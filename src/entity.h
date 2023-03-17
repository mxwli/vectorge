#ifndef ENTITY_H
#define ENTITY_H

#include "game.h"
#include "advvec.h"



#define PLAYER 0


#define SLIME 1
//function to create a slime and automatically fill in variables will slime defaults
Entity prototypeSlime(Node* loc, int team);

void functionSlime(Entity* ent, EntityVector ents, WallVector walls, EntityVector* buffer, float f);



#endif
