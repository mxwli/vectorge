#include "game.h"


Entity* blankEntity(Node* loc, float rad) {
	Entity* ret = malloc(sizeof(Entity));
	memset(ret, 0, sizeof(Entity));
	ret->loc = loc; ret->radius = rad;
	return ret;
}

void entityTick(Entity* ent, float f) {
	pushNode(ent->loc, Vector2Scale(ent->vel, f));
	ent->damage += ent->decayHP * f;
	ent->HP = fclamp(0, ent->maxHP, ent->HP-ent->damage);
	ent->damage = 0;
}
