#define USESSHORTFORM

#include <math.h>
#include <stdlib.h>
#include "advvec.h"

float _cp(Vector2 v1, Vector2 v2) {
	return v1.x*v2.y-v1.y*v2.x;
}

bool intersect(Vector2 x1, Vector2 x2, Vector2 y1, Vector2 y2) {
	float a = _cp(_ms(x2, x1), _ms(y1, x1)),
		b = _cp(_ms(x2, x1), _ms(y2, x1)),
		c = _cp(_ms(y2, y1), _ms(x1, y1)),
		d = _cp(_ms(y2, y1), _ms(x2, y1));
	int A = (a > EPSILON) - (a < EPSILON),
		B = (b > EPSILON) - (b < EPSILON),
		C = (c > EPSILON) - (c < EPSILON),
		D = (d > EPSILON) - (d < EPSILON);
	
	if(!A && !B && !C && !D) {
		//special intersecting case with possibly infinite intersections
		A = _dp(_ms(x2, x1), _ms(y1, x1)) > 0 && _dp(_ms(x1, x2), _ms(y1, x2)) > 0;
		B = _dp(_ms(x2, x1), _ms(y2, x1)) > 0 && _dp(_ms(x1, x2), _ms(y2, x2)) > 0;
		C = _dp(_ms(y2, y1), _ms(x1, y1)) > 0 && _dp(_ms(y1, y2), _ms(x1, y2)) > 0;
		D = _dp(_ms(y2, y1), _ms(x2, y1)) > 0 && _dp(_ms(y1, y2), _ms(x2, y2)) > 0;
		return A || B || C || D;
	}
	else {
		return (A != B) && (C != D);
	}
}

double distance(Vector2 x, Vector2 y1, Vector2 y2) {
	int A = _dp(_ms(y2, y1), _ms(x, y1)) > 0 && _dp(_ms(y1, y2), _ms(x, y2)) > 0;
	if(A) {
		return fabs(_cp(_ms(y2, y1), _ms(x, y1)) / Vector2Distance(y1, y2));
	}
	else {
		float a = Vector2Distance(y1, x), b = Vector2Distance(y2, x);
		if(a < b) return a;
		return b;
	}
}

Vector2 projection(Vector2 A, Vector2 B) {
	float projlength = _dp(A, B) / Vector2Length(B);
	return Vector2Scale(B, projlength/Vector2Length(B));
}

float fclamp(float min, float max, float val) {
	if(val < min) return min;
	if(val > max) return max;
	return val;
}
