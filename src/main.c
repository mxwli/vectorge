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
			camera->y = -3*WindowHeight/4;
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

		playerWrapper->vel.x = (IsKeyDown(KEY_D)?150:0)-(IsKeyDown(KEY_A)?150:0);
		playerWrapper->vel.y = (IsKeyDown(KEY_S)?150:0)-(IsKeyDown(KEY_W)?150:0);
		playerWrapper->vel = Vector2Rotate(playerWrapper->vel, camera->parent->rotation);

		camera->parent->scale /= 1+GetMouseWheelMove()/6.0;
		if(camera->parent->scale > 5) camera->parent->scale = 5;

		float hmove = GetMouseDelta().x/750;
		camera->parent->rotation += hmove;
		SetMousePosition(WindowWidth/2, WindowHeight/2);

		
		functionEnts(entityWrapper, wallWrapper, &entityBuffer, GetFrameTime());
		normalizeEnts(&entityWrapper, &wallWrapper);
		tickEnts(&entityWrapper, GetFrameTime());
		purgeEntities(&entityWrapper);	
		purgeTree(headNode);

		drawGame();
		debugTree(headNode);
	}
	else if (WindowState == SETTINGS) {
		
	}
	else if (WindowState == PAUSE) {
		
	}
	else if (WindowState == EDITOR) {
		if(IsKeyPressed(KEY_ESCAPE)) WindowState = MAINMENU;
		playerWrapper->vel.x = (IsKeyDown(KEY_D)?900:0)-(IsKeyDown(KEY_A)?900:0);
		playerWrapper->vel.y = (IsKeyDown(KEY_S)?900:0)-(IsKeyDown(KEY_W)?900:0);
		camera->parent->scale /= 1+GetMouseWheelMove()/6.0;
		if(camera->parent->scale < 0.2) camera->parent->scale = 0.2;
		pushNode(playerWrapper->loc, Vector2Scale(playerWrapper->vel, GetFrameTime()));

		DrawText("Space to create new wall instance\nClick to add wall vertex\nShift to close shape\nRight click to remove walls", 5, 5, 15, WHITE);

		if(IsKeyPressed(KEY_SPACE)) {
			Wall newWall;
			memset(&newWall, 0, sizeof(Wall));
			newWall.loc = addChild(wallsNode);
			pushWall(&wallWrapper, newWall);
			
//			if(numWalls == wallCap) {
//				wallCap = wallCap*2+1;
//				wallWrapper = realloc(wallWrapper, wallCap*sizeof(Wall));
//			}
//			memset(wallWrapper+numWalls, 0, sizeof(Wall));
//			wallWrapper.array[numWalls].loc = addChild(wallsNode);
//			numWalls++;
		}
		if(IsMouseButtonPressed(0) && wallWrapper.size > 0) {
			Node* location = addChild(wallWrapper.array[wallWrapper.size-1].loc);
			setOffset(dummy, GetMousePosition());
			setOffset(location, localPos(dummy, -1));
		}
		if(IsKeyPressed(KEY_LEFT_SHIFT)) {
			Node* location = addChild(wallWrapper.array[wallWrapper.size-1].loc);
			setOffset(location, localPos(wallWrapper.array[wallWrapper.size-1].loc->children[0], 1));
		}
		if(IsKeyPressed(KEY_ONE)) {
			Node* newNode = addChild(entities);
			setOffset(dummy, GetMousePosition());
			setOffset(newNode, localPos(dummy, -1));
			pushEntity(&entityWrapper, prototypeSlime(newNode, 1));
			playerWrapper = entityWrapper.array;
		}

		drawGame();
	}

	EndDrawing();
}

void destructVars() {
	remNode(headNode);
}
