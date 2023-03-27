#include <raylib.h>

#include "editor.h"
#include "node.h"
#include "game.h"
#include "main.h"
#include "entity.h"

#define ADDWALLS 0
#define DELETE 1
#define ADDSLIME 2

extern int WindowState;
extern Node* headNode, * wallsNode, * entities, * camera, * dummy;
extern EntityVector entityWrapper, entityBuffer;
extern WallVector wallWrapper;

int EditingMode;

void editorFrame() {

	static Vector2 deletionDragBegin, deletionDragEnd;

	if(IsKeyPressed(KEY_ESCAPE)) WindowState = MAINMENU;
	Entity* playerWrapper = entityWrapper.array;
	playerWrapper->vel.x = (IsKeyDown(KEY_D)?900:0)-(IsKeyDown(KEY_A)?900:0);
	playerWrapper->vel.y = (IsKeyDown(KEY_S)?900:0)-(IsKeyDown(KEY_W)?900:0);
	camera->parent->scale /= 1+GetMouseWheelMove()/6.0;
	if(camera->parent->scale < 0.2) camera->parent->scale = 0.2;
	pushNode(playerWrapper->loc, Vector2Scale(playerWrapper->vel, GetFrameTime()));
	
	if(IsKeyPressed(KEY_ZERO)) EditingMode = ADDWALLS;
	if(IsKeyPressed(KEY_ONE)) EditingMode = DELETE;
	if(IsKeyPressed(KEY_TWO)) EditingMode = ADDSLIME;
	
	switch(EditingMode) {
		case ADDWALLS:
			DrawText(
				"EditingMode: Adding Walls\nSpace to create new wall isntance\nClick to add wall vertex\nLShift to close loop"
				, 10, 10, 25, WHITE);
			if(IsKeyPressed(KEY_SPACE)) {
				Wall newWall;
				memset(&newWall, 0, sizeof(Wall));
				newWall.loc = addChild(wallsNode);
				pushWall(&wallWrapper, newWall);
			}
			if(IsMouseButtonPressed(0) && wallWrapper.size > 0) {
				Node* location = addChild(wallWrapper.array[wallWrapper.size-1].loc);
				setOffset(dummy, GetMousePosition());
				setOffset(location, localPos(dummy, -1));
			}
			if(IsKeyPressed(KEY_LEFT_SHIFT) && wallWrapper.size > 0 && wallWrapper.array[0].loc->size > 0) {
				Node* location = addChild(wallWrapper.array[wallWrapper.size-1].loc);
				setOffset(location, localPos(wallWrapper.array[wallWrapper.size-1].loc->children[0], 1));
			}

			break;
		case DELETE:
			DrawText("EditingMode: Deletion", 10, 10, 25, WHITE);
			if(IsMouseButtonPressed(0)) {
				setOffset(dummy, GetMousePosition());
				deletionDragBegin = localPos(dummy, -1);
			}
			if(IsMouseButtonReleased(0)) {
				setOffset(dummy, GetMousePosition());
				deletionDragEnd = localPos(dummy, -1);
				if(deletionDragBegin.x != deletionDragEnd.x || deletionDragEnd.y != deletionDragBegin.y) {
					int* entarr = scanEntsLine(entityWrapper, deletionDragBegin, deletionDragEnd, defaultEntScan);
					int* wallarr = scanWallsLine(wallWrapper, deletionDragBegin, deletionDragEnd, defaultWallScan);
					for(int i = 0; entarr[i] != -1; i++)
						entityWrapper.array[entarr[i]].HP = -1;
					for(int i = 0; wallarr[i] != -1; i++)
						wallWrapper.array[wallarr[i]].deletion = 1;
					free(entarr);
					free(wallarr);
					purgeEntities(&entityWrapper);
					purgeWalls(&wallWrapper);
					purgeTree(headNode);
				}
			}
			break;
		case ADDSLIME:
			DrawText("Editing Mode: Adding Slime", 10, 10, 25, WHITE);
			if(IsMouseButtonPressed(0)) {
				Node* newNode = addChild(entities);
				setOffset(dummy, GetMousePosition());
				setOffset(newNode, localPos(dummy, -1));
				pushEntity(&entityWrapper, prototypeSlime(newNode, 1));
			}
			break;
		default:
			DrawText("Error: Unknown Editing Mode", 10, 10, 25, RED);
	}


	drawGame();
	//debugTree(headNode);
}
