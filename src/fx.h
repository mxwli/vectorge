#ifndef FX_H
#define FX_H

#include<raylib.h>
#include<raymath.h>

#define RAYTYPE 0


typedef struct EffectStruct Effect;
struct EffectStruct {
	int type;
	float lifetime;
	Vector2 a, b;
};


#endif
