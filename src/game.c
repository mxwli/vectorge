#include "game.h"
#include "entity.h"

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
void removeWall(WallVector* w, int idx) {
	for(int i = idx+1; i < w->size; i++) {
		w->array[i-1] = w->array[i];
	}
	w->size--;
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
void removeEntity(EntityVector* ents, int idx) {
	for(int i = idx+1; i < ents->size; i++)
		ents->array[i-1] = ents->array[i];
	ents->size--;
}

void tickEnt(Entity* ent, float f) {
	pushNode(ent->loc, Vector2Scale(ent->vel, f));
	ent->damage += ent->decayHP*f;
	ent->HP = fclamp(-1, ent->maxHP, ent->HP - ent->damage);
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


//testing: OK
void purgeEntities(EntityVector* ent) {
	Entity* newEnt = malloc(sizeof(Entity)*ent->cap);
	memset(newEnt, 0, sizeof(Entity)*ent->cap);
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

void purgeWalls(WallVector* walls) {
	Wall* newWalls = malloc(sizeof(Wall)*walls->cap);
	memset(newWalls, 0, sizeof(Wall)*walls->cap);
	int size = 0;
	for(int i = 0; i < walls->size; i++) {
		if(walls->array[i].deletion)
			walls->array[i].loc->deletionflag = 1;
		else newWalls[size++] = walls->array[i];
	}
	for(int i = 0; i < size; i++) walls->array[i] = newWalls[i];
	walls->size = size;
	free(newWalls);
}

//testing: none
bool interCircleCircle(Vector2 a, float radA, Vector2 b, float radB) {
	return Vector2Distance(a, b) <= radA+radB;
}
//testing: none
bool interCircleLine(Vector2 a, float rad, Vector2 x1, Vector2 x2) {
	return Vector2Length(displacement(a, x1, x2)) <= rad;
}
//testing: ... you know
bool interLineLine(Vector2 x1, Vector2 x2, Vector2 y1, Vector2 y2) {
	return intersect(x1, x2, y1, y2);
}
//testing: ...
bool interEntCircle(Entity a, Vector2 b, float radB) {
	return interCircleCircle(localPos(a.loc, -1), localScale(a.loc, -1) * a.radius, b, radB);
}
//testing: 
bool interEntLine(Entity a, Vector2 x1, Vector2 x2) {
	return interCircleLine(localPos(a.loc, -1), localScale(a.loc, -1) * a.radius, x1, x2);
}
//t
bool interEntEnt(Entity a, Entity b) {
	return interCircleCircle(localPos(a.loc, -1), localScale(a.loc, -1) * a.radius,
								localPos(b.loc, -1), localScale(b.loc, -1) * b.radius);
}
bool interEntWall(Entity a, Wall b) {
	bool ret = 0;
	for(int i = 1; !ret && i < b.loc->size; i++) {
		ret = interEntLine(a, localPos(b.loc->children[i], -1), localPos(b.loc->children[i-1], -1));
	}
	return ret;
}

//testing: none
bool interWallLine(Wall a, Vector2 x1, Vector2 x2) {
	bool ret = 0;
	for(int i = 1; !ret && i < a.loc->size; i++) {
		ret = interLineLine(x1, x2, localPos(a.loc->children[i],-1),localPos(a.loc->children[i-1],-1));
	}
	return ret;
}

bool defaultEntScan(Entity e) {return 1;}
bool defaultWallScan(Wall w) {return 1;}

//testing: none
int* scanEntsLine(EntityVector arr, Vector2 x1, Vector2 x2, bool (*scan)(Entity)) {
	int cnt = 0;
	for(int i = 0; i < arr.size; i++)
		if(scan(arr.array[i]) && interEntLine(arr.array[i], x1, x2)) cnt++;
	
	int* ret = malloc((cnt+1)*sizeof(int));
	ret[cnt] = -1;
	cnt = 0;
	for(int i = 0; i < arr.size; i++)
		if(scan(arr.array[i]) && interEntLine(arr.array[i], x1, x2)) ret[cnt++] = i;
	return ret;
}

//testing: none
int* scanWallsLine(WallVector arr, Vector2 x1, Vector2 x2, bool (*scan)(Wall)) {
	int cnt = 0;
	for(int i = 0; i < arr.size; i++)
		if(scan(arr.array[i]) && interWallLine(arr.array[i], x1, x2)) cnt++;
	int* ret = malloc((cnt+1)*sizeof(int));
	ret[cnt] = -1;
	cnt = 0;
	for(int i = 0; i < arr.size; i++)
		if(scan(arr.array[i]) && interWallLine(arr.array[i], x1, x2)) ret[cnt++] = i;
	return ret;
}

Vector2 delimitEntsLine(EntityVector arr, Vector2 x1, Vector2 x2, bool (*scan)(Entity)) {
	x2 = Vector2Subtract(x2, x1);
	float l = 0, h = 1;
	for(int it = 0; it < 15; it++) { //how many times this iterator iterates can be modified
		bool flag = 0;
		for(int i = 0; !flag && i < arr.size; i++)
			if(scan(arr.array[i]) &&
				interEntLine(arr.array[i], x1, Vector2Add(x1, Vector2Scale(x2, (h+l)/2))))
				flag = 1;

		if(flag) {h = (h+l)/2;}
		else {l = (h+l)/2;}
	}
	return Vector2Add(x1, Vector2Scale(x2, h));
}

Vector2 delimitWallsLine(WallVector arr, Vector2 x1, Vector2 x2, bool (*scan)(Wall)) {
	x2 = Vector2Subtract(x2, x1);
	float l = 0, h = 1;
	for(int it = 0; it < 15; it++) { //how many times this iterator iterates can be modified
		bool flag = 0;
		for(int i = 0; !flag && i < arr.size; i++)
			if(scan(arr.array[i]) &&
				interWallLine(arr.array[i], x1, Vector2Add(x1, Vector2Scale(x2, (h+l)/2))))
				flag = 1;

		if(flag) {h = (h+l)/2;}
		else {l = (h+l)/2;}
	}
	return Vector2Add(x1, Vector2Scale(x2, h));
}

void functionEnt(Entity* ent, EntityVector ents, WallVector walls, EntityVector* buffer, float f) {
	switch(ent->entityType) {
		case PLAYER:
			//do nothing, the player receives functionalities externally
			break;
		case SLIME:
			functionSlime(ent, ents, walls, buffer, f);
			break;
		default:
			break;
	}
}

void functionEnts(EntityVector ents, WallVector walls, EntityVector* buffer, float f) {
	for(int i = 0; i < ents.size; i++) {
		functionEnt(ents.array+i, ents, walls, buffer, f);
	}
}
