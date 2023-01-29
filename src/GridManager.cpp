
#include <InputManager.h>
#include <RenderSystem.h>

#include <IsometricGrid.h>
#include <iostream>
#include <MomoMaths.h>

#include <UIManager.h>

namespace GridManager {
	namespace iso = IsometricGrid;

	iso::cell* grid;
	const int gridX{ 20 }, gridY{ 20 };
	AEGfxTexture* tex;;

	void storeClickData(Vec2<int> mousePos);

	void Initialize() {
		grid = { new iso::cell[gridX * gridY]{} };
		//GRID SET UP
		//Init a grid with 0 tiles
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = x + gridX * y;
				iso::vec2i ScreenPos = iso::WorldIndexToScreenPos(x, y);
				// //*! SET THE POSITION OF THE MESH HERE!!!!
				ScreenPos.y += (gridY * 50) / 2;		//move the grid up by half its size
				grid[index].pos = ScreenPos;

				//TODO Fix this!! it's very wonky calculations for now
				iso::vec2i test = iso::ScreenPosToIso(ScreenPos.x, ScreenPos.y);
				if ((test.x >= -5 && test.x <= 5) && (test.y >= -5 && test.y <= 5)) {
					// std::cout << "x is " << x << " and iso is " << test.x<<'\n';
					// std::cout << "y is " << y << " and iso is " << test.y<<'\n';
					grid[index].isRenderable = true;
				}
			}
		}

		InputManager::onMouseClick.Subscribe(storeClickData);
		tex = AEGfxTextureLoad("Assets/tree_test.png");
	}

	void UpdateMouseToGrid() {
		// //MOUSE INPUTS (Tile width = 100, tile height = 50)
		// int cellX = InputManager::mousePos.x / 100;
		// int cellY = InputManager::mousePos.y / 50;

		// int xOffset = InputManager::mousePos.x % 100;
		// int yOffset = InputManager::mousePos.y % 50;
		// //Origin -> screen/tile. For now I use numbers
		// int originX = AEGetWindowWidth()/2 / 100;
		// int originY = AEGetWindowHeight()/2 / 50;

		// int selectX = (cellX - originX) + (cellY - originY);
		// int selectY = (cellY - originY) - (cellX - originX);

		// //TOP LEFT
		// if (iso::isInside(xOffset, yOffset, 0, 0, 0, 25, 50, 0))selectX--;
		// //BOTTOM LEFT
		// if (iso::isInside(xOffset, yOffset, 0, 25, 0, 50, 50, 50))selectY++;
		// //TOP RIGHT
		// if (iso::isInside(xOffset, yOffset, 50, 0, 100, 0, 100, 25))selectY--;
		// //BOTTOM RIGHT
		// if (iso::isInside(xOffset, yOffset, 50, 50, 100, 50, 100, 25))selectX++;
		// // iso::vec2i selectedTile{selectX,selectY};
		// // return selectedTile;
		// std::cout << "Selected : " << selectX << ", " << selectY << '\n';

	}

	void storeClickData(Vec2<int> mousePos) {
		//MOUSE INPUTS (Tile width = 100, tile height = 50)
		int cellX = mousePos.x / 100;
		int cellY = mousePos.y / 50;

		int xOffset = mousePos.x % 100;
		int yOffset = mousePos.y % 50;
		//Origin -> screen/tile. For now I use numbers
		int originX = AEGetWindowWidth() / 2 / 100;
		int originY = AEGetWindowHeight() / 2 / 50;

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
		// iso::vec2i selectedTile{selectX,selectY};
		// return selectedTile;
		std::cout << "Selected : " << selectX << ", " << selectY << '\n';
	}

	void PrepareTileRenderBatch() {
		// Your own rendering logic goes here
		//Render grid test
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = x + gridX * y;

				switch (grid[index].ID) {
				case iso::RESIDENTIAL:
					break;
				case iso::INDUSTRIAL:
					break;
				case iso::COMMERCIAL:
					break;
				case iso::NATURE:
					RenderSystem::AddSpriteBatch(RenderSystem::NATURE_BATCH, RenderSystem::NATURE, grid[index].pos.x, grid[index].pos.y);
					break;
				default:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::RESIDENTIAL_S, grid[index].pos.x, grid[index].pos.y);
					break;
				}
				if (grid[index].isRenderable) RenderSystem::AddSpriteBatch(RenderSystem::TILE_BATCH, RenderSystem::TILE, grid[index].pos.x, grid[index].pos.y);
			}
		}
	}
}