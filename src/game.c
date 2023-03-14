#include "game.h"

//testing: good
Wall* blankWall(Node* loc) {
	Wall* ret = malloc(sizeof(Wall));
	memset(ret, 0, sizeof(Wall));
	ret->loc = loc;
	return ret;
}

//testing: good
Entity* blankEntity(Node* loc, float rad) {
	Entity* ret = malloc(sizeof(Entity));
	memset(ret, 0, sizeof(Entity));
	ret->loc = loc; ret->radius = rad;
	return ret;
}

//testing: none
void entityTick(Entity* ent, float f) {
	pushNode(ent->loc, Vector2Scale(ent->vel, f));
	ent->damage += ent->decayHP * f;
	ent->HP = fclamp(0, ent->maxHP, ent->HP-ent->damage);
	ent->damage = 0;
}

//testing: OK
void normalizeEnt(Entity* ent, Wall* walls, int wallsize) {
	int left = 0, right = 0;
	//ent->vel = Vector2Rotate(ent->vel, localRotation(ent->loc, -1));
	for(int i = 0; i < wallsize; i++) {
		Node* loc = walls[i].loc;
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
	//ent->vel = Vector2Rotate(ent->vel, -localRotation(ent->loc, -1));
}

//testing: OK
void normalizeVectors(Entity* ent, int entsize, Wall* walls, int wallsize) {
	for(int i = 0; i < entsize; i++) {
		normalizeEnt(ent + i, walls, wallsize);
	}
}

//testing: none
int purgeEntities(Entity* ent, int entsize) {
	Entity* newEnt = malloc(sizeof(Entity) * entsize);
	memset(newEnt, 0, sizeof newEnt);
	int size = 0;
	for(int i = 0; i < entsize; i++) {
		if(!ent[i].invincible && ent[i].HP <= 0)
			ent[i].loc->deletionflag = 1;
		else newEnt[size++] = ent[i];
	}
	for(int i = 0; i < size; i++) ent[i] = newEnt[i];
	free(newEnt);
	return size;
}
