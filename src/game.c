#include "game.h"

//testing: good
Wall* blankWall(Node* loc) {
	Wall* ret = malloc(sizeof(Wall));
	memset(ret, 0, sizeof(Wall));
	ret->loc = loc;
	return ret;
}

WallVector* blankWallVector() {
	WallVector* ret = malloc(sizeof(WallVector));
	memset(ret, 0, sizeof(WallVector));
	return ret;
}

void pushWall(WallVector* walls, Wall val) {
	if(walls->size == walls->cap) {
		walls->array = realloc(walls->array, (2*walls->cap+1)*sizeof(Wall));
		walls->cap = 2*walls->cap + 1;
	}
	walls->array[walls->size++] = val;
}

//testing: good
Entity* blankEntity(Node* loc, float rad) {
	Entity* ret = malloc(sizeof(Entity));
	memset(ret, 0, sizeof(Entity));
	ret->loc = loc; ret->radius = rad;
	return ret;
}

EntityVector* blankEntityVector() {
	EntityVector* ret = malloc(sizeof(EntityVector));
	memset(ret, 0, sizeof(EntityVector));
	return ret;
}

void pushEntity(EntityVector* ents, Entity val) {
	if(ents->size == ents->cap) {
		ents->array = realloc(ents->array, (2*ents->cap+1)*sizeof(Entity));
		ents->cap = 2*ents->cap+1;
	}
	ents->array[ents->size++] = val;
}

void tickEnt(Entity* ent, float f) {
	pushNode(ent->loc, Vector2Scale(ent->vel, f));
	ent->damage += ent->decayHP*f;
	ent->HP = fclamp(0, ent->maxHP, ent->HP - ent->damage);
	ent->damage = 0;
}

void tickEnts(EntityVector* ent, float f) {
	for(int i = 0; i < ent->size; i++) {
		tickEnt(ent->array+i, f);
	}
}

void normalizeEnt(Entity* ent, WallVector* walls) {
	int left = 0, right = 0;
	for(int i = 0; i < walls->size; i++) {
		Node* loc = walls->array[i].loc;
		for(int i2 = 1; i2 < loc->size; i2++) {
			Vector2 A = localPos(loc->children[i2], -1);
			Vector2 B = localPos(loc->children[i2-1], -1);
			Vector2 dis = displacement(localPos(ent->loc, -1), A, B);

			if(Vector2Length(dis) <= ent->radius && Vector2DotProduct(ent->vel, dis) > 0) {
				if(Vector2CrossProd(ent->vel, dis) < 0) left = 1;
				else right = 1;
				ent->vel = projectionNorm(ent->vel, dis);
			}
		}
	}
	if(left && right) ent->vel = Vector2Zero();

}

void normalizeEnts(EntityVector* ent, WallVector* walls) {
	for(int i = 0; i < ent->size; i++) {
		normalizeEnt(ent->array+i, walls);
	}
}


//testing: none
void purgeEntities(EntityVector* ent) {
	Entity* newEnt = malloc(sizeof(Entity)*ent->cap);
	memset(newEnt, 0, sizeof newEnt);
	int size = 0;
	for(int i = 0; i < ent->size; i++) {
		if(!ent->array[i].invincible && ent->array[i].HP <= 0)
			ent->array[i].loc->deletionflag = 1;
		else newEnt[size++] = ent->array[i];
	}
	for(int i = 0; i < size; i++) ent->array[i] = newEnt[i];
	ent->size = size;
	free(newEnt);
}
