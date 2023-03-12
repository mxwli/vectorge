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

	initializeVars();
	InitWindow(WindowWidth, WindowHeight, "VectorGE");
	SetTargetFPS(TargetFPS);
	SetExitKey(KEY_NULL);
	
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

Node* headNode, * wall, * player;
Entity* playerWrapper;
Wall* wallWrapper;

void initializeVars() {
	headNode = newNode(0, 0, 0, 0);
	wall = addChild(headNode);
	player = addChild(headNode);
	playerWrapper = blankEntity(player, 25);
	wallWrapper = blankWall(wall);
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
		
		for(int i = 1; i < wall->size; i++) {
			DrawLineV(localPos(wall->children[i-1], -1), localPos(wall->children[i], -1), WHITE);
		}
		DrawCircleV(localPos(playerWrapper->loc, -1), playerWrapper->radius, GREEN);
		playerWrapper->vel.x = (IsKeyDown(KEY_D)?250:0)-(IsKeyDown(KEY_A)?250:0);
		playerWrapper->vel.y = (IsKeyDown(KEY_S)?250:0)-(IsKeyDown(KEY_W)?250:0);
		
		normalizeVectors(playerWrapper, 1, wallWrapper, 1);
		entityTick(playerWrapper, GetFrameTime());
	}
	else if (WindowState == SETTINGS) {

	}
	else if (WindowState == PAUSE) {
		
	}
	else if (WindowState == EDITOR) {
		if(IsKeyPressed(KEY_ESCAPE)) WindowState = MAINMENU;
		if(IsMouseButtonPressed(0)) {
			Node* tmp = addChild(wall);
			setOffset(tmp, GetMousePosition());
		}
		if(IsMouseButtonPressed(1)) {
			setOffset(player, GetMousePosition());
		}
		for(int i = 1; i < wall->size; i++) {
			DrawLineV(localPos(wall->children[i-1], -1), localPos(wall->children[i], -1), WHITE);
		}
		DrawCircleV(localPos(playerWrapper->loc, -1), playerWrapper->radius, GREEN);
	}

	EndDrawing();
}

void destructVars() {
	remNode(headNode);
}
