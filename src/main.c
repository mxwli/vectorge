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

Node* headNode;
Node* tail;
Node* camera;

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
	return 0;
}

void initializeVars() {
	headNode = newNode(0, 0, 0, 0);
	srand(time(0));
	
	camera = addChild(headNode);
	camera->x = -WindowWidth/2;
	camera->y = -WindowHeight/2;

	Node* ptr = headNode;
	for(int i = 0; i < 10; i++) {
		ptr = addChild(ptr);
		ptr->x = 75;
	}
	tail = ptr;
}

void drawFrame() {
	BeginDrawing();
	ClearBackground(DARKGRAY);
	DrawFPS(10, 10);
	
	Node* ptr = tail;
	while(ptr != headNode && ptr != NULL) {
		DrawLineV(relativePos(ptr, camera), relativePos(ptr->parent, camera), WHITE);
		ptr->rotation += 0.1*GetFrameTime();
		ptr->x = 75*sin(GetTime());
		ptr = ptr->parent;
	}
	
	EndDrawing();
}
