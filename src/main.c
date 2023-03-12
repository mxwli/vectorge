#include<stdlib.h>
#include<raylib.h>
#include<math.h>
#include<time.h>

#ifdef PLATFORM_WEB
	#include <emscripten/emscripten.h>
#endif

#include"main.h"
#include"node.h"
#include"game.h"
#include"advvec.h"

int main() {

	
	InitWindow(WindowWidth, WindowHeight, "VectorGE");
	SetTargetFPS(TargetFPS);
	SetExitKey(KEY_NULL);
	initializeVars();
	
#ifdef PLATFORM_WEB
	emscripten_set_main_loop(drawFrame, 0, 1);
#else
	while(!WindowShouldClose()) drawFrame();
#endif
	
	CloseWindow();
	destructVars();

	return 0;
}

int WindowState = MAINMENU;

Node* headNode, * wallsNode, * entities, * teams[2], * camera;
Entity* playerWrapper, * friendlyWrapper, * enemyWrapper;
int numFriendlies, 
Wall* wallWrapper;

//skeletal node structure, as defined in the diagram

void initializeVars() {
	headNode = newNode(0, 0, 0, 0);
	wallsNode = addChild(headNode);
	entities = addChild(headNode);
	teams[0] = addChild(entities);
	teams[1] = addChild(entities);
	playerWrapper = blankEntity(addChild(teams[0]), 25);
	camera = addChild(addChild(playerWrapper->loc));
	camera->x = -WindowWidth/2;
	camera->y = -WindowHeight/2;
	
}

void drawFrame() {
	BeginDrawing();
	ClearBackground(DARKGRAY);
	
	if(WindowState == MAINMENU) {
		char* welcome = "VectorGE";
		int fontsize = 200;
		int width = MeasureText(welcome, fontsize);
		DrawText(welcome, WindowWidth/2-width/2, WindowHeight/2-300, fontsize, SKYBLUE);
		DrawText("Click anywhere.", WindowWidth/2-width/2, WindowHeight/2, 50, WHITE);
		DrawText("E for Editor", WindowWidth/2-width/2, WindowHeight/2+75, 25, WHITE);

		if(IsMouseButtonPressed(0)) WindowState = INGAME;
		if(IsKeyPressed(KEY_E)) WindowState = EDITOR;
	}
	else if (WindowState == INGAME) {
		if(IsKeyPressed(KEY_ESCAPE)) WindowState = MAINMENU;
		
	}
	else if (WindowState == SETTINGS) {

	}
	else if (WindowState == PAUSE) {
		
	}
	else if (WindowState == EDITOR) {
		if(IsKeyPressed(KEY_ESCAPE)) WindowState = MAINMENU;
		
	}

	EndDrawing();
}

void destructVars() {
	remNode(headNode);
}
