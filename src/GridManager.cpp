
#include <InputManager.h>
#include <RenderSystem.h>

#include <IsometricGrid.h>
#include <iostream>
#include <MomoMaths.h>

#include <UIManager.h>
#include "GridManager.h"

namespace GridManager {
	namespace iso = IsometricGrid;

	iso::cell* grid;
	const int gridX{ 20 }, gridY{ 20 };

	void storeClickData(Vec2<int> mousePos);

	void Initialize() {
		grid = { new iso::cell[gridX * gridY]{} };
		//GRID SET UP
		//Init a grid with 0 tiles
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);
				iso::vec2i ScreenPos = iso::WorldIndexToScreenPos(x, y);
				// //*! SET THE POSITION OF THE MESH HERE!!!!
				//This is 10 units up so we need to move x index and y index up by 10 units
				ScreenPos.y += (gridY * 50) / 2;		//move the grid up by half its size (20 units / 2 = 10)
				grid[index].pos = ScreenPos;

				//Probably need to redo some of the calc for this, note that x index and y index += same offset because we moved the y up by 10 units
				iso::vec2i test = iso::ScreenPosToIso(ScreenPos.x, ScreenPos.y);
				if ((test.x >= -5 && test.x <= 5) && (test.y >= -5 && test.y <= 5)) {
					grid[index].isRenderable = true;
				}
				if (x > 1 && x < 3) {
					// grid[index].ID = iso::RESIDENTIAL;
					std::cout << "x is " << x << " and iso is " << test.x << '\n';
					// std::cout << "y is " << y << " and iso is " << test.x<<'\n';
				}
			}
		}

		InputManager::onMouseClick.Subscribe(storeClickData);
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
		int index = cellX + gridX * cellY;

		int xOffset = mousePos.x % 100;
		int yOffset = mousePos.y % 50;
		//Origin -> screen/tile. For now I use numbers
		int originX = AEGetWindowWidth() / 2 / 100;
		int originY = AEGetWindowHeight() / 2 / 50;

		// int selectX = (cellX - originX) + (cellY - originY);
		// int selectY = (cellY - originY) - (cellX - originX);

		iso::vec2i SelectedCell{
			(cellX - originX) + (cellY - originY),		//x
			(cellY - originY) - (cellX - originX)		//y
		};
		//TOP LEFT
		if (iso::isInside(xOffset, yOffset, 0, 0, 0, 25, 50, 0))SelectedCell.x--;
		//BOTTOM LEFT
		if (iso::isInside(xOffset, yOffset, 0, 25, 0, 50, 50, 50))SelectedCell.y++;
		//TOP RIGHT
		if (iso::isInside(xOffset, yOffset, 50, 0, 100, 0, 100, 25))SelectedCell.y--;
		//BOTTOM RIGHT
		if (iso::isInside(xOffset, yOffset, 50, 50, 100, 50, 100, 25))SelectedCell.x++;

		//We offset by 10 units x and y because of how iso works. We moved the grid up by 10 units
		if ((((SelectedCell.x + 10) < 0) || ((SelectedCell.x + 10) > gridX)) || ((SelectedCell.y + 10) < 0 || (SelectedCell.y + 10) > gridY)) return;
		index = (SelectedCell.x + 10) + gridX * (SelectedCell.y + 10);
		index = GetIndex(SelectedCell.x + 10, SelectedCell.y + 10);

		//if the cell is water (means it doesn't need to be rendered) we don't allow placement
		if (!grid[index].isRenderable) return;
		//ROTATES BETWEEN BUILDINGS
		switch (grid[index].ID)
		{
		case iso::NONE:
			grid[index].ID = iso::RESIDENTIAL;
			break;
		case iso::RESIDENTIAL:
			grid[index].ID = iso::INDUSTRIAL;
			break;
		case iso::INDUSTRIAL:
			grid[index].ID = iso::COMMERCIAL;
			break;
		case iso::COMMERCIAL:
			grid[index].ID = iso::NATURE;
			break;
		case iso::NATURE:
			grid[index].ID = iso::NONE;
			break;
		}

		//This is where it gets messy sorry
		//MERGE LOGIC
		CheckCellNeighbor(grid, SelectedCell);

	}

	void PrepareTileRenderBatch() {
		// Your own rendering logic goes here
		//Render grid test
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);

				switch (grid[index].ID) {
				case iso::RESIDENTIAL:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::RESIDENTIAL_S, grid[index].pos.x, grid[index].pos.y);
					break;
				case iso::INDUSTRIAL:
					break;
				case iso::COMMERCIAL:
					break;
				case iso::NATURE:
					RenderSystem::AddSpriteBatch(RenderSystem::NATURE_BATCH, RenderSystem::NATURE, TextureManager::NATURE_TREE, grid[index].pos.x, grid[index].pos.y);
					break;
				default:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::RESIDENTIAL_S, grid[index].pos.x, grid[index].pos.y);
					break;
				}
				if (grid[index].isRenderable) RenderSystem::AddSpriteBatch(RenderSystem::TILE_BATCH, RenderSystem::TILE, TextureManager::TILE_TEX, grid[index].pos.x, grid[index].pos.y);
			}
		}
		// UIManager::RenderButton(0, 0, 100, 100, 0, UIManager::GetFont(UIManager::ROBOTO).S, "dawdawdwadwadawdawd", Vec4<float>{1, 1, 0, 1}, Vec3<float>{1, 0, 1});
	}

	void GridManager::CheckCellNeighbor(iso::cell*& grid, iso::vec2i cellIndex)
	{
		//The order to check is CLOCKWISE, so we go NORTH, EAST, SOUTH, WEST
		// //Note that Alpha engine Y minus GOES UP!!!
		int NorthIndex = GetIndex(cellIndex.x, cellIndex.y--);
		int EastIndex = GetIndex(cellIndex.x++, cellIndex.y);
		int SouthIndex = GetIndex(cellIndex.x, cellIndex.y++);
		int WestIndex = GetIndex(cellIndex.x--, cellIndex.y);
		// //NORTH


	}
	int GetIndex(int x, int y)
	{
		return x + gridX * y;
	}
}