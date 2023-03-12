#include <raymath.h>

typedef struct Node2D Node;

struct Node2D {
	int x, y; //offset from parent
	double rotation, scale; //rotation & scale of the node. only affects children
	struct Node2D* parent; //pointer to parent
	struct Node2D** children; //pointer to array of children
	int size, cap; //size/cap of children array
};

Node* newNode(int x, int y, double r, Node* p);

Node* addChild(Node* node);
//adds child node to given node and returns its location
void removeChild(Node* node, Node* child);
//removes child

Vector2 localPos(Node* node, int p);
//local position of a node relative to its p th ancestor
//set p=-1 for position relative to global context

double localScale(Node* node, int p);
//local scale of a node relative to its p th ancestor
//set p=-1 for scale relative to global context

Vector2 relativePos(Node* A, Node* B);
//position of A relative to B, factoring scales into consideration

double relativeScale(Node* A, Node* B);
//scale of A relative to B

void setOffset(Node* node, Vector2 newpos);

void pushNode(Node* node, Vector2 offset);


