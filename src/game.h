#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "node.h"
#include "advvec.h"
#include "fx.h"

//WALL DEFINITION
typedef struct WallStruct Wall;
struct WallStruct {
	Node* loc;
	//the children of loc are the verticies that make up the wall

//wall data
	bool deletion;
};
Wall blankWall(Node* loc);

//VECTOR OF WALLS DEFINITION
typedef struct WallVect WallVector;
struct WallVect {
	Wall* array;
	int size, cap;
	//can be thought of as a C++ std::vector<Wall>
};
WallVector blankWallVector();

void pushWall(WallVector* w, Wall val);
void removeWall(WallVector* w, int idx);


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
	Color displayColor;
	//display information

	float lookDirection;
	//misc data, not necessarily used by all entities
};
Entity blankEntity(Node* loc, float rad);

//VECTOR OF ENTITIES DEFINITION
typedef struct EntityVect EntityVector;
struct EntityVect {
	Entity* array;
	int size, cap;
	//can be thought of as a C++ std::vector<Entity>
};
EntityVector blankEntityVector();

void pushEntity(EntityVector* e, Entity val);
void removeEntity(EntityVector* ents, int idx);


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
void purgeWalls(WallVector* walls);
//removes walls that are marked for deletion, and then marks their nodes for deletion
//be sure to purge the node tree afterwards

//TODO ADD INTERACTIONS

//intersection function naming convention: inter[a][b]
//where a, b are one of {Ent, Wall, Circle, Line}

bool interCircleCircle(Vector2 a, float radA, Vector2 b, float radB);	//uses advvec functions
bool interCircleLine(Vector2 a, float rad, Vector2 x1, Vector2 x2);		//uses advvec functions
bool interLineLine(Vector2 x1, Vector2 x2, Vector2 y1, Vector2 y2);		//uses advvec functions
bool interEntCircle(Entity a, Vector2 b, float radB);					//uses intercirclecircle
bool interEntLine(Entity a, Vector2 x1, Vector2 x2);					//uses intercircleline
bool interEntEnt(Entity a, Entity b);									//uses intercirclecircle
bool interEntWall(Entity a, Wall b);									//uses intercircleline
bool interWallLine(Wall a, Vector2 x1, Vector2 x2);						//uses interlineline

//the following functions test for intersections in a list of items
//they return a list of integers pointing to the intersected indecies, terminating with -1
//there is also a parameter that takes in an entity/wall and returns if it's valid to be intersecting with.

bool defaultEntScan(Entity e);
bool defaultWallScan(Wall w);

int* scanEntsLine(EntityVector arr, Vector2 x1, Vector2 x2, bool (*scan)(Entity));
//scans an array of entities using a line
int* scanWallsLine(WallVector arr, Vector2 x1, Vector2 x2, bool (*scan)(Wall));
//scans an array of walls using a line

//REMEMBER TO DEALLOCATE THE RETURNED MEMORY

//currently the implementation of the following functions are TERRIBLE
//that's because they binary search how long the line can travel before they encounter an intersection point.
//running time is nlogn
Vector2 delimitEntsLine(EntityVector arr, Vector2 x1, Vector2 x2, bool (*scan)(Entity));
//finds closest intersection point and returns it.
Vector2 delimitWallsLine(WallVector arr, Vector2 x1, Vector2 x2, bool (*scan)(Wall));
//finds closest intersection point and returns it.




void functionEnt(Entity* ent, float f);
//performs entity logic
//the parameters for these two functions are the entity/array of entities, the array of walls, and the entitybuffer, which is an "output" array for entities to spawn new entities into
void functionEnts (EntityVector ents, float f);

/*
things an entity can do:
- spawn other entities
	- best case implementation is one which can spawn any entity anywhere with any configuration of parameters
	- due to node implementation, adding new nodes should be easy
- use rays to scan the area - DONE
	- these rays are able to select between different teams and toggle walls - DONE
- modify other entity's health - SHOULD BE DONE
*/



#endif
