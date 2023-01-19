
#include <InputManager.h>
#include <RenderSystem.h>

#include <IsometricGrid.h>
#include <iostream>

namespace GridManager {
	namespace iso = IsometricGrid;

	iso::cell* grid;
	const int gridX{ 5 }, gridY{ 6 };

	void Initialize() {
		grid = { new iso::cell[gridX * gridY]{} };
		//GRID SET UP
		//Init a grid with 0 tiles
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				// iso::vec2i testPos{(x*100)-400,(y*-50)+300};
				int index = x + gridX * y;
				// grid[index].pos = testPos;
				iso::vec2i ScreenPos = iso::WorldIndexToScreenPos(x, y);
				// //*! SET THE POSITION OF THE MESH HERE!!!!
				grid[index].pos = ScreenPos;
			}
		}
	}

	void ScreenToTilePos() {
		//MOUSE INPUTS (Tile width = 100, tile height = 50)
		int cellX = InputManager::mousePos.x / 100;
		int cellY = InputManager::mousePos.y / 50;

		int xOffset = InputManager::mousePos.x % 100;
		int yOffset = InputManager::mousePos.y % 50;
		//Origin -> screen/tile. For now I use numbers
		int originX = 400 / 100;
		int originY = 300 / 50;

		int selectX = (cellX - originX) + (cellY - originY);
		int selectY = (cellY - originY) - (cellX - originX);

		//TOP LEFT
		if (iso::isInside(xOffset, yOffset, 0, 0, 0, 25, 50, 0))selectX--;
		//BOTTOM LEFT
		if (iso::isInside(xOffset, yOffset, 0, 25, 0, 50, 50, 50))selectY++;
		//TOP RIGHT
		if (iso::isInside(xOffset, yOffset, 50, 0, 100, 0, 100, 25))selectY--;
		//BOTTOM RIGHT
		if (iso::isInside(xOffset, yOffset, 50, 50, 100, 50, 100, 25))selectX++;

		std::cout << "Selected : " << selectX << ", " << selectY << '\n';
	}

	void PrepareTileRenderBatch() {
		// Your own rendering logic goes here
		//Render grid test
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = x + gridX * y;
				switch (grid[index].ID) {
				default:
					RenderSystem::AddBatch(RenderSystem::TILE_BATCH, RenderSystem::TILE, grid[index].pos.x, grid[index].pos.y);
					break;
				}
			}
		}
	}
}