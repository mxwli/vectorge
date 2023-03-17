#include "entity.h"
#include "node.h"
#include "advvec.h"


//slime functionalities
Entity slimeSelf; //since the scanning function isn't passed the scanner entity, we use global vars
bool slimeScan(Entity e) {
	return e.team != slimeSelf.team;
}
Entity prototypeSlime(Node* loc, int team) {
	Entity ret; memset(&ret, 0, sizeof(Entity));
	ret.loc = loc;
	ret.radius = 10;
	ret.entityType = SLIME;
	ret.team = team;
	ret.maxHP = ret.HP = 100;
	ret.visible = 1;
	return ret;
}
void functionSlime(Entity* ent, EntityVector ents, WallVector walls, EntityVector* buffer, float f) {
	slimeSelf = *ent;
	Vector2 origin = localPos(ent->loc, -1);
	Vector2 dest = {10000, 0};
	dest = Vector2Rotate(dest, ent->lookDirection);
	dest = Vector2Add(origin, dest);
	dest = delimitWallsLine(walls, origin, dest, defaultWallScan);

	int* hits = scanEntsLine(ents, origin, dest, slimeScan);

	if(hits[0] != -1) {
		ent->vel.x = 100, ent->vel.y = 0;
		ent->vel = Vector2Rotate(ent->vel, ent->lookDirection);
	}
	else {
		ent->vel = Vector2Zero();
		ent->lookDirection += .05;
	}
	free(hits);
}

