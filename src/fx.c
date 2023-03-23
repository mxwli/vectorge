#include "fx.h"


Effect blankEffect() {
	Effect ret; memset(&ret, 0, sizeof(Effect));
	return ret;
}


EffectVector blankEffectVector() {
	EffectVector ret; memset(&ret, 0, sizeof(EffectVector));
	return ret;
}

void pushEffect(EffectVector* e, Effect nw) {
	if(e->size == e->cap) {
		e->array = realloc(e->array, (2*e->cap+1)*sizeof(Effect));
		e->cap = 2*e->cap+1;
	}
	e->array[e->size] = nw;
	e->size++;
}

void drawEffect(Effect* e, float f, Node* camera) {
	float opacity = (e->transcurve != NULL? e->transcurve(e->timer): 1);
	Color realColor = e->display; realColor.a *= opacity;
	float scale = (e->scalecurve != NULL? e->scalecurve(e->timer): 1);
	float realRadius = e->radius * scale * relativeScale(e->a, camera);
	if(e->b == NULL) {
		DrawCircleV(relativePos(e->a, camera), realRadius, realColor);
	}
	else {
		DrawLineEx(relativePos(e->a, camera), relativePos(e->b, camera), realRadius, realColor);
	}
}

void drawEffects(EffectVector fx, float f, Node* camera) {
	for(int i = 0; i < fx.size; i++) {
		drawEffect(fx.array + i, f, camera);
		fx.array[i].timer += f;
	}
}

void purgeEffects(EffectVector* fx) {
	Effect* newArr = malloc(fx->cap*sizeof(Effect));
	int size = 0;
	for(int i = 0; i < fx->size; i++) {
		if(fx->array[i].timer < fx->array[i].lifetime) {
			newArr[size++] = fx->array[i];
		}
		else {
			fx->array[i].a->deletionflag = 1;
			if(fx->array[i].b != NULL) fx->array[i].b->deletionflag = 1;
		}
	}
	fx->size = size;
	for(int i = 0; i < size; i++) fx->array[i] = newArr[i];
	free(newArr);
}
