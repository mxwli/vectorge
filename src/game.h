#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"
#include "advvec.h"

//WALL DEFINITION
typedef struct WallStruct Wall;
struct WallStruct {
	Node* loc;
	//the children of loc are the verticies that make up the wall

//wall data
	
};
Wall* blankWall(Node* loc);

//VECTOR OF WALLS DEFINITION
typedef struct WallVect WallVector;
struct WallVect {
	Wall* array;
	int size, cap;
	//can be thought of as a C++ std::vector<Wall>
};
WallVector* blankWallVector();

void pushWall(WallVector* w, Wall val);


//ENTITY DEFINITION
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

//VECTOR OF ENTITIES DEFINITION
typedef struct EntityVect EntityVector;
struct EntityVect {
	Entity* array;
	int size, cap;
	//can be thought of as a C++ std::vector<Entity>
};
EntityVector* blankEntityVector();

void pushEntity(EntityVector* e, Entity val);


//-----------INTERACTIONS BETWEEN DIFFERENT ENTITIES DEFINED BELOW-------------------\\

void tickEnt(Entity* ent, float f);
//updates entity one tick. call this after normalizeents!!

void tickEnts(EntityVector* ent, float f);
//updates all the entities in ent by one tick. Call this after normalizeents!

void normalizeEnt(Entity* ent, WallVector* walls);

void normalizeEnts(EntityVector* ent, WallVector* walls);
//adjusts entity velocities so they don't go through walls
//there may be a better name for this


void purgeEntities(EntityVector* ent);
//removes noninvincible entities and marks their nodes for deletion
//be sure to purge the node tree afterwards

//TODO ADD INTERACTIONS



/*
things an entity can do:
- spawn other entities
	- best case implementation is one which can spawn any entity anywhere with any configuration of parameters
	- due to node implementation, adding new nodes should be easy
- use rays to scan the area
	- these rays are able to select between different teams and toggle walls
- modify other entity's health
*/



#endif
