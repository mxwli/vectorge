#ifndef FX_H
#define FX_H

#include<stdlib.h>
#include<string.h>
#include<raylib.h>
#include<raymath.h>
#include "node.h"

#define RAYTYPE 0





typedef struct EffectStruct Effect;
struct EffectStruct {
	float lifetime, timer, radius; //lifetime of ray before it's removed, internal clock,
									//thickness of drawing particle
	Node* a, * b; //beginning node of ray, ending node of ray
	
	Color display; //display color of effect
	float (*transcurve)(float); //function defining the transparency curve wrt time
	float (*scalecurve)(float); //function defining the scale curve wrt time
};

Effect blankEffect();


typedef struct EffectStructVector EffectVector;
struct EffectStructVector {
	Effect* array;
	int size, cap;
};

EffectVector blankEffectVector();

void pushEffect(EffectVector* e, Effect nw);

void drawEffect(Effect* e, float f, Node* camera);

void drawEffects(EffectVector fx, float f, Node* camera);

void purgeEffects(EffectVector* fx);


#endif
