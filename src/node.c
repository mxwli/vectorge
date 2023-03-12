#include <stdlib.h>
#include <stdio.h>
#include "node.h"



Node* newNode(int x, int y, double r, Node* p) {
	Node* ret = malloc(sizeof(Node));
	ret->x = x; ret->y = y; ret->rotation = r; ret->scale = 1;
	ret->parent = p; ret->children = NULL;
	ret->size = ret->cap = 0;
	ret->deletionflag = 0;
	return ret;
}

void remNode(Node* nde) {
	for(int i = 0; i < nde->size; i++)
		remNode(nde->children[i]);
	free(nde->children);
	free(nde);
}

Node* addChild(Node* node) {
	Node* ret = newNode(0, 0, 0, 0);
	appendChild(node, ret);
	return ret;
}

void appendChild(Node* node, Node* child) {
	if(node->size == node->cap) {
		node->children = realloc(node->children, 2*node->cap*sizeof(Node*)+1);
		for(int i = node->cap; i < node->cap*2+1; i++) node->children[i] = NULL;
		node->cap = node->cap*2+1;
	}
	
	int ret = node->size; (node->size)++;
	node->children[ret] = child;

	return;
}

void removeChild(Node* node, Node* child) {
	int idx = -1;
	for(int i = 0; i < node->size; i++) if(node->children[i] == child) {idx = i; break;}
	if(idx == -1) {fprintf(stderr, "error finding child\n"); return;}

	remNode(node->children[idx]);
	for(int i = idx+1; i < node->size; i++) node->children[i-1] = node->children[i];
	node->size -= 1;
}

void purgeTree(Node* node) {
	if(node->deletionflag) {
		remNode(node);
	}
	else {
		Node** newSet = malloc(node->cap*sizeof(Node*));
		memset(newSet, 0, node->cap*sizeof(Node*));
		int size = 0;
		for(int i = 0; i < node->size; i++) {
			if(node->children[i]->deletionflag) {
				purgeTree(node->children[i]);
			}
			else {
				newSet[size] = node->children[i];
				size++;
			}
		}
		free(node->children);
		node->children = newSet;
		node->size = size;
	}
}

Vector2 localPos(Node* node, int p) {
	Vector2 ret = {0, 0};
	while(p && node != NULL) {
		
		Vector2 nw = {node->x, node->y};
		double rot = node->rotation, scale = node->scale;
		
		if(scale != 1)
			ret = Vector2Scale(ret, scale);
		if(rot != 0)
			ret = Vector2Rotate(ret, rot);

		ret = Vector2Add(ret, nw);
		node = node->parent;
		p--;
	}
	return ret;
}

double localScale(Node* node, int p) {
	double ret = 1;
	while(p && node != NULL && node->parent != NULL) {
		ret *= node->parent->scale;
		node = node->parent;
	}
	return ret;
}

Vector2 relativePos(Node* A, Node* B) {
	Vector2 posA = localPos(A, -1), posB = localPos(B, -1);
	Vector2 delta = Vector2Subtract(posA, posB);
	delta = Vector2Scale(delta, 1/localScale(B, -1));
	return delta;
}

double relativeScale(Node* A, Node* B) {
	return localScale(A, -1) / localScale(B, -1);
}

void setOffset(Node* node, Vector2 newpos) {
	node->x = newpos.x;
	node->y = newpos.y;
}

void pushNode(Node* node, Vector2 offset) {
	node->x += offset.x;
	node->y += offset.y;
}

