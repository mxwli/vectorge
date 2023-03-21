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
#include"entity.h"
#include"editor.h"

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

int WindowState;

Node* headNode, * wallsNode, * entities, * camera, * dummy;
Entity* playerWrapper;
EntityVector entityWrapper, entityBuffer;
WallVector wallWrapper;

//skeletal node structure, as defined in the diagram

void initializeVars() {
	srand(time(0));
	WindowState = MAINMENU;
	headNode = newNode(0, 0, 0, 0);
	wallsNode = addChild(headNode);
	entities = addChild(headNode);
	
	//playerWrapper = blankEntity(addChild(entities), 25);

	playerWrapper = malloc(sizeof(Entity)); *playerWrapper = prototypePlayer(addChild(entities), 0);

	camera = addChild(addChild(playerWrapper->loc));
	camera->x = -WindowWidth/2;
	camera->y = -3*WindowHeight/4;
	dummy = addChild(camera);
	entityWrapper.array = playerWrapper;
	entityWrapper.size = entityWrapper.cap = 1;
}

Vector2 screnPos(Node* n) {
	return relativePos(n, camera);
}
double screnScal(Node* n) {
	return relativeScale(n, camera);
}
void drawGame() {
	DrawFPS(1800, 10);
	for(int i = 0; i < entityWrapper.size; i++) {
		Entity current = entityWrapper.array[i];
		if(current.visible) {
			DrawCircleV(screnPos(current.loc), screnScal(current.loc)*current.radius, current.displayColor);
			float f = (float)current.HP/current.maxHP;
			DrawCircleV(screnPos(current.loc), screnScal(current.loc)*current.radius*f/2, RED);
		}
	}
	
	for(int i = 0; i < wallWrapper.size; i++) {
		for(int i2 = 1; i2 < wallWrapper.array[i].loc->size; i2++) {
			Node* A = wallWrapper.array[i].loc->children[i2-1];
			Node* B = wallWrapper.array[i].loc->children[i2];
			DrawLineV(screnPos(A), screnPos(B), WHITE);
		}
		if(wallWrapper.array[i].loc->size == 1) {
			DrawCircleV(screnPos(wallWrapper.array[i].loc->children[0]), 1, RED);
		}
	}
}

void debugTree(Node* n) {
	if(n->parent != NULL) {
		DrawLineV(screnPos(n), screnPos(n->parent), GRAY);
	}
	for(int i = 0; i < n->size; i++) {
		debugTree(n->children[i]);
	}
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

		if(IsMouseButtonPressed(0)) {
			HideCursor();
			WindowState = INGAME;
			camera->y = -2*WindowHeight/3;
			camera->parent->rotation = 0;
		}
		if(IsKeyPressed(KEY_E)) {
			WindowState = EDITOR;
			camera->y = -WindowHeight/2;
		}
	}
	else if (WindowState == INGAME) {
		if(IsKeyPressed(KEY_ESCAPE)) {
			WindowState = MAINMENU;
			ShowCursor();
		}
		
		functionEnts(entityWrapper, wallWrapper, &entityBuffer, GetFrameTime());
		normalizeEnts(&entityWrapper, &wallWrapper);
		tickEnts(&entityWrapper, GetFrameTime());
		purgeEntities(&entityWrapper);	
		purgeTree(headNode);

		drawGame();
		//debugTree(headNode);
		SetMousePosition(WindowWidth/2, WindowHeight/2);

	}
	else if (WindowState == SETTINGS) {
		
	}
	else if (WindowState == PAUSE) {
		
	}
	else if (WindowState == EDITOR) {
		editorFrame();
	}

	EndDrawing();
}

void destructVars() {
	remNode(headNode);
}
