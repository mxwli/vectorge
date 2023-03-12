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
	destructVars();

	return 0;
}

void initializeVars() {
	
}

void drawFrame() {
	BeginDrawing();
	ClearBackground(DARKGRAY);
	
	if(WindowState == MAINMENU) {
		char* welcome = "VectorGE";
		int fontsize = 200;
		int width = MeasureText(welcome, fontsize);
		DrawText(welcome, WindowWidth/2-width/2, WindowHeight/2-300, fontsize, WHITE);

		DrawText("Click anywhere.", WindowWidth/2-width/2, WindowHeight/2, 50, WHITE);

		
	}
	else if (WindowState == INGAME) {
		
	}
	else if (WindowState == SETTINGS) {
		
	}
	else if (WindowState == PAUSE) {
		
	}
	else if (WindowState == EDITOR) {
		
	}

	EndDrawing();
}

void destructVars() {
	
}
