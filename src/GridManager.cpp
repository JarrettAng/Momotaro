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

#include <GridManager.h>
#include <PauseManager.h>

namespace GridManager {
	namespace iso = IsometricGrid;

	int synergyPoints{ 0 };
	int terrainNum{ 0 };

	iso::cell* grid;
	const int gridX{ 20 }, gridY{ 20 };		//total grid size
	const int mapSize{ 5 };				//total playing area size
	const int mapPos{ -2 };		//Playable area position

	void storeClickData();

	int randomNature{ 0 };

	int buildingID{ 0 };			//THIS ID IS FOR TRACKING BUILDINGS!

	//Test enum
	BuildingEnum::ORIENTATION TestOrientation{ BuildingEnum::RIGHT };

#pragma region BuildingStuff
	//Temporary stuff for buildings
	Building ResidentialLvl1{
		BuildingEnum::RESIDENTIAL,
		Vec2<int>{1,1},
		BuildingEnum::L1,
		BuildingEnum::RIGHT,
		1,3,-3,2,
		"Residential Lvl 1","Joe",
		TextureManager::RESIDENTIAL_1X1_L1 };

	Building BigResidentialLvl1{
		BuildingEnum::RESIDENTIAL,
		Vec2<int>{2,2},
		BuildingEnum::L1,
		BuildingEnum::RIGHT,
		1,5,-8,1,
		"Residential Lvl 1","Bigger Joe",
		TextureManager::RESIDENTIAL_1X2_L1 };

	Building CommercialLvl1{
		BuildingEnum::COMMERCIAL,
		Vec2<int>{1,1},
		BuildingEnum::L1,
		BuildingEnum::RIGHT,
		3,0,-1,1,
		"Commercial Lvl 1","Sugondese",
		TextureManager::COMMERCIAL_1X1_L1 };

	Building IndustrialLvl1{
		BuildingEnum::INDUSTRIAL,
		Vec2<int>{1,1},
		BuildingEnum::L1,
		BuildingEnum::RIGHT,
		-3,-1,3,0,
		"Industrial Lvl 1","Candice",
		TextureManager::INDUSTRIAL_1X1_L1 };

	Building NatureTree{
		BuildingEnum::NATURE,
		Vec2<int>{1,1},
		BuildingEnum::L1,
		BuildingEnum::RIGHT,
		0,0,0,0,
		"Nature!","Tree",
		TextureManager::NATURE_TREE
	};
	Building NatureRock{
		BuildingEnum::NATURE,
		Vec2<int>{1,1},
		BuildingEnum::L1,
		BuildingEnum::RIGHT,
		0,0,0,0,
		"Nature!","The Rock",
		TextureManager::NATURE_ROCK
	};
#pragma endregion


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

				//basically we want the grid to be from -2 to 2, but since there's a 10 unit offset, we add 10
				if (((x >= (mapPos + 10)) && (x <= (mapPos + 10 + mapSize))) && (y >= (mapPos + 10) && y <= (mapPos + 10 + mapSize))) {
					// grid[index].ID = iso::RESIDENTIAL;
					grid[index].isRenderable = true;
					// std::cout << "y is " << y << " and iso is " << test.x<<'\n';
				}
				grid[index].ID = 0;
			}
		}

		// Hardcoded island parts
		// Top left


		InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, storeClickData);
		InputManager::SubscribeToKey(AEVK_C, InputManager::TRIGGERED, ClearGrid);
		InputManager::SubscribeToKey(AEVK_R, InputManager::TRIGGERED, RandomiseTerrain);
		InputManager::SubscribeToKey(AEVK_1, InputManager::TRIGGERED, SpawnResidential);
		InputManager::SubscribeToKey(AEVK_2, InputManager::TRIGGERED, SpawnCommerical);
		InputManager::SubscribeToKey(AEVK_3, InputManager::TRIGGERED, SpawnIndustrial);
		InputManager::SubscribeToKey(AEVK_Q, InputManager::TRIGGERED, SpawnBigResidential);
		InputManager::SubscribeToKey(AEVK_W, InputManager::TRIGGERED, SpawnBigResidential3x1);
		InputManager::SubscribeToKey(AEVK_E, InputManager::TRIGGERED, SpawnBigResidential);
		InputManager::SubscribeToKey(AEVK_S, InputManager::TRIGGERED, SpawnBigResidential);
		InputManager::SubscribeToKey(AEVK_N, InputManager::TRIGGERED, SpawnNature);
	}


	bool isCellSafe(iso::vec2i selectedCell) {
		if ((((selectedCell.x) < 0) || ((selectedCell.x) >= gridX)) || ((selectedCell.y) < 0 || (selectedCell.y) >= gridY)) return false;
		if (!grid[GetIndex(selectedCell.x, selectedCell.y)].isRenderable) return false;
		return true;
	}

	void ChangeOrientation() {
		TestOrientation = static_cast<BuildingEnum::ORIENTATION>(TestOrientation + 1);
		if (TestOrientation == BuildingEnum::ORIENTATION_LENGTH) {
			TestOrientation = static_cast<BuildingEnum::ORIENTATION>(0);
		}
	}

	void SpawnBigResidential() {
		//1x2
		if (PauseManager::IsPaused()) return;
		// ClearGrid();
		Vec2<int> mousePos = InputManager::GetMousePos();
		iso::vec2i SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;
		ChangeOrientation();
		SetGridIndex(TestOrientation, BigResidentialLvl1.data.size, SelectedCell.x, SelectedCell.y);
		// grid[index].ID = ++buildingID;
		// grid[index]._building = ResidentialLvl1;
	}
	void SpawnBigResidential3x1() {
		//1x2
		if (PauseManager::IsPaused()) return;
		// ClearGrid();
		Vec2<int> mousePos = InputManager::GetMousePos();
		iso::vec2i SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;
		ChangeOrientation();
		SetGridIndex(TestOrientation, Vec2<int>{3, 1}, SelectedCell.x, SelectedCell.y);
		// grid[index].ID = ++buildingID;
		// grid[index]._building = ResidentialLvl1;
	}

	void SpawnResidential() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		iso::vec2i SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;

		grid[index].ID = ++buildingID;
		grid[index]._building = ResidentialLvl1;
		CheckCellNeighbor(grid, SelectedCell);

#if testVector
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
#endif

	}
	void SpawnCommerical() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		iso::vec2i SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;

		grid[index].ID = ++buildingID;
		grid[index]._building = CommercialLvl1;
		CheckCellNeighbor(grid, SelectedCell);
	}
	void SpawnIndustrial() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		iso::vec2i SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;

		grid[index].ID = ++buildingID;
		grid[index]._building = IndustrialLvl1;
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
			grid[index]._building = NatureRock;
		else
			grid[index]._building = NatureTree;
		grid[index].ID = ++buildingID;
	}
#pragma region TerrainStuff
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
#pragma endregion

	void SetGridIndex(BuildingEnum::ORIENTATION _orientation, Vec2<int> _size, int _x, int _y)
	{
		int index = GetIndex(_x, _y);
		// TextureManager::TEX_TYPE test{TextureManager::RESIDENTIAL_1X2_L1};
		iso::vec2i _SelectedCell{ 0,0 };
		// for(int y{0}; y < _size.y; ++y){
		// 	for(int x{0}; x<_size.x; ++x){
		// 		switch (_orientation)
		// 		{
		// 		case BuildingEnum::RIGHT:
		// 			if(!isCellSafe(iso::vec2i{_x+x,y+_y})){
		// 				std::cout << "Invalid position!\n";
		// 				return;
		// 			}
		// 		case BuildingEnum::TOP:
		// 			if(!isCellSafe(iso::vec2i{_x+x,_y-y})){
		// 				std::cout << "Invalid position!\n";
		// 				return;
		// 			}
		// 		case BuildingEnum::LEFT:
		// 			if(!isCellSafe(iso::vec2i{_x-x,_y-y})){
		// 				std::cout << "Invalid position!\n";
		// 				return;
		// 			}
		// 		case BuildingEnum::DOWN:
		// 			if(!isCellSafe(iso::vec2i{_x-x,y+_y})){
		// 				std::cout << "Invalid position!\n";
		// 				return;
		// 			}
		// 		}
		// 	}
		// }

		std::cout << "Cell Pos : " << _x << ", " << _y << '\n';
		//if the orientation is right or left, we have to swap the 2x1 into 1x2
		if (_orientation == BuildingEnum::RIGHT || _orientation == BuildingEnum::LEFT) {
			_size = Vec2<int>{ _size.y,_size.x };
		}
		grid[index].ID = ++buildingID;
		// grid[index]._building = BigResidentialLvl1;
		for (int y{ 0 }; y < _size.y; ++y) {
			for (int x{ 0 }; x < _size.x; ++x) {
				int otherIndex{ 0 };
				// grid[otherIndex]._building = BigResidentialLvl1;
				switch (_orientation)
				{
				case BuildingEnum::RIGHT:
					// std::cout<<"RIGHT\n";
					otherIndex = GetIndex(_x + x, y + _y);
					_SelectedCell = { _x + x,y + y };
					break;
				case BuildingEnum::TOP:
					// std::cout<<"TOP\n";
					otherIndex = GetIndex(_x + x, _y - y);
					_SelectedCell = { _x + x,_y - y };

					break;
				case BuildingEnum::LEFT:
					// std::cout<<"LEFT\n";
					otherIndex = GetIndex(_x - x, _y - y);
					_SelectedCell = { _x - x,_y - y };

					break;
				case BuildingEnum::DOWN:
					std::cout << "DOWN\n";
					otherIndex = GetIndex(_x - x, y + _y);
					_SelectedCell = { _x - x,y + _y };
					break;
				}
				grid[otherIndex].ID = buildingID;
				// std::cout <<"INDEX ID : "<< grid[index].ID << '\n';
				// std::cout <<"OTHER INDEX ID : " <<grid[otherIndex].ID << '\n';
				// std::cout << "SELECTED CELL : " << _SelectedCell.x << ", " << _SelectedCell.y << '\n';
				// grid[otherIndex]._building.data.TextureID = test;
				grid[otherIndex]._building = BigResidentialLvl1;
				// grid[otherIndex]._building.data.TextureID = TextureManager::NATURE_ROCK;
				std::cout << grid[otherIndex]._building.data.type << '\n';
				// CheckCellNeighbor(grid,_SelectedCell);
			}
		}
		// grid[index]._building.data.TextureID = test;
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
		// //ROTATES BETWEEN BUILDINGS ON CLICK
		// switch (grid[index].ID)
		// {
		// case iso::NONE:
		// 	grid[index].ID = iso::RESIDENTIAL;
		// 	break;
		// case iso::RESIDENTIAL:
		// 	grid[index].ID = iso::INDUSTRIAL;
		// 	break;
		// case iso::INDUSTRIAL:
		// 	grid[index].ID = iso::COMMERCIAL;
		// 	break;
		// case iso::COMMERCIAL:
		// 	randomNature = rand() % 2;
		// 	if (randomNature % 2 == 0)
		// 		grid[index].ID = iso::TREE;
		// 	else
		// 		grid[index].ID = iso::ROCK;
		// 	break;
		// case iso::ROCK:
		// 	grid[index].ID = iso::NONE;
		// 	break;
		// case iso::TREE:
		// 	grid[index].ID = iso::NONE;
		// 	break;
		// }

		//This is where it gets messy sorry
		//MERGE LOGIC
		// CheckCellNeighbor(grid, SelectedCell);

	}

	void ClearGrid() {
		if (PauseManager::IsPaused()) return;

		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);
				grid[index].ID = iso::NONE;
				grid[index]._building = Building{};
			}
		}
	}

	void PrepareTileRenderBatch() {
		// Your own rendering logic goes here
		//Render grid test
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);

				if (grid[index].ID > 0) {
					RenderSystem::AddRectToBatch(
						RenderSystem::GAME_PIECES_BATCH,
						grid[index].pos.x, grid[index].pos.y,
						100, 100,
						grid[index]._building.data.TextureID
					);
				}
				if (grid[index].isRenderable) RenderSystem::AddRectToBatch(RenderSystem::TILE_BATCH, grid[index].pos.x, grid[index].pos.y, 100, 100, TextureManager::TILE_TEX);

#pragma region Old Code
				// switch (grid[index].ID) {
				// 	//RESIDENTIAL
				// case iso::RESIDENTIAL + SMALL:
				// 	RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::RESIDENTIAL_1X1_L1, grid[index].pos.x, grid[index].pos.y);
				// 	break;
				// case iso::RESIDENTIAL + MEDIUM:
				// 	RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::RESIDENTIAL_1X1_L2, grid[index].pos.x, grid[index].pos.y);
				// 	break;
				// case iso::RESIDENTIAL + LARGE:
				// 	RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::RESIDENTIAL_1X1_L3, grid[index].pos.x, grid[index].pos.y);
				// 	break;

				// 	//COMMERCIAL
				// case iso::COMMERCIAL + SMALL:
				// 	RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::COMMERCIAL_1X1_L1, grid[index].pos.x, grid[index].pos.y);
				// 	break;
				// case iso::COMMERCIAL + MEDIUM:
				// 	RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::COMMERCIAL_1X1_L2, grid[index].pos.x, grid[index].pos.y);
				// 	break;
				// case iso::COMMERCIAL + LARGE:
				// 	RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::COMMERCIAL_1X1_L3, grid[index].pos.x, grid[index].pos.y);
				// 	break;

				// 	//INDUSTRIAL
				// case iso::INDUSTRIAL + SMALL:
				// 	RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::INDUSTRIAL_1X1_L1, grid[index].pos.x, grid[index].pos.y);
				// 	break;
				// case iso::INDUSTRIAL + MEDIUM:
				// 	RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::INDUSTRIAL_1X1_L2, grid[index].pos.x, grid[index].pos.y);
				// 	break;
				// case iso::INDUSTRIAL + LARGE:
				// 	RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::INDUSTRIAL_1X1_L3, grid[index].pos.x, grid[index].pos.y);
				// 	break;

				// 	//NATURE
				// case iso::TREE:
				// 	RenderSystem::AddSpriteBatch(RenderSystem::NATURE_BATCH, RenderSystem::NATURE, TextureManager::NATURE_TREE, grid[index].pos.x, grid[index].pos.y);
				// 	break;
				// case iso::ROCK:
				// 	RenderSystem::AddSpriteBatch(RenderSystem::NATURE_BATCH, RenderSystem::NATURE, TextureManager::NATURE_ROCK, grid[index].pos.x, grid[index].pos.y);
				// 	break;

				// default:
				// 	// RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING, TextureManager::RESIDENTIAL_S, grid[index].pos.x, grid[index].pos.y);
				// 	break;
				// }
#pragma endregion
			}
		}
		// UIManager::RenderButton(0, 0, 100, 100, 0, UIManager::GetFont(UIManager::ROBOTO).S, "dawdawdwadwadawdawd", Vec4<float>{1, 1, 0, 1}, Vec3<float>{1, 0, 1});
	}

	void GridManager::CheckCellNeighbor(iso::cell* grid, iso::vec2i cellIndex)
	{
		//The order to check is CLOCKWISE, so we go NORTH, EAST, SOUTH, WEST
		int matchCount{ 0 };
		int matchedCells[2]{};
		int gridIndex = GetIndex(cellIndex.x, cellIndex.y);
		int NorthIndex = GetIndex(cellIndex.x, cellIndex.y - 1);
		int EastIndex = GetIndex(cellIndex.x + 1, cellIndex.y);
		int SouthIndex = GetIndex(cellIndex.x, cellIndex.y + 1);
		int WestIndex = GetIndex(cellIndex.x - 1, cellIndex.y);
		//NORTH
#if 1
		std::cout << "Index : " << cellIndex.x << ", " << cellIndex.y << '\n';
		std::cout << "Selected : " << grid[gridIndex]._building.data.type << '\n';
		std::cout << "North : " << grid[NorthIndex]._building.data.type << '\n';
		std::cout << "East : " << grid[EastIndex]._building.data.type << '\n';
		std::cout << "South : " << grid[SouthIndex]._building.data.type << '\n';
		std::cout << "West : " << grid[WestIndex]._building.data.type << '\n';

		std::cout << "Is N true? : " << (grid[NorthIndex].ID != 0 && grid[NorthIndex]._building.data.type == grid[gridIndex]._building.data.type) << '\n';
		std::cout << "Is E true? : " << (grid[EastIndex].ID != 0 && grid[EastIndex]._building.data.type == grid[gridIndex]._building.data.type) << '\n';
		std::cout << "Is S true? : " << (grid[SouthIndex].ID != 0 && grid[SouthIndex]._building.data.type == grid[gridIndex]._building.data.type) << '\n';
		std::cout << "Is W true? : " << (grid[WestIndex].ID != 0 && grid[WestIndex]._building.data.type == grid[gridIndex]._building.data.type) << '\n';
#endif

		if (grid[NorthIndex].ID != 0 && grid[NorthIndex]._building == grid[gridIndex]._building && grid[NorthIndex].ID != grid[gridIndex].ID) {
			if (matchCount < 2) {
				matchedCells[matchCount] = NorthIndex;
				matchCount++;
			}
		}
		if (grid[EastIndex].ID != 0 && grid[EastIndex]._building == grid[gridIndex]._building && grid[EastIndex].ID != grid[gridIndex].ID) {
			if (matchCount < 2) {
				matchedCells[matchCount] = EastIndex;
				matchCount++;
			}
		}
		if (grid[SouthIndex].ID != 0 && grid[SouthIndex]._building == grid[gridIndex]._building && grid[SouthIndex].ID != grid[gridIndex].ID) {
			if (matchCount < 2) {
				matchedCells[matchCount] = SouthIndex;
				matchCount++;
			}
		}
		if (grid[WestIndex].ID != 0 && grid[WestIndex]._building == grid[gridIndex]._building && grid[WestIndex].ID != grid[gridIndex].ID) {
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
			//If it is not the cell you came from and the building types match
			if ((NorthIndex != gridIndex) && grid[NorthIndex]._building == grid[selectedNeighbor]._building && grid[NorthIndex].ID != grid[gridIndex].ID) {
				if (matchCount < 2) {
					matchedCells[matchCount] = NorthIndex;
					matchCount++;
				}
			}
			if ((EastIndex != gridIndex) && grid[EastIndex]._building == grid[selectedNeighbor]._building && grid[EastIndex].ID != grid[gridIndex].ID) {
				if (matchCount < 2) {
					matchedCells[matchCount] = EastIndex;
					matchCount++;
				}
			}
			if ((SouthIndex != gridIndex) && grid[SouthIndex]._building == grid[selectedNeighbor]._building && grid[SouthIndex].ID != grid[gridIndex].ID) {
				if (matchCount < 2) {
					matchedCells[matchCount] = SouthIndex;
					matchCount++;
				}
			}
			if ((WestIndex != gridIndex) && grid[WestIndex]._building == grid[selectedNeighbor]._building && grid[WestIndex].ID != grid[gridIndex].ID) {
				if (matchCount < 2) {
					matchedCells[matchCount] = WestIndex;
					matchCount++;
				}
			}
		}
		std::cout << "match count is " << matchCount << '\n';
		//if more than 200 means it's lvl 3
		if (matchCount == 2 && grid[gridIndex]._building.data.level < BuildingEnum::L3) {
			for (int c : matchedCells) {
				std::cout << "MATCH!\n";
				grid[c].ID = 0;
				grid[c]._building = Building{};
				// if(grid[c]._building.data.size != Vec2<int>{1,1}){
				// Vec2<int> _size{grid[c]._building.data.size};
				// 	if(grid[c]._building.data.orientation == BuildingEnum::RIGHT || grid[c]._building.data.orientation == BuildingEnum::LEFT){
				// 		_size = Vec2<int>{_size.y,_size.x};
				// 	}
				// 	int _x{grid[c].pos.x};
				// 	int _y{grid[c].pos.y};
				// 	iso::vec2i _SelectedCell{};
				// 	for(int y{0}; y< _size.y; ++y){
				// 		for(int x{0}; x<_size.x; ++x){
				// 			int otherIndex{0};
				// 			// grid[otherIndex]._building = BigResidentialLvl1;
				// 			switch (grid[c]._building.data.orientation)
				// 			{
				// 			case BuildingEnum::RIGHT:
				// 			std::cout<<"RIGHT\n";
				// 			otherIndex = GetIndex(_x+x,y+_y);
				// 			_SelectedCell = {_x+x,y+y};
				// 				break;
				// 			case BuildingEnum::TOP:
				// 			std::cout<<"TOP\n";
				// 			otherIndex = GetIndex(_x+x,_y-y);
				// 			_SelectedCell = {_x+x,_y-y};

				// 				break;
				// 			case BuildingEnum::LEFT:
				// 			std::cout<<"LEFT\n";
				// 			otherIndex = GetIndex(_x-x,_y-y);
				// 			_SelectedCell = {_x-x,_y-y};

				// 				break;
				// 			case BuildingEnum::DOWN:
				// 			std::cout<<"DOWN\n";
				// 			otherIndex = GetIndex(_x-x,y+_y);
				// 			_SelectedCell = {_x-x,y+_y};
				// 				break;
				// 			}
				// 			grid[GetIndex(_SelectedCell.x,_SelectedCell.y)].ID=0;
				// 			grid[GetIndex(_SelectedCell.x,_SelectedCell.y)]._building = Building{};
				// 		}
				// 	}
				// }
			}
			grid[gridIndex]._building.LevelUp();
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