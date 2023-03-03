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
#include <FontManager.h>
#include <IsometricGrid.h>
#include <iostream>
#include <MomoMaths.h>
#include <ColorTable.h>
#include <GridManager.h>
#include <PauseManager.h>
#include <CardManager.h>
#include <UIManager.h>
#include <ScoreManager.h>
namespace GridManager {
	namespace iso = IsometricGrid;

	int synergyPoints{ 0 };
	int terrainNum{ 2 };

	iso::cell* grid;
	const int gridX{ 20 }, gridY{ 20 };		//total grid size
	const int mapSize{ 5 };				//total playing area size
	const int mapPos{ -2 };		//Playable area position

	void storeClickData();

	int randomNature{ 0 };

	int buildingID{ 0 };			//THIS ID IS FOR TRACKING BUILDINGS!

	std::vector<Vec2<int>> CurrentSynergyArea{};
	std::vector<Vec2<int>> CurrentBuildingCells{};

	static int previousIndex{ -1 };
	static int currentIndex{ -2 };

	int totalPoints{};

	//Test enum
	BuildingEnum::ORIENTATION TestOrientation{ BuildingEnum::RIGHT };
	const BuildingData* selectedBuilding{};
	EventSystem::Event<void> onMergeBuildings;

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
	Building BigResidential3x1Lvl1{
		BuildingEnum::RESIDENTIAL,
		Vec2<int>{3,1},
		BuildingEnum::L1,
		BuildingEnum::RIGHT,
		1,5,-8,1,
		"Residential Lvl 1","Bigger Joe mama",
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
				Vec2<int> ScreenPos = iso::WorldIndexToScreenPos(x, y);
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
		RandomiseTerrain();

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

		CardManager::onNewCardSelected.Subscribe(GetBuildingCard);
		CardManager::onCardPlaced.Subscribe(SpawnBuilding);
	}
	void SpawnBuilding(Vec2<int>mousePos) {
		Vec2<int> SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		if (!isCellSafe(SelectedCell)) {
			std::cout << "Error " << __FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
			return;
		}
		int index = GetIndex(SelectedCell);
		grid[index].ID = ++buildingID;
		grid[index]._building.data = *selectedBuilding;
		grid[index]._building.buildingCells = CurrentBuildingCells;
		CheckCellNeighbor(grid, SelectedCell);
		ScoreManger::AddScore(totalPoints);
		CurrentSynergyArea.clear();
		CurrentBuildingCells.clear();
	}

	bool isCellSafe(Vec2<int> selectedCell) {
		// if ((((selectedCell.x) < 0) || ((selectedCell.x) > gridX)) || ((selectedCell.y) < 0 || (selectedCell.y) > gridY)) return false;
		if (grid[GetIndex(selectedCell.x, selectedCell.y)].ID > 0) return false;
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
		ClearGrid();
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;
		ChangeOrientation();
		// SetGridIndex(TestOrientation, BigResidentialLvl1.data, SelectedCell.x, SelectedCell.y);
		grid[index]._building.GetSynergyArea();
		for (Vec2<int> cell : grid[index]._building.synergyAreaCells) {
			grid[GetIndex(cell)].isRenderable = false;
		}
	}
	void SpawnBigResidential3x1() {
		//1x2
		if (PauseManager::IsPaused()) return;
		// ClearGrid();
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;
		// ChangeOrientation();
		// SetGridIndex(TestOrientation, BigResidential3x1Lvl1.data, SelectedCell.x, SelectedCell.y);

	}

	void SpawnResidential() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;

		grid[index].ID = ++buildingID;
		grid[index]._building = ResidentialLvl1;
		grid[index]._building.buildingCells.push_back(SelectedCell);
		grid[index]._building.GetSynergyArea();
		for (Vec2<int> cell : grid[index]._building.synergyAreaCells) {
			// grid[GetIndex(cell)].isRenderable = false;
		}
		CheckCellNeighbor(grid, SelectedCell);


	}
	void SpawnCommerical() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;

		grid[index].ID = ++buildingID;
		grid[index]._building = CommercialLvl1;
		grid[index]._building.buildingCells.push_back(SelectedCell);
		CheckCellNeighbor(grid, SelectedCell);
	}
	void SpawnIndustrial() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;

		grid[index].ID = ++buildingID;
		grid[index]._building = IndustrialLvl1;
		grid[index]._building.buildingCells.push_back(SelectedCell);
		CheckCellNeighbor(grid, SelectedCell);
	}
	void SpawnNature() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
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
		case 1:
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
		case 0:
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


	//Subscribed to select building card
	void GetBuildingCard(const BuildingData* _data) {
		// if (_data != nullptr)
		selectedBuilding = _data;
	}
	bool IsBuildingValid(const BuildingData* _data, int _x, int _y) {
		int index = GetIndex(_x, _y);
		Vec2<int> _size = _data->size;
		Vec2<int> _SelectedCell{ 0,0 };
		std::vector<Vec2<int>> AllCells;
		if (_data->orientation == BuildingEnum::RIGHT || _data->orientation == BuildingEnum::LEFT) {
			_size = Vec2<int>{ _size.y,_size.x };
		}
		for (int y{ 0 }; y < _size.y; ++y) {
			for (int x{ 0 }; x < _size.x; ++x) {
				switch (_data->orientation)
				{
				case BuildingEnum::RIGHT:
					if (!isCellSafe(Vec2<int>{_x + x, y + _y})) {
						std::cout << "Error " << __FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
						return false;
					}
					break;
				case BuildingEnum::TOP:
					if (!isCellSafe(Vec2<int>{_x + x, _y - y})) {
						std::cout << "Error " << __FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
						return false;
					}
					break;
				case BuildingEnum::LEFT:
					if (!isCellSafe(Vec2<int>{_x - x, _y - y})) {
						std::cout << "Error " << __FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
						return false;
					}
					break;
				case BuildingEnum::DOWN:
					if (!isCellSafe(Vec2<int>{_x - x, y + _y})) {
						std::cout << "Error " << __FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
						return false;
					}
					break;
				}
			}
		}
		return true;
	}
	std::vector<Vec2<int>> GetBuildingCells(const BuildingData* _data, int _x, int _y) {
		int index = GetIndex(_x, _y);
		Vec2<int> _size = _data->size;
		Vec2<int> _SelectedCell{ 0,0 };
		std::vector<Vec2<int>> AllCells;
		if (!IsBuildingValid(_data, _x, _y)) assert("");
		std::cout << "Cell Pos : " << _x << ", " << _y << '\n';
		// grid[index].ID = ++buildingID;
		for (int y{ 0 }; y < _size.y; ++y) {
			for (int x{ 0 }; x < _size.x; ++x) {
				int otherIndex{ 0 };
				switch (_data->orientation)
				{
				case BuildingEnum::RIGHT:
					// std::cout<<"RIGHT\n";
					otherIndex = GetIndex(_x + x, y + _y);
					_SelectedCell = { _x + x,_y + y };
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
					// std::cout << "DOWN\n";
					otherIndex = GetIndex(_x - x, y + _y);
					_SelectedCell = { _x - x,y + _y };
					break;
				}
				// grid[otherIndex].ID = buildingID;
				// grid[otherIndex]._building.data = _data;
				AllCells.push_back(_SelectedCell);
			}
		}
		// for (Vec2<int> cell : AllCells) {
		// 	grid[GetIndex(cell.x, cell.y)]._building.buildingCells.assign(AllCells.begin(), AllCells.end());
		// }
		return AllCells;
	}

	std::vector<Vec2<int>> GetSynergyArea(std::vector<Vec2<int>> _buildingCells)
	{
		if (CurrentBuildingCells.empty()) std::cerr << "Error " << __FILE__ << "ln" << __LINE__ << " : NO BUILDING CELLS TO GET AREA!\n";
		// return std::vector<Vec2<int>>{};
		//First we get all the building cells
		std::vector<Vec2<int>> tempVec;
		//Then for every building cell, we get the diagonal AND adjacent cells.
		for (Vec2<int> cell : CurrentBuildingCells) {
			for (int x{ -1 }; x < 2; ++x) {
				tempVec.push_back(cell + Vec2<int>{0, x * 2});
				tempVec.push_back(cell + Vec2<int>{x * 2, 0});
				for (int y{ -1 }; y < 2; ++y) {
					tempVec.push_back(cell + Vec2<int>{x, y});
				}
			}
		}
		//Small hack to make a compare operator then sort the vector of vectors
		std::sort(tempVec.begin(), tempVec.end(), [](Vec2<int> a, Vec2<int> b) {return a < b; });	//once we sort it we prune
		auto last = std::unique(tempVec.begin(), tempVec.end());
		tempVec.erase(last, tempVec.end());
		return tempVec;
		// synergyAreaCells = tempVec;
		// for(Vec2<int>cell : synergyAreaCells){
		// 	std::cout << "SYNERGY AREA : " << cell << '\n';
		// }
	}

	void SetGridIndex(BuildingEnum::ORIENTATION _orientation, const BuildingData* _data, int _x, int _y)
	{
		// int index = GetIndex(_x, _y);
		// Vec2<int> _size = _data.size;
		// // TextureManager::TEX_TYPE test{TextureManager::RESIDENTIAL_1X2_L1};
		// Vec2<int> _SelectedCell{ 0,0 };
		// std::vector<Vec2<int>> AllCells;
		// //if the orientation is right or left, we have to swap the 2x1 into 1x2
		// if (_orientation == BuildingEnum::RIGHT || _orientation == BuildingEnum::LEFT) {
		// 	_size = Vec2<int>{ _size.y,_size.x };
		// }
		// for (int y{ 0 }; y < _size.y; ++y) {
		// 	for (int x{ 0 }; x < _size.x; ++x) {
		// 		switch (_orientation)
		// 		{
		// 		case BuildingEnum::RIGHT:
		// 			if (!isCellSafe(Vec2<int>{_x + x, y + _y})) {
		// 				std::cout << "Error " <<__FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
		// 				return;
		// 			}
		// 			break;
		// 		case BuildingEnum::TOP:
		// 			if (!isCellSafe(Vec2<int>{_x + x, _y - y})) {
		// 				std::cout<< "Error " <<__FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
		// 				return;
		// 			}
		// 			break;
		// 		case BuildingEnum::LEFT:
		// 			if (!isCellSafe(Vec2<int>{_x - x, _y - y})) {
		// 				std::cout << "Error " <<__FILE__ << "ln" << __LINE__ <<  ": Invalid position!\n";
		// 				return;
		// 			}
		// 			break;
		// 		case BuildingEnum::DOWN:
		// 			if (!isCellSafe(Vec2<int>{_x - x, y + _y})) {
		// 				std::cout << "Error " <<__FILE__ << "ln" << __LINE__ <<  ": Invalid position!\n";
		// 				return;
		// 			}
		// 			break;
		// 		}
		// 	}
		// }
		// std::cout << "Cell Pos : " << _x << ", " << _y << '\n';
		// grid[index].ID = ++buildingID;
		// for (int y{ 0 }; y < _size.y; ++y) {
		// 	for (int x{ 0 }; x < _size.x; ++x) {
		// 		int otherIndex{ 0 };
		// 		switch (_orientation)
		// 		{
		// 		case BuildingEnum::RIGHT:
		// 			// std::cout<<"RIGHT\n";
		// 			otherIndex = GetIndex(_x + x, y + _y);
		// 			_SelectedCell = { _x + x,_y + y };
		// 			break;
		// 		case BuildingEnum::TOP:
		// 			// std::cout<<"TOP\n";
		// 			otherIndex = GetIndex(_x + x, _y - y);
		// 			_SelectedCell = { _x + x,_y - y };
		// 			break;
		// 		case BuildingEnum::LEFT:
		// 			// std::cout<<"LEFT\n";
		// 			otherIndex = GetIndex(_x - x, _y - y);
		// 			_SelectedCell = { _x - x,_y - y };
		// 			break;
		// 		case BuildingEnum::DOWN:
		// 			// std::cout << "DOWN\n";
		// 			otherIndex = GetIndex(_x - x, y + _y);
		// 			_SelectedCell = { _x - x,y + _y };
		// 			break;
		// 		}
		// 		grid[otherIndex].ID = buildingID;
		// 		// std::cout <<"INDEX ID : "<< grid[index].ID << '\n';
		// 		// std::cout <<"OTHER INDEX : " <<otherIndex << '\n';
		// 		// std::cout << "SELECTED CELL : " << _SelectedCell << '\n';
		// 		// grid[otherIndex]._building.data.TextureID = test;
		// 		grid[otherIndex]._building.data = _data;
		// 		AllCells.push_back(_SelectedCell);
		// 		// grid[otherIndex]._building.data.TextureID = TextureManager::NATURE_ROCK;
		// 		// std::cout << grid[otherIndex]._building.data.type << '\n';
		// 	}
		// }
		// for (Vec2<int> cell : AllCells) {
		// 	grid[GetIndex(cell.x, cell.y)]._building.buildingCells.assign(AllCells.begin(), AllCells.end());
		// 	// std::cout << "Cell to check : " << cell << '\n';
		// }
		//CheckCellNeighbor(grid, _SelectedCell);
		// std::cout <<"ALL CELLS SIZE : "<< AllCells.size() << '\n'; 
		// std::cout << "INDEX : " << index <<'\n';
		// std::cout <<"SIZE : "<< grid[index]._building.buildingCells.size() << '\n'; 
		// grid[index]._building.data.TextureID = test;
	}

	void UpdateMouseToGrid() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos{ InputManager::GetMousePos() };
		Vec2<int> SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		//First we check if the mouse has moved
		if (InputManager::HasMouseMoved()) {
			//Then we check if the index is different
		}
		currentIndex = GetIndex(SelectedCell);
		//If the mouse has moved out of the previous index, we need to update the synergy cells
		if (currentIndex != previousIndex) {
			//First we clear out the vector
			CurrentSynergyArea.clear();
			CurrentBuildingCells.clear();
			//DRAWING DEBUG TEXT
			//Then we set the grid index
			// std::cout << "CLEARED VECTORS\n";
			if (selectedBuilding != nullptr) {
			// std::cout << "SELECTED BUILDING NOT NULL\n";
				if (IsBuildingValid(selectedBuilding, SelectedCell.x, SelectedCell.y)) {
					CurrentBuildingCells = GetBuildingCells(selectedBuilding, SelectedCell.x, SelectedCell.y);
					CurrentSynergyArea = GetSynergyArea(CurrentBuildingCells);

					// RenderSystem::AddTextToBatch(
					// 	RenderSystem::GAME_PIECES_BATCH,
					// 	((float)InputManager::GetMousePos().x / AEGetWindowWidth() * 2) - 1,
					// 	(((float)InputManager::GetMousePos().y / AEGetWindowHeight() * 2) - 1) * -1,
					// 	FontManager::GetFont(FontManager::ROBOTO),
					// 	20,
					// 	std::to_string(InputManager::GetMousePosDelta().x) + " , " + std::to_string(InputManager::GetMousePosDelta().y),
					// 	99,
					// 	COLOR_BLACK
					// );

				}


			}
		}
		previousIndex = currentIndex;
	}

	void storeClickData() {
		if (PauseManager::IsPaused()) return;

		// Vec2<int> mousePos = InputManager::GetMousePos();
		// //Convert the mouse position into iso
		// Vec2<int> SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
		// //We offset by 10 units x and y because of how iso works. We moved the grid up by 10 units
		// if ((((SelectedCell.x) < 0) || ((SelectedCell.x) > gridX)) || ((SelectedCell.y) < 0 || (SelectedCell.y) > gridY)) return;
		// int index = (SelectedCell.x) + gridX * (SelectedCell.y);
		// index = GetIndex(SelectedCell.x, SelectedCell.y);

		// //if the cell is water (means it doesn't need to be rendered) we don't allow placement
		// if (!grid[index].isRenderable) return;
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
						static_cast<float>(grid[index].pos.x), static_cast<float>(grid[index].pos.y),
						100, 100,
						grid[index]._building.data.TextureID, 99
					);
				}
				if (grid[index].isRenderable) RenderSystem::AddRectToBatch(RenderSystem::TILE_BATCH, static_cast<float>(grid[index].pos.x), static_cast<float>(grid[index].pos.y), 100, 100, TextureManager::TILE_TEX);
			}
		}
		if (selectedBuilding != nullptr) {
			if (!CurrentBuildingCells.empty()) {
				for (Vec2<int> cell : CurrentBuildingCells) {
					RenderSystem::AddRectToBatch(
						RenderSystem::GAME_PIECES_BATCH,
						static_cast<float>(grid[GetIndex(cell)].pos.x), static_cast<float>(grid[GetIndex(cell)].pos.y+12.5f),
						100, 100,
						selectedBuilding->TextureID, 99, 0
					);
				}
			}
			if (!CurrentSynergyArea.empty()) {
				UI::TextBox pointText;
				Vec2<float> pointTextPos;
				int points{};
				totalPoints = 0;
				Vec3<float> pointsColor;
				Vec3<float> totalPointsColor;

				Vec2<int> mousePos = InputManager::GetMousePos();
				//Convert the mouse position into iso
				Vec2<int> SelectedCell{ iso::ScreenPosToIso(mousePos.x,mousePos.y) };
				for (Vec2<int> cell : CurrentSynergyArea) {
					pointTextPos.x = static_cast<float>(grid[GetIndex(cell)].pos.x);
					pointTextPos.y = static_cast<float>(grid[GetIndex(cell)].pos.y) - 62.5;
					points = GetSynergyText(cell, *selectedBuilding);
					totalPoints += points;
					if (points > 0) {
						pointsColor = COLOR_BOX_POSITIVE;
					}
					else if (points < 0) {
						pointsColor = COLOR_BOX_NEGATIVE;
					}
					else {
						pointsColor = COLOR_BOX_NEUTRAL;
					}
					if (cell == SelectedCell) {
						continue;
					}
					else if (points != 0) {
						pointText = UI::TextBox(pointTextPos, std::to_string(points), UI::CENTER_JUSTIFY, 100, 42, pointsColor);
					}
					pointText.Render();
					//Draw synergy texture
					RenderSystem::AddRectToBatch(
						RenderSystem::GAME_PIECES_BATCH,
						static_cast<float>(grid[GetIndex(cell)].pos.x), static_cast<float>(grid[GetIndex(cell)].pos.y),
						100, 100,
						TextureManager::POSITIVE_SYNERGY
					);
				}

				if (totalPoints > 0) {
					totalPointsColor = COLOR_BOX_POSITIVE;
				}
				else if (totalPoints < 0) {
					totalPointsColor = COLOR_BOX_NEGATIVE;
				}
				else {
					totalPointsColor = COLOR_BOX_NEUTRAL;
				}
				Vec2<float> totalPointTexPos{ static_cast<float>(grid[GetIndex(SelectedCell)].pos.x),static_cast<float>(grid[GetIndex(SelectedCell)].pos.y) - 62.5 };
				pointText = UI::TextBox(totalPointTexPos, std::to_string(totalPoints), UI::CENTER_JUSTIFY, 120, 69, totalPointsColor);
				pointText.Render();
			}
		}

	}

	int GetSynergyText(Vec2<int> cellToCheck, BuildingData _data) {
		// if (!(&_data))
		// 	std::cerr << "Error " << __FILE__ << "ln" << __LINE__ << " : UNABLE TO GET SYNERGY POINTS!\n";
		// return 0;
		switch (grid[GetIndex(cellToCheck)]._building.data.type) {
		case BuildingEnum::NONE:
			if (!grid[GetIndex(cellToCheck)].isRenderable) return _data.SynergyNature;
			return 0;
			break;
		case BuildingEnum::RESIDENTIAL:
			return _data.SynergyResidential;
		case BuildingEnum::COMMERCIAL:
			return _data.SynergyCommercial;
		case BuildingEnum::INDUSTRIAL:
			return _data.SynergyIndustrial;
		case BuildingEnum::NATURE:
			return _data.SynergyNature;
		}
		std::cerr << "Error " << __FILE__ << "ln" << __LINE__ << " : UNABLE TO GET SYNERGY POINTS!\n";
		return 0;
	}

	void GridManager::CheckCellNeighbor(iso::cell* grid, Vec2<int> cellIndex)
	{
		int gridIndex = GetIndex(cellIndex.x, cellIndex.y);
		int matchCount{ 1 };
		int matchedCells[3]{ grid[gridIndex].ID };
		//We need to check for every cell that is in the building
		for (Vec2<int> cell : grid[gridIndex]._building.buildingCells) {
			//The order to check is CLOCKWISE, so we go NORTH, EAST, SOUTH, WEST
			gridIndex = GetIndex(cell);
			int NorthIndex = GetIndex(cell.x, cell.y - 1);
			int EastIndex = GetIndex(cell.x + 1, cell.y);
			int SouthIndex = GetIndex(cell.x, cell.y + 1);
			int WestIndex = GetIndex(cell.x - 1, cell.y);
			if (grid[NorthIndex].ID != 0 && grid[NorthIndex]._building == grid[gridIndex]._building && grid[NorthIndex].ID != grid[gridIndex].ID) {
				if (matchCount < 3) {
					if (!HasID(matchedCells, matchCount, grid[NorthIndex].ID)) {
						matchedCells[matchCount] = grid[NorthIndex].ID;
						matchCount++;
					}
				}
			}
			if (grid[EastIndex].ID != 0 && grid[EastIndex]._building == grid[gridIndex]._building && grid[EastIndex].ID != grid[gridIndex].ID) {
				if (matchCount < 3) {
					if (!HasID(matchedCells, matchCount, grid[EastIndex].ID)) {
						matchedCells[matchCount] = grid[EastIndex].ID;
						matchCount++;
					}
				}
			}
			if (grid[SouthIndex].ID != 0 && grid[SouthIndex]._building == grid[gridIndex]._building && grid[SouthIndex].ID != grid[gridIndex].ID) {
				if (matchCount < 3) {
					if (!HasID(matchedCells, matchCount, grid[SouthIndex].ID)) {
						matchedCells[matchCount] = grid[SouthIndex].ID;
						matchCount++;
					}
				}
			}
			if (grid[WestIndex].ID != 0 && grid[WestIndex]._building == grid[gridIndex]._building && grid[WestIndex].ID != grid[gridIndex].ID) {
				if (matchCount < 3) {
					if (!HasID(matchedCells, matchCount, grid[WestIndex].ID)) {
						matchedCells[matchCount] = grid[WestIndex].ID;
						matchCount++;
					}
				}
			}
		}
		if (matchCount == 2) {
			//Now we check using the selected neighbor
			int selectedNeighbor = GetIndexFromID(matchedCells[1]);
			for (Vec2<int> cell : grid[selectedNeighbor]._building.buildingCells) {
				selectedNeighbor = GetIndex(cell);
				int NorthIndex = selectedNeighbor - gridX;
				int EastIndex = selectedNeighbor + 1;
				int SouthIndex = selectedNeighbor + gridX;
				int WestIndex = selectedNeighbor - 1;
				//If it is not the cell you came from and the building types match
				if ((NorthIndex != gridIndex) && grid[NorthIndex]._building == grid[selectedNeighbor]._building && grid[NorthIndex].ID != grid[selectedNeighbor].ID) {
					if (matchCount < 3) {
						if (!HasID(matchedCells, matchCount, grid[NorthIndex].ID)) {
							matchedCells[matchCount] = grid[NorthIndex].ID;
							matchCount++;
						}
					}
				}
				if ((EastIndex != gridIndex) && grid[EastIndex]._building == grid[selectedNeighbor]._building && grid[EastIndex].ID != grid[selectedNeighbor].ID) {
					if (matchCount < 3) {
						if (!HasID(matchedCells, matchCount, grid[EastIndex].ID)) {
							matchedCells[matchCount] = grid[EastIndex].ID;
							matchCount++;
						}
					}
				}
				if ((SouthIndex != gridIndex) && grid[SouthIndex]._building == grid[selectedNeighbor]._building && grid[SouthIndex].ID != grid[selectedNeighbor].ID) {
					if (matchCount < 3) {
						if (!HasID(matchedCells, matchCount, grid[SouthIndex].ID)) {
							matchedCells[matchCount] = grid[SouthIndex].ID;
							matchCount++;
						}
					}
				}
				if ((WestIndex != gridIndex) && grid[WestIndex]._building == grid[selectedNeighbor]._building && grid[WestIndex].ID != grid[selectedNeighbor].ID) {
					if (matchCount < 3) {
						if (!HasID(matchedCells, matchCount, grid[WestIndex].ID)) {
							matchedCells[matchCount] = grid[WestIndex].ID;
							matchCount++;
						}
					}
				}
			}
		}
		std::cout << "match count is " << matchCount << '\n';
		//if more than 200 means it's lvl 3
		if (matchCount == 3 && grid[gridIndex]._building.data.level < BuildingEnum::L3) {
			for (int i{ 0 }; i < matchCount; ++i) {
				std::cout << "MATCH!\n" << "Match ID's : " << matchedCells[i] << '\n';
				int index = GetIndexFromID(matchedCells[i]);
				if (grid[index].ID == grid[gridIndex].ID) {
					std::cout << "CONTINUED!\n";
					for (Vec2<int>cell : grid[gridIndex]._building.buildingCells) {
						grid[GetIndex(cell)]._building.LevelUp();
					}
					continue;
				}
				// grid[c].ID = 0;
				if (grid[index]._building.data.size != Vec2<int>{1, 1}) {
					std::vector<Vec2<int>> tempCells{ grid[index]._building.buildingCells };
					for (Vec2<int> cell : tempCells) {
						// std::cout << "Building Cells to destroy : " << cell <<", ID : " << matchedCells[i] <<'\n';
						grid[GetIndex(cell.x, cell.y)].ID = 0;
						grid[GetIndex(cell.x, cell.y)]._building = Building{};
					}
				}
				else {
					grid[index].ID = 0;
					grid[index]._building = Building{};
				}
			}
			onMergeBuildings.Invoke();
			//then we recurse and check again till no matches
			CheckCellNeighbor(grid, cellIndex);
		}
		//NORTH
#if DEBUG
// std::cout << "Index : " << cellIndex.x << ", " << cellIndex.y << '\n';
// std::cout << "Selected : " << grid[gridIndex]._building.data.type << '\n';
// std::cout << "North : " << grid[NorthIndex]._building.data.type << '\n';
// std::cout << "East : " << grid[EastIndex]._building.data.type << '\n';
// std::cout << "South : " << grid[SouthIndex]._building.data.type << '\n';
// std::cout << "West : " << grid[WestIndex]._building.data.type << '\n';

		std::cout << "Is N true? : " << (grid[NorthIndex].ID != 0 && grid[NorthIndex]._building == grid[gridIndex]._building && grid[NorthIndex].ID != grid[gridIndex].ID) << '\n';
		std::cout << "Is E true? : " << (grid[EastIndex].ID != 0 && grid[EastIndex]._building == grid[gridIndex]._building && grid[EastIndex].ID != grid[gridIndex].ID) << '\n';
		std::cout << "Is S true? : " << (grid[SouthIndex].ID != 0 && grid[SouthIndex]._building == grid[gridIndex]._building && grid[SouthIndex].ID != grid[gridIndex].ID) << '\n';
		std::cout << "Is W true? : " << (grid[WestIndex].ID != 0 && grid[WestIndex]._building == grid[gridIndex]._building && grid[WestIndex].ID != grid[gridIndex].ID) << '\n';
#endif

	}

	int GetIndex(int x, int y)
	{
		return x + gridX * y;
	}
	int GetIndex(Vec2<int> cell) {
		return cell.x + gridX * cell.y;
	}
	bool HasID(int* array, int count, int ID) {
		if (count < 0) return false;
		std::cout << "Checking for ID: " << ID << '\n';
		for (int i{ 0 }; i < count; ++i) {
			if (*(array + i) == ID) {
				std::cout << "Has ID: " << *(array + i) << " == " << ID << '\n';
				return true;
			}
		}
		return false;
	}
	BuildingEnum::TYPE GetTypeFromIndex(Vec2<int> cell) {
		return grid[GetIndex(cell)]._building.data.type;
	}
	int GetIndexFromID(int ID) {
		for (int i{ 0 }; i < (gridX * gridY); ++i) {
			if (grid[i].ID == ID) return i;
		}
		std::cerr << "Error " << __FILE__ << "ln" << __LINE__ << ": UNABLE TO FIND INDEX FROM ID!\n";
		return 0;
	}
}