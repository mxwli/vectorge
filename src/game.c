#include "game.h"

void entityTick(Entity* ent, float f) {
	pushNode(ent->loc, Vector2Scale(ent->vel, f));
	
}
