#include<stdlib.h>
#include<raylib.h>
#include<math.h>
#include<time.h>

#ifdef PLATFORM_WEB
	#include <emscripten/emscripten.h>
#endif

#include"main.h"
#include"node.h"


int WindowState = MAINMENU;


int main() {

	initializeVars();
	InitWindow(WindowWidth, WindowHeight, "VectorGE");
	SetTargetFPS(TargetFPS);
	
#ifdef PLATFORM_WEB
	emscripten_set_main_loop(drawFrame, 0, 1);
#else
	while(!WindowShouldClose()) drawFrame();
#endif
	
	CloseWindow();
	remNode(headNode);
	return 0;
}

void initializeVars() {
	
}

void drawFrame() {
	BeginDrawing();
	ClearBackground(DARKGRAY);
	DrawFPS(10, 10);

	

	EndDrawing();
}
