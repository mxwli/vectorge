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

int WindowState;

Node* headNode, * wallsNode, * entities, * teams[2], * camera;
Entity* playerWrapper, * friendlyWrapper, * enemyWrapper;
int numFriendlies, numEnemies, friendlyCap, enemyCap;
Wall* wallWrapper;
int numWalls, wallCap;

//skeletal node structure, as defined in the diagram

void initializeVars() {
	WindowState = MAINMENU;
	headNode = newNode(0, 0, 0, 0);
	wallsNode = addChild(headNode);
	entities = addChild(headNode);
	teams[0] = addChild(entities);
	teams[1] = addChild(entities);
	playerWrapper = blankEntity(addChild(addChild(teams[0])), 25);
	camera = addChild(addChild(playerWrapper->loc));
	camera->x = -WindowWidth/2;
	camera->y = -WindowHeight/2;
	friendlyWrapper = playerWrapper; numFriendlies = friendlyCap = 1;
	enemyWrapper = NULL; numEnemies = enemyCap = 0;
	wallWrapper = NULL; numWalls = wallCap = 0;
}

Vector2 screnPos(Node* n) {
	return relativePos(n, camera);
}
double screnScal(Node* n) {
	return relativeScale(n, camera);
}
void drawGame() {
	for(int i = 0; i < numFriendlies; i++) {
		Entity current = friendlyWrapper[i];
		DrawCircleV(screnPos(current.loc), screnScal(current.loc) * current.radius, GREEN);
	}
	
	for(int i = 0; i < numEnemies; i++) {
		Entity current = enemyWrapper[i];
		DrawCircleV(screnPos(current.loc), screnScal(current.loc) * current.radius, RED);
	}

	for(int i = 0; i < numWalls; i++) {
		for(int i2 = 1; i2 < wallWrapper[i].loc->size; i2++) {
			Node* A = wallWrapper[i].loc->children[i2-1];
			Node* B = wallWrapper[i].loc->children[i2];
			DrawLineV(screnPos(A), screnPos(B), WHITE);
		}
		if(wallWrapper[i].loc->size == 1) {
			DrawCircleV(screnPos(wallWrapper[i].loc->children[0]), 1, RED);
		}
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

		if(IsMouseButtonPressed(0)) WindowState = INGAME;
		if(IsKeyPressed(KEY_E)) WindowState = EDITOR;
	}
	else if (WindowState == INGAME) {
		if(IsKeyPressed(KEY_ESCAPE)) WindowState = MAINMENU;

		playerWrapper->vel.x = (IsKeyDown(KEY_D)?150:0)-(IsKeyDown(KEY_A)?150:0);
		playerWrapper->vel.y = (IsKeyDown(KEY_S)?150:0)-(IsKeyDown(KEY_W)?150:0);

		camera->parent->scale /= 1+GetMouseWheelMove()/6.0;
		if(camera->parent->scale > 5) camera->parent->scale = 5;
		float hmove = GetMouseDelta().x/3000;
		camera->parent->rotation += hmove;
		playerWrapper->loc->parent->rotation += hmove;
		SetMousePosition(WindowWidth/2, WindowHeight/2);
		
		normalizeVectors(friendlyWrapper, numFriendlies, wallWrapper, numWalls);
		normalizeVectors(enemyWrapper, numEnemies, wallWrapper, numWalls);

		for(int i = 0; i < numFriendlies; i++) entityTick(friendlyWrapper + i, GetFrameTime());
		for(int i = 0; i < numEnemies; i++) entityTick(enemyWrapper + i, GetFrameTime());
		
		purgeEntities(friendlyWrapper, numFriendlies);
		purgeEntities(enemyWrapper, numEnemies);
		purgeTree(headNode);

		drawGame();

	}
	else if (WindowState == SETTINGS) {
		
	}
	else if (WindowState == PAUSE) {
		
	}
	else if (WindowState == EDITOR) {
		camera->parent->rotation = 0; //dont wanna parse rotation
		if(IsKeyPressed(KEY_ESCAPE)) WindowState = MAINMENU;
		playerWrapper->vel.x = (IsKeyDown(KEY_D)?150:0)-(IsKeyDown(KEY_A)?150:0);
		playerWrapper->vel.y = (IsKeyDown(KEY_S)?150:0)-(IsKeyDown(KEY_W)?150:0);
		camera->parent->scale /= 1+GetMouseWheelMove()/6.0;
		if(camera->parent->scale < 0.2) camera->parent->scale = 0.2;
		pushNode(camera, Vector2Scale(playerWrapper->vel, GetFrameTime()));

		DrawText("Space to create new wall instance\nClick to add wall vertex", 5, 5, 15, WHITE);

		if(IsKeyPressed(KEY_SPACE)) {
			if(numWalls == wallCap) {
				wallCap = wallCap*2+1;
				wallWrapper = realloc(wallWrapper, wallCap*sizeof(Wall));
			}
			memset(wallWrapper+numWalls, 0, sizeof(Wall));
			wallWrapper[numWalls].loc = addChild(wallsNode);
			numWalls++;
		}
		if(IsMouseButtonPressed(0) && numWalls > 0) {
			Node* location = addChild(wallWrapper[numWalls-1].loc);
			Vector2 gamePos = Vector2Scale(GetMousePosition(), camera->parent->scale);
			gamePos = Vector2Add(gamePos, localPos(camera, -1));
			setOffset(location, gamePos);

		}

		drawGame();
	}

	EndDrawing();
}

void destructVars() {
	remNode(headNode);
}
