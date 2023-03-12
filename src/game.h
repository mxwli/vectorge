#ifndef GAME_H
#define GAME_H

#include "node.h"
#include "advvec.h"
#include <raylib.h>
#include <raymath.h>

typedef struct Node2D Wall;
//A wall is a node whose children make up its vertecies.

typedef struct EntityStruct Entity;

struct EntityStruct {
	Node* loc;
	Vector2 vel; //velocity, affected by parent rotation
	double radius; //radius of entity - all entities are circles

//entity data

	int entityType, team;
	//classification data


	double maxHP, HP, decayHP;
	int invincible;
	//hit point values
	

	int visible;
	Color displaycolor;
	//display information

	
	
};

void entityTick(Entity* ent);
//updates entity one tick. call this after normalizevectors!!

void normalizeVectors(Entity* ent, int entsize, Wall* walls, int wallsize);
//adjusts entity velocities so they don't go through walls

#endif
