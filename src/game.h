#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"
#include "advvec.h"

typedef struct Node2D Wall;
//A wall is a node whose children make up its vertecies.

typedef struct EntityStruct Entity;

struct EntityStruct {
	Node* loc;
	Vector2 vel; //velocity, affected by parent rotation
	float radius; //radius of entity - all entities are circles

//entity data

	int entityType, team;
	//classification data


	float maxHP, HP, decayHP, damage;
	int invincible;
	//hit point values
	

	int visible;
	Color displaycolor;
	//display information

	
	
};

Entity* blankEntity(Node* loc, float rad);

void entityTick(Entity* ent, float f);
//updates entity one tick. call this after normalizevectors!!

void normalizeVectors(Entity* ent, int entsize, Wall* walls, int wallsize);
//adjusts entity velocities so they don't go through walls

#endif
