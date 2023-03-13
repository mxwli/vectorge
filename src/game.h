#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"
#include "advvec.h"

typedef struct WallStruct Wall;

struct WallStruct {
	Node* loc;
	//the children of loc are the verticies that make up the wall

//wall data
	
};

Wall* blankWall(Node* loc);

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

void normalizeEnt(Entity* ent, Wall* walls, int wallsize);

void normalizeVectors(Entity* ent, int entsize, Wall* walls, int wallsize);
//adjusts entity velocities so they don't go through walls
//there may be a better name for this

int purgeEntities(Entity* ent, int entsize);
//removes noninvincible entities and marks their nodes for deletion
//returns the resulting array length
//be sure to purge the node tree afterwards

//TODO ADD INTERACTIONS

#endif
