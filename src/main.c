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
Node* tailA, *tailB, *tailC;
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
	remNode(headNode);
	return 0;
}

void initializeVars() {
	headNode = newNode(0, 0, 0, 0);
	srand(time(0));
	
	camera = addChild(addChild(headNode));
	camera->x = -WindowWidth/2;
	camera->y = -WindowHeight/2;

	Node* ptr = headNode;
	for(int i = 0; i < 10; i++) {
		ptr = addChild(ptr);
		ptr->x = 75;
	}
	tailA = ptr;
	ptr = headNode;
	for(int i = 0; i < 10; i++) {
		ptr = addChild(ptr);
		if(i == 0) ptr->rotation = 2*PI/3;
		ptr->x = 75;
	}
	tailB = ptr;
	ptr = headNode;
	for(int i = 0; i < 10; i++) {
		ptr = addChild(ptr);
		if(i == 0) ptr->rotation = 4*PI/3;
		ptr->x = 75;
	}
	tailC = ptr;
}

void drawFrame() {
	BeginDrawing();
	ClearBackground(DARKGRAY);
	DrawFPS(10, 10);

	Node* ptr = tailA;
	while(ptr != headNode) {
		DrawLineV(relativePos(ptr, camera), relativePos(ptr->parent, camera), WHITE);
		DrawCircleV(relativePos(ptr, camera), 3, RED);
		ptr->rotation += GetFrameTime();
		ptr->scale += 0.01*GetFrameTime();
		ptr = ptr->parent;
	}
	ptr = tailB;
	while(ptr != headNode) {
		DrawLineV(relativePos(ptr, camera), relativePos(ptr->parent, camera), WHITE);
		DrawCircleV(relativePos(ptr, camera), 3, RED);
		ptr->rotation += GetFrameTime();
		ptr->scale += 0.01*GetFrameTime();
		ptr = ptr->parent;
	}
	ptr = tailC;
	while(ptr != headNode) {
		DrawLineV(relativePos(ptr, camera), relativePos(ptr->parent, camera), WHITE);
		DrawCircleV(relativePos(ptr, camera), 3, RED);
		ptr->rotation += GetFrameTime();
		ptr->scale += 0.01*GetFrameTime();
		ptr = ptr->parent;
	}

	//input
	camera->parent->scale /= 1+GetMouseWheelMove()/10;
	if(camera->parent->scale < 0.2) camera->parent->scale = 0.2;
	
	EndDrawing();
}
