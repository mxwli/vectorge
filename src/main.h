#ifndef MAIN_H
#define MAIN_H

#define WindowWidth 1900
#define WindowHeight 1000
#define TargetFPS 60

#define MAINMENU 0
#define INGAME 1
#define SETTINGS 2
#define PAUSE 3
#define EDITOR 4

void initializeVars();
void destructVars();

void drawFrame();

#endif
