#include <stdlib.h>
#include <stdio.h>
#include "node.h"



Node* newNode(int x, int y, double r, Node* p) {
	Node* ret = malloc(sizeof(Node));
	ret->x = x; ret->y = y; ret->rotation = r; ret->parent = p; ret->children = NULL;
	ret->size = ret->cap = 0;
	return ret;
}

static void remNode(Node* nde) {
	for(int i = 0; i < nde->size; i++)
		remNode(nde->children[i]);
	free(nde->children);
	free(nde);
}

Node* addChild(Node* node) {
	if(node->size == node->cap)
		node->children = realloc(node->children, 2*node->cap*sizeof(Node*)+1);
	
	int ret = node->size; (node->size)++;
	node->children[ret] = newNode(0, 0, 0, node);

	return node->children[ret];
}

void removeChild(Node* node, Node* child) {
	int idx = -1;
	for(int i = 0; i < node->size; i++) if(node->children[i] == child) {idx = i; break;}
	if(idx == -1) {fprintf(stderr, "error finding child\n"); return;}

	remNode(node->children[idx]);
	for(int i = idx+1; i < node->size; i++) node->children[i-1] = node->children[i];
	node->size -= 1;
}

Vector2 localPos(Node* node, int p) {
	Vector2 ret = {0, 0};
	while(p && node != NULL) {
		Vector2 nw = {node->x, node->y};
		ret = Vector2Add(Vector2Rotate(ret, node->rotation), nw);
		node = node->parent;
		p--;
	}
	return ret;
}

Vector2 relativePos(Node* A, Node* B) {
	return Vector2Subtract(localPos(A, -1), localPos(B, -1));
}

void setOffset(Node* node, Vector2 newpos) {
	node->x = newpos.x;
	node->y = newpos.y;
}

void pushNode(Node* node, Vector2 offset) {
	node->x += offset.x;
	node->y += offset.y;
}

