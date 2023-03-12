#ifndef ADVVEC_H
#define ADVVEC_H
#include <raymath.h>
#include <stdbool.h>

#ifdef USESSHORTFORM
#define _cp Vector2CrossProd
#define _ms Vector2Subtract
#define _dp Vector2DotProduct
#endif

float Vector2CrossProd(Vector2 v1, Vector2 v2);
//calculates the absolute of the cross product of v1 and v2 as if they were 3 dimensional

bool intersect(Vector2 x1, Vector2 x2, Vector2 y1, Vector2 y2);
//returns if x intersects y

double distance(Vector2 x, Vector2 y1, Vector2 y2);
//returns the distance from x to the line y

Vector2 projection(Vector2 A, Vector2 B);
//projection of A onto B

#endif
