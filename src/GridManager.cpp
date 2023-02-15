/*!************************************************************************
\file:          GridManager.cpp
\author:		Amadeus Chia
\par DP email:	amadeusjinhan.chia@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This source file implements functions that manage the game grid.
This includes the merge logic and the placement of buildings

The functions include:
-
**************************************************************************/

#include <InputManager.h>
#include <RenderSystem.h>

#include <IsometricGrid.h>
#include <iostream>
#include <MomoMaths.h>

#include <UIManager.h>
#include <GridManager.h>
#include <PauseManager.h>

namespace GridManager {
	namespace iso = IsometricGrid;

	int synergyPoints{ 0 };
	int terrainNum{ 0 };

	iso::cell* grid;
	const int gridX{ 20 }, gridY{ 20 };

	void storeClickData();

	int randomNature{ 0 };

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
				// iso::vec2i test = iso::ScreenPosToIso(ScreenPos.x, ScreenPos.y);
				// if ((test.x >= -5 && test.x <= 5) && (test.y >= -5 && test.y <= 5)) {
				// 	grid[index].isRenderable = true;
				// }
				//basically we want the grid to be from -2 to 2, but since there's a 10 unit offset, we add 10
				if ((x >= 8 && x <= 13) && (y >= 8 && y <= 13)) {
					// grid[index].ID = iso::RESIDENTIAL;
					grid[index].isRenderable = true;
					// std::cout << "y is " << y << " and iso is " << test.x<<'\n';
				}
			}
		}

		// Hardcoded island parts
		// Top left


		InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, storeClickData);
		InputManager::SubscribeToKey(AEVK_C, InputManager::TRIGGERED, ClearGrid);
		InputManager::SubscribeToKey(AEVK_R, InputManager::TRIGGERED, RandomiseTerrain);
		InputManager::SubscribeToKey(AEVK_1, InputManager::PRESSED, SpawnResidential);
		InputManager::SubscribeToKey(AEVK_2, InputManager::TRIGGERED, SpawnCommerical);
		InputManager::SubscribeToKey(AEVK_3, InputManager::TRIGGERED, SpawnIndustrial);
		InputManager::SubscribeToKey(AEVK_N, InputManager::TRIGGERED, SpawnNature);
	}


	bool isCellSafe(iso::vec2i selectedCell) {
		if ((((selectedCell.x) < 0) || ((selectedCell.x) > gridX)) || ((selectedCell.y) < 0 || (selectedCell.y) > gridY)) return false;
		if (!grid[GetIndex(selectedCell.x, selectedCell.y)].isRenderable) return false;
		return true;
	}

	void SpawnResidential() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		iso::vec2i SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;

		grid[index].ID = iso::RESIDENTIAL;
		CheckCellNeighbor(grid, SelectedCell);

		Vec2<int> test1{ 1,1 };
		Vec2<int> test11{ 2,2 };
		Vec3<int> test2{ 1,1,1 };
		Vec3<int> test22{ 2,2,2 };
		Vec4<int> test3{ 1,1,1,1 };
		Vec4<int> test33{ 2,2,2,2 };

		std::cout << "TEST 1 (vec+=vec)\n";

		std::cout << (test11 += test1) << " = (3,3)" << '\n';
		std::cout << (test22 += test2) << " = (3,3,3)" << '\n';
		std::cout << (test33 += test3) << " = (3,3,3,3)" << '\n';

		std::cout << "TEST 2 (vec-=vec)\n";
		std::cout << (test11 -= test1) << " = (2,2)" << '\n';
		std::cout << (test22 -= test2) << " = (2,2,2)" << '\n';
		std::cout << (test33 -= test3) << " = (2,2,2,2)" << '\n';

		std::cout << "TEST 3 (vec-vec higher)\n";
		std::cout << test33 - test1 << " = (1,1,2,2)" << '\n';
		std::cout << test33 - test2 << " = (1,1,1,2)" << '\n';
		std::cout << test33 - test3 << " = (1,1,1,1)" << '\n';
		std::cout << test22 - test1 << " = (1,1,2)" << '\n';
		std::cout << test22 - test2 << " = (1,1,1)" << '\n';

		// std::cout <<"TEST 4 (vec-vec lower)\n";
		// std::cout << test1-test33 << " = (1,1,2,2)"<< '\n';
		// std::cout << test2-test33 << " = (1,1,1,2)"<<'\n';
		// std::cout << test3-test33 << " = (1,1,1,1)"<< '\n';
		// std::cout << test1-test22 << " = (1,1,2)"<< '\n';
		// std::cout << test2-test22 << " = (1,1,1)"<< '\n';

		std::cout << "TEST 5 (vec+vec)\n";
		std::cout << test1 + test33 << " = (3,3,2,2)" << '\n';
		std::cout << test2 + test33 << " = (3,3,3,2)" << '\n';
		std::cout << test3 + test33 << " = (3,3,3,3)" << '\n';
		std::cout << test1 + test22 << " = (3,3,2)" << '\n';
		std::cout << test2 + test22 << " = (3,3,3)" << '\n';

		std::cout << "TEST 6 (vec*=vec)\n";
		test1 *= 4;
		test2 *= 4;
		test3 *= 4;
		std::cout << test1 << " = (4,4)" << '\n';
		std::cout << test2 << " = (4,4,4)" << '\n';
		std::cout << test3 << " = (4,4,4,4)" << '\n';

		std::cout << "TEST 7 (vec*T)\n";
		std::cout << test11 * 2 << " = (4,4)" << '\n';
		std::cout << test22 * 2 << " = (4,4,4)" << '\n';
		std::cout << test33 * 2 << " = (4,4,4,4)" << '\n';

	}
	void SpawnCommerical() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		iso::vec2i SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;

		grid[index].ID = iso::COMMERCIAL;
		CheckCellNeighbor(grid, SelectedCell);
	}
	void SpawnIndustrial() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		iso::vec2i SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;

		grid[index].ID = iso::INDUSTRIAL;
		CheckCellNeighbor(grid, SelectedCell);
	}
	void SpawnNature() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		iso::vec2i SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;
		randomNature = rand() % 2;
		if (randomNature % 2 == 0)
			grid[index].ID = iso::TREE;
		else
			grid[index].ID = iso::ROCK;
	}
	void RandomiseTerrain() {
		if (PauseManager::IsPaused()) return;

		/*
		TOP LEFT = x--;
		TOP RIGHT = y--;
		BOTTOM LEFT = y++;
		BOTTOM RIGHT = x++;
		*/
		ClearGrid();
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);
				if ((x >= 8 && x <= 13) && (y >= 8 && y <= 13)) {
					grid[index].isRenderable = true;
				}
				else grid[index].isRenderable = false;
			}
		}
		// std::cout << terrainNum << '\n';
		switch (terrainNum)
		{
		case 0:
			//Top Left
			grid[GetIndex(7, 9)].isRenderable = true;
			grid[GetIndex(7, 10)].isRenderable = true;
			grid[GetIndex(7, 11)].isRenderable = true;
			grid[GetIndex(7, 12)].isRenderable = true;
			grid[GetIndex(6, 11)].isRenderable = true;
			grid[GetIndex(6, 12)].isRenderable = true;
			// Bottom left
			grid[GetIndex(9, 16)].isRenderable = true;
			grid[GetIndex(9, 15)].isRenderable = true;
			grid[GetIndex(9, 14)].isRenderable = true;
			grid[GetIndex(10, 14)].isRenderable = true;
			grid[GetIndex(10, 15)].isRenderable = true;
			grid[GetIndex(11, 14)].isRenderable = true;
			grid[GetIndex(12, 14)].isRenderable = true;
			grid[GetIndex(12, 15)].isRenderable = true;
			grid[GetIndex(10, 15)].isRenderable = true;
			grid[GetIndex(11, 15)].isRenderable = true;
			// Bottom right
			grid[GetIndex(14, 9)].isRenderable = true;
			grid[GetIndex(14, 10)].isRenderable = true;
			grid[GetIndex(14, 11)].isRenderable = true;
			grid[GetIndex(14, 12)].isRenderable = true;
			grid[GetIndex(15, 11)].isRenderable = true;
			grid[GetIndex(15, 12)].isRenderable = true;
			// Top right
			grid[GetIndex(10, 7)].isRenderable = true;
			grid[GetIndex(11, 7)].isRenderable = true;
			grid[GetIndex(12, 7)].isRenderable = true;
			grid[GetIndex(11, 6)].isRenderable = true;
			grid[GetIndex(12, 6)].isRenderable = true;
			terrainNum++;
			break;
		case 1:
			//Top Left
			grid[GetIndex(7, 9)].isRenderable = true;
			grid[GetIndex(7, 10)].isRenderable = true;
			grid[GetIndex(7, 11)].isRenderable = true;
			grid[GetIndex(7, 12)].isRenderable = true;
			grid[GetIndex(6, 11)].isRenderable = true;
			grid[GetIndex(6, 12)].isRenderable = true;
			grid[GetIndex(6, 13)].isRenderable = true;
			grid[GetIndex(6, 14)].isRenderable = true;
			grid[GetIndex(7, 13)].isRenderable = true;
			grid[GetIndex(7, 14)].isRenderable = true;
			grid[GetIndex(8, 14)].isRenderable = true;
			// Bottom left
			grid[GetIndex(9, 13)].isRenderable = true;
			grid[GetIndex(9, 14)].isRenderable = true;
			grid[GetIndex(11, 14)].isRenderable = true;
			grid[GetIndex(11, 15)].isRenderable = true;
			grid[GetIndex(12, 14)].isRenderable = true;
			grid[GetIndex(12, 15)].isRenderable = true;
			grid[GetIndex(10, 15)].isRenderable = true;
			grid[GetIndex(11, 15)].isRenderable = true;
			grid[GetIndex(11, 16)].isRenderable = true;
			// Bottom right
			grid[GetIndex(14, 9)].isRenderable = true;
			grid[GetIndex(14, 11)].isRenderable = true;
			grid[GetIndex(14, 12)].isRenderable = true;
			grid[GetIndex(15, 11)].isRenderable = true;
			grid[GetIndex(15, 12)].isRenderable = true;
			grid[GetIndex(15, 9)].isRenderable = true;
			// Top right
			grid[GetIndex(11, 8)].isRenderable = true;
			grid[GetIndex(12, 8)].isRenderable = true;
			grid[GetIndex(12, 7)].isRenderable = true;
			grid[GetIndex(11, 6)].isRenderable = true;
			grid[GetIndex(12, 6)].isRenderable = true;
			terrainNum++;
			break;
		case 2:
			//mini island
			grid[GetIndex(16, 3)].isRenderable = true;
			grid[GetIndex(16, 4)].isRenderable = true;
			grid[GetIndex(15, 4)].isRenderable = true;
			grid[GetIndex(15, 5)].isRenderable = true;
			grid[GetIndex(14, 5)].isRenderable = true;
			grid[GetIndex(14, 4)].isRenderable = true;

			grid[GetIndex(8, 7)].isRenderable = true;
			grid[GetIndex(7, 7)].isRenderable = true;
			grid[GetIndex(6, 7)].isRenderable = true;
			grid[GetIndex(8, 6)].isRenderable = true;
			grid[GetIndex(7, 6)].isRenderable = true;
			grid[GetIndex(6, 5)].isRenderable = true;
			grid[GetIndex(6, 7)].isRenderable = true;
			grid[GetIndex(5, 7)].isRenderable = true;
			grid[GetIndex(5, 8)].isRenderable = true;
			grid[GetIndex(6, 8)].isRenderable = true;
			grid[GetIndex(7, 8)].isRenderable = true;
			grid[GetIndex(4, 8)].isRenderable = true;
			grid[GetIndex(3, 8)].isRenderable = true;
			grid[GetIndex(7, 8)].isRenderable = true;
			grid[GetIndex(7, 9)].isRenderable = true;
			grid[GetIndex(7, 10)].isRenderable = true;
			grid[GetIndex(7, 11)].isRenderable = true;
			grid[GetIndex(7, 12)].isRenderable = true;
			grid[GetIndex(7, 13)].isRenderable = true;
			grid[GetIndex(6, 13)].isRenderable = true;
			grid[GetIndex(6, 12)].isRenderable = true;
			grid[GetIndex(6, 11)].isRenderable = true;
			grid[GetIndex(6, 10)].isRenderable = true;
			grid[GetIndex(6, 9)].isRenderable = true;
			grid[GetIndex(6, 8)].isRenderable = true;
			grid[GetIndex(5, 8)].isRenderable = true;
			grid[GetIndex(5, 9)].isRenderable = true;
			grid[GetIndex(5, 10)].isRenderable = true;
			grid[GetIndex(5, 11)].isRenderable = true;
			grid[GetIndex(5, 12)].isRenderable = true;
			grid[GetIndex(5, 13)].isRenderable = true;
			grid[GetIndex(4, 11)].isRenderable = true;
			grid[GetIndex(4, 10)].isRenderable = true;
			grid[GetIndex(4, 12)].isRenderable = true;
			grid[GetIndex(9, 7)].isRenderable = true;
			grid[GetIndex(10, 7)].isRenderable = true;
			grid[GetIndex(11, 7)].isRenderable = true;
			grid[GetIndex(12, 7)].isRenderable = true;
			grid[GetIndex(13, 14)].isRenderable = true;
			grid[GetIndex(12, 14)].isRenderable = true;
			grid[GetIndex(11, 14)].isRenderable = true;
			grid[GetIndex(10, 14)].isRenderable = true;
			grid[GetIndex(9, 14)].isRenderable = true;
			grid[GetIndex(8, 14)].isRenderable = true;
			grid[GetIndex(7, 14)].isRenderable = true;
			grid[GetIndex(7, 15)].isRenderable = true;
			grid[GetIndex(8, 15)].isRenderable = true;
			grid[GetIndex(9, 15)].isRenderable = true;
			grid[GetIndex(14, 10)].isRenderable = true;
			grid[GetIndex(14, 11)].isRenderable = true;
			grid[GetIndex(14, 12)].isRenderable = true;
			grid[GetIndex(14, 13)].isRenderable = true;
			grid[GetIndex(15, 13)].isRenderable = true;
			grid[GetIndex(15, 12)].isRenderable = true;
			terrainNum++;
			break;
		case 3:
			for (int y{ 0 }; y < gridY; ++y) {
				for (int x{ 0 }; x < gridX; ++x) {
					int index = GetIndex(x, y);
					if ((x >= 8 && x <= 13) && (y >= 8 && y <= 13)) {
						grid[index].isRenderable = true;
					}
					else grid[index].isRenderable = false;
				}
			}
			terrainNum = 0;
			break;
		}
	}

	void UpdateMouseToGrid() {
		if (PauseManager::IsPaused()) return;

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

	void storeClickData() {
		if (PauseManager::IsPaused()) return;

		Vec2<int> mousePos = InputManager::GetMousePos();
		//Convert the mouse position into iso
		iso::vec2i SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		//We offset by 10 units x and y because of how iso works. We moved the grid up by 10 units
		if ((((SelectedCell.x) < 0) || ((SelectedCell.x) > gridX)) || ((SelectedCell.y) < 0 || (SelectedCell.y) > gridY)) return;
		int index = (SelectedCell.x) + gridX * (SelectedCell.y);
		index = GetIndex(SelectedCell.x, SelectedCell.y);

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
			randomNature = rand() % 2;
			if (randomNature % 2 == 0)
				grid[index].ID = iso::TREE;
			else
				grid[index].ID = iso::ROCK;
			break;
		case iso::ROCK:
			grid[index].ID = iso::NONE;
			break;
		case iso::TREE:
			grid[index].ID = iso::NONE;
			break;
		}

		//This is where it gets messy sorry
		//MERGE LOGIC
		CheckCellNeighbor(grid, SelectedCell);

	}

	void ClearGrid() {
		if (PauseManager::IsPaused()) return;

		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);
				grid[index].ID = iso::NONE;
			}
		}
	}

	void PrepareTileRenderBatch() {
		// Your own rendering logic goes here
		//Render grid test
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);

				switch (grid[index].ID) {
					//RESIDENTIAL
				case iso::RESIDENTIAL + SMALL:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::RESIDENTIAL_1X1_L1, grid[index].pos.x, grid[index].pos.y);
					break;
				case iso::RESIDENTIAL + MEDIUM:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::RESIDENTIAL_1X1_L2, grid[index].pos.x, grid[index].pos.y);
					break;
				case iso::RESIDENTIAL + LARGE:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::RESIDENTIAL_1X1_L3, grid[index].pos.x, grid[index].pos.y);
					break;

					//COMMERCIAL
				case iso::COMMERCIAL + SMALL:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::COMMERCIAL_1X1_L1, grid[index].pos.x, grid[index].pos.y);
					break;
				case iso::COMMERCIAL + MEDIUM:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::COMMERCIAL_1X1_L2, grid[index].pos.x, grid[index].pos.y);
					break;
				case iso::COMMERCIAL + LARGE:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::COMMERCIAL_1X1_L3, grid[index].pos.x, grid[index].pos.y);
					break;

					//INDUSTRIAL
				case iso::INDUSTRIAL + SMALL:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::INDUSTRIAL_1X1_L1, grid[index].pos.x, grid[index].pos.y);
					break;
				case iso::INDUSTRIAL + MEDIUM:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::INDUSTRIAL_1X1_L2, grid[index].pos.x, grid[index].pos.y);
					break;
				case iso::INDUSTRIAL + LARGE:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::INDUSTRIAL_1X1_L3, grid[index].pos.x, grid[index].pos.y);
					break;

					//NATURE
				case iso::TREE:
					RenderSystem::AddSpriteBatch(RenderSystem::NATURE_BATCH, RenderSystem::NATURE, TextureManager::NATURE_TREE, grid[index].pos.x, grid[index].pos.y);
					break;
				case iso::ROCK:
					RenderSystem::AddSpriteBatch(RenderSystem::NATURE_BATCH, RenderSystem::NATURE, TextureManager::NATURE_ROCK, grid[index].pos.x, grid[index].pos.y);
					break;

				default:
					// RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::RESIDENTIAL_S, grid[index].pos.x, grid[index].pos.y);
					break;
				}
				if (grid[index].isRenderable) RenderSystem::AddSpriteBatch(RenderSystem::TILE_BATCH, RenderSystem::TILE, TextureManager::TILE_TEX, grid[index].pos.x, grid[index].pos.y);
			}
		}
		// UIManager::RenderButton(0, 0, 100, 100, 0, UIManager::GetFont(UIManager::ROBOTO).S, "dawdawdwadwadawdawd", Vec4<float>{1, 1, 0, 1}, Vec3<float>{1, 0, 1});
	}

	void GridManager::CheckCellNeighbor(iso::cell* grid, iso::vec2i cellIndex)
	{
		//The order to check is CLOCKWISE, so we go NORTH, EAST, SOUTH, WEST
		int matchCount{ 0 };
		int matchedCells[2];
		int gridIndex = GetIndex(cellIndex.x, cellIndex.y);
		int NorthIndex = GetIndex(cellIndex.x, cellIndex.y - 1);
		int EastIndex = GetIndex(cellIndex.x + 1, cellIndex.y);
		int SouthIndex = GetIndex(cellIndex.x, cellIndex.y + 1);
		int WestIndex = GetIndex(cellIndex.x - 1, cellIndex.y);
		//NORTH
#if DEBUG
		std::cout << "Index : " << cellIndex.x << ", " << cellIndex.y << '\n';
		std::cout << "Selected : " << grid[gridIndex].ID << '\n';
		std::cout << "North : " << grid[NorthIndex].ID << '\n';
		std::cout << "West : " << grid[WestIndex].ID << '\n';
		std::cout << "Is true? : " << (grid[NorthIndex].ID == grid[gridIndex].ID) << '\n';
		std::cout << "Is true? : " << (grid[SouthIndex].ID == grid[gridIndex].ID) << '\n';
		std::cout << "Is true? : " << (grid[EastIndex].ID == grid[gridIndex].ID) << '\n';
		std::cout << "Is true? : " << (grid[WestIndex].ID == grid[gridIndex].ID) << '\n';
#endif
		if (grid[NorthIndex].ID == grid[gridIndex].ID && grid[NorthIndex].ID) {
			if (matchCount < 2) {
				matchedCells[matchCount] = NorthIndex;
				matchCount++;
			}
		}
		if (grid[EastIndex].ID == grid[gridIndex].ID && grid[EastIndex].ID) {
			if (matchCount < 2) {
				matchedCells[matchCount] = EastIndex;
				matchCount++;
			}
		}
		if (grid[SouthIndex].ID == grid[gridIndex].ID && grid[SouthIndex].ID) {
			if (matchCount < 2) {
				matchedCells[matchCount] = SouthIndex;
				matchCount++;
			}
		}
		if (grid[WestIndex].ID == grid[gridIndex].ID && grid[WestIndex].ID) {
			if (matchCount < 2) {
				matchedCells[matchCount] = WestIndex;
				matchCount++;
			}
		}
		if (matchCount == 1) {
			//Now we check using the selected neighbor
			int selectedNeighbor = matchedCells[0];
			int NorthIndex = selectedNeighbor - gridX;
			int EastIndex = selectedNeighbor + 1;
			int SouthIndex = selectedNeighbor + gridX;
			int WestIndex = selectedNeighbor - 1;

			if (grid[NorthIndex].ID == grid[selectedNeighbor].ID && (NorthIndex != gridIndex)) {
				if (matchCount < 2) {
					matchedCells[matchCount] = NorthIndex;
					matchCount++;
				}
			}
			if (grid[EastIndex].ID == grid[selectedNeighbor].ID && (EastIndex != gridIndex)) {
				if (matchCount < 2) {
					matchedCells[matchCount] = EastIndex;
					matchCount++;
				}
			}
			if (grid[SouthIndex].ID == grid[selectedNeighbor].ID && (SouthIndex != gridIndex)) {
				if (matchCount < 2) {
					matchedCells[matchCount] = SouthIndex;
					matchCount++;
				}
			}
			if (grid[WestIndex].ID == grid[selectedNeighbor].ID && (WestIndex != gridIndex)) {
				if (matchCount < 2) {
					matchedCells[matchCount] = WestIndex;
					matchCount++;
				}
			}
		}
		// std::cout << matchCount <<'\n';
		//if more than 200 means it's lvl 3
		if (matchCount == 2 && grid[gridIndex].ID < 200) {
			for (int c : matchedCells) {
				grid[c].ID = iso::NONE;
			}
			grid[gridIndex].ID += 100;
			//then we recurse and check again till no matches
			CheckCellNeighbor(grid, cellIndex);
		}
	}

	// iso::cell& GetNeighbor(iso::cell*&grid,iso::vec2i indexToCheck, iso::vec2i selectedCell){
	// 	int selectedIndex = GetIndex(selectedCell.x,selectedCell.y);	//The starting index
	// 	int NorthIndex = GetIndex(indexToCheck.x,indexToCheck.y--);		
	// 	std::cout << "Selected : " << selectedCell.x << ", " << selectedCell.y << '\n';
	// 	std::cout << "North : " << indexToCheck.x << ", " << --indexToCheck.y << '\n';
	// 	// std::cout << " og index : " << selectedIndex << " , north index : " << NorthIndex << '\n';
	// 	//If the index I'm checking has a north neighbor and it's NOT the cell I came from
	// 	if((NorthIndex >= 0) && (NorthIndex <= (gridX*gridY)) && (NorthIndex!=selectedIndex)){
	// 		//if it's the same tile type, we want to keep track of it
	// 		if(grid[NorthIndex].ID == grid[GetIndex(selectedCell.x,selectedCell.y)]._tileType) return grid[NorthIndex];
	// 	}

	// 	int EastIndex = GetIndex(indexToCheck.x++,indexToCheck.y);
	// 	if((EastIndex >=0) && (EastIndex <= gridX*gridY) && (EastIndex!=selectedIndex)){
	// 		if(grid[EastIndex].ID == grid[GetIndex(selectedCell.x,selectedCell.y)]._tileType) return grid[EastIndex];
	// 	}

	// 	int SouthIndex = GetIndex(indexToCheck.x,indexToCheck.y++);
	// 	if((SouthIndex >=0) && (SouthIndex <= gridX*gridY) && (SouthIndex!=selectedIndex)){
	// 		if(grid[SouthIndex].ID == grid[GetIndex(selectedCell.x,selectedCell.y)]._tileType) return grid[SouthIndex];
	// 	}

	// 	int WestIndex = GetIndex(indexToCheck.x--,indexToCheck.y);
	// 	if((WestIndex >=0) && (WestIndex <= gridX*gridY) && (WestIndex!=selectedIndex)){
	// 		if(grid[WestIndex].ID == grid[GetIndex(selectedCell.x,selectedCell.y)]._tileType) return grid[WestIndex];
	// 	}
	// 	return grid[selectedIndex];
	// }
	int GetIndex(int x, int y)
	{
		return x + gridX * y;
	}
}