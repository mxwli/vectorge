#ifndef MAIN_H
#define MAIN_H

#define WindowWidth 1920
#define WindowHeight 1000
#define TargetFPS 75

#define MAINMENU 0
#define INGAME 1
#define SETTINGS 2
#define PAUSE 3
#define EDITOR 4
#define ENDSCREEN 5

#include "node.h"

void initializeVars();
void destructVars();

void drawFrame();
void drawGame();
void debugTree(Node* n);
Vector2 screnPos(Node* n);


#endif
