// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "RenderSystem.h"
#include "IsometricGrid.h"
#include <iostream>

namespace iso= IsometricGrid;
// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	int gGameRunning = 1;

	// Initialization of your own variables go here

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	RenderSystem::Renderer RS{};
	RenderSystem::RenderSetting redTint;
	redTint.tint = { 1,1,1,1 };
	redTint.blendColor = { 1,1,1,1 };
	redTint.transperancy = 1;
	redTint.blendMode = AE_GFX_BM_BLEND;

	//GRID SET UP
	const int gridX{5}, gridY{6};
	//Init a grid with 0 tiles
	iso::cell *grid = {new iso::cell[gridX*gridY]{}};
	s32 mouseX{0},mouseY{0};
	for (int y{ 0 }; y < gridY; ++y) {
		for (int x{ 0 }; x < gridX; ++x) {
			int index = x + gridX*y;
			iso::vec2i ScreenPos = iso::WorldIndexToScreenPos(x,y);
			//*! SET THE POSITION OF THE MESH HERE!!!!
			grid[index].pos = ScreenPos;
		}
	}

	//COLORS
	// RenderSystem::Vec4<float> lightcyan{0.5f,1.0f,1.0f,1.0f};
	// RenderSystem::Vec4<float> yellow{237/255.f,229/255.f,138/255.f};
	// RenderSystem::Vec4<float> green{157/255.f,237/255.f,138/255.f};
	// RenderSystem::Vec4<float> gray{154/255.f,161/255.f,153/255.f};
	// RenderSystem::Vec4<float> purple{168/255.f,96/255.f,168/255.f};
	// RenderSystem::Vec4<float> white{1.f,1.f,1.f};

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

		//MOUSE INPUTS (Tile width = 100, tile height = 50)
		int cellX =  mouseX/100;		
		int cellY =  mouseY/50;		
		int xOffset = mouseX%100;
		int yOffset = mouseY%50;
		//Origin -> screen/tile. For now I use numbers
		int originX = 400/100;
		int originY = 300/50;

		int selectX = (cellX - originX) + (cellY - originY);
		int selectY = (cellY - originY) - (cellX - originX);
		if(iso::isInside(xOffset,yOffset,0,0,0,25,50,0))selectX--;
		//BOTTOM LEFT
		if(iso::isInside(xOffset,yOffset,0,25,0,50,50,50))selectY++;
		//TOP RIGHT
		if(iso::isInside(xOffset,yOffset,50,0,100,0,100,25))selectY--;
		//BOTTOM RIGHT
		if(iso::isInside(xOffset,yOffset,50,50,100,50,100,25))selectX++;
		
		std::cout << "Selected : " << selectX << ", "<< selectY <<'\n';
		
		iso::vec2i MouseToIndex = iso::ScreenPosToIso(mouseX,mouseY);
		// Your own update logic goes here


		// Your own rendering logic goes here
		//Render grid test
		for(int y{0}; y< gridY; ++y){
			for(int x{0}; x< gridX; ++x){
				int index = x+gridX*y;
				switch(grid[index].ID){
					default:
					RS.AddBatch(RenderSystem::TILE_BATCH,RenderSystem::TILE,grid[index].pos.x,grid[index].pos.y);
					break;
				}
			}
		}

		#if 0
		//TILE THING
		RS.AddBatch(RenderSystem::TILE_BATCH, RenderSystem::TILE, 0, 0);

		//EARTH THING
		RS.AddBatch(RenderSystem::BUILDING_BATCH, RenderSystem::RESIDENTIAL_S, 50, 0, 99, RenderSystem::MID, redTint);
		#endif
		RS.Render();

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}


	// free the system
	AESysExit();
}