#include<stdlib.h>
#include<raylib.h>

#ifdef PLATFORM_WEB
	#include <emscripten/emscripten.h>
#endif

#include"main.h"


static int WindowState = MAINMENU;

static void DrawFrame() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	
	
	
	EndDrawing();
}

int main() {
	InitWindow(WindowWidth, WindowHeight, "VectorGE");
	SetTargetFPS(TargetFPS);
	
#ifdef PLATFORM_WEB
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	while(!WindowShouldClose()) DrawFrame();
#endif
	
	CloseWindow();
	return 0;
}
