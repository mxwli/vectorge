#include <raylib.h>

#include "entity.h"
#include "node.h"
#include "advvec.h"
#include "fx.h"

extern EntityVector entityWrapper, entityBuffer;
extern WallVector wallWrapper;
extern EffectVector effectWrapper, effectBuffer;
extern Node* headNode, * wallsNode, * entities, * effects, * camera, * dummy;

//common things an entity might do
bool scanForEntities(Entity* ent, bool (*scan)(Entity), float angle) {
	Vector2 origin = localPos(ent->loc, -1);
	Vector2 dest = {10000, 0};
	dest = Vector2Rotate(dest, angle);
	dest = Vector2Add(origin, dest);
	dest = delimitWallsLine(wallWrapper, origin, dest, defaultWallScan);
	int* hits = scanEntsLine(entityWrapper, origin, dest, scan);
	bool ret = hits[0] != -1;
	free(hits);
	return ret;
}


//player functionalities
Entity prototypePlayer(Node* loc, int team) {
	Entity ret; memset(&ret, 0, sizeof(Entity));
	ret.loc = loc;
	ret.radius = 25;
	ret.entityType = PLAYER;
	ret.team = team;
	ret.maxHP = ret.HP = 100;
	ret.visible = 1;
	ret.displayColor = SKYBLUE;
	return ret;
}

void functionPlayer(Entity* player, float f) {
	Node* camera = player->loc->children[0]->children[0];
	player->vel.x = (IsKeyDown(KEY_D)?250:0)-(IsKeyDown(KEY_A)?250:0);
	player->vel.y = (IsKeyDown(KEY_S)?250:0)-(IsKeyDown(KEY_W)?250:0);
	player->vel = Vector2Rotate(player->vel, camera->parent->rotation);

	camera->parent->scale /= 1+GetMouseWheelMove()/6.0;
	if(camera->parent->scale > 5) camera->parent->scale = 5;

	float hmove = GetMouseDelta().x/750;
	camera->parent->rotation += hmove;

	if(IsMouseButtonDown(0)) {
		Entity rocket = prototypeRocket(addChild(entities), player->team, camera->parent->rotation);
		setOffset(rocket.loc, localPos(player->loc, -1));
		pushEntity(
			&entityBuffer, rocket
			);
	}
}


//slime functionalities
Entity slimeSelf; //since the scanning function isn't passed the scanner entity, we use global vars
bool slimeScan(Entity e) {
	return e.team != slimeSelf.team;
}
Entity prototypeSlime(Node* loc, int team) {
	Entity ret; memset(&ret, 0, sizeof(Entity));
	ret.loc = loc;
	ret.radius = 50;
	ret.entityType = SLIME;
	ret.team = team;
	ret.maxHP = ret.HP = 100;
	ret.visible = 1;
	ret.displayColor = DARKGREEN;
	return ret;
}

void functionSlime(Entity* ent, float f) {
	slimeSelf = *ent;
	ent->vel = Vector2Zero();
	for(int i = -7; i <= 7; i++)
		if(scanForEntities(ent, slimeScan, ent->lookDirection+0.1*i)) {
			ent->vel.x = 100, ent->vel.y = 0;
			ent->vel = Vector2Rotate(ent->vel, ent->lookDirection+0.1*i);
			ent->lookDirection += 0.1*i;
			break;
		}
	if(ent->vel.x == 0 && ent->vel.y == 0) {
		ent->lookDirection = 1.618 * rand();
	}
}


//rocket functionalities
Entity rocketSelf;
bool rocketScan(Entity e) {
	return e.team != rocketSelf.team;
}
Entity prototypeRocket(Node* loc, int team, float direction) {
	Entity ret; memset(&ret, 0, sizeof(Entity));
	ret.loc = loc;
	ret.radius = 5;
	ret.entityType=ROCKET;
	ret.team = team;
	ret.maxHP = ret.HP = 10;
	ret.decayHP = 0.5;
	ret.visible = 1;
	ret.displayColor = RED;
	ret.vel.x = 0; ret.vel.y = -500;
	ret.vel = Vector2Rotate(ret.vel, direction);
	return ret;
}

void functionRocket(Entity* ent, float f) {
	rocketSelf = *ent;
	Effect trace = blankEffect();
	trace.lifetime = 0.5;
	trace.a = addChild(effects);
	setOffset(trace.a, localPos(ent->loc, -1));
	trace.radius = 2;
	trace.display = ent->displayColor;
	trace.display.a = 100;
	pushEffect(&effectBuffer, trace);

	int* ents = scanEntsCircle(entityWrapper, localPos(ent->loc, -1), ent->radius, rocketScan);
	int* walls = scanWallsCircle(wallWrapper, localPos(ent->loc, -1), ent->radius, defaultWallScan);
	if(ents[0] != -1 || walls[0] != -1) {
		ent->HP = 0;
		for(int i = 0; ents[i] != -1; i++) entityWrapper.array[ents[i]].damage += 10;
	}
	free(ents);
	free(walls);
}
