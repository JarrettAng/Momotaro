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
#include <iostream>
#include <MomoMaths.h>
#include <ColorTable.h>
#include <GridManager.h>
#include <PauseManager.h>
#include <BuildingManager.h>
#include <CardManager.h>
#include <UIManager.h>
#include <ScoreManager.h>
#include <AudioManager.h>
#include <FileIOManager.h>
namespace GridManager {
	///////////////////////////////////////////////////////////////////////////
	//GRID CONSTANTS
	const int tileWidth{ 140 };
	const int tileHeight{ tileWidth / 2 };
	int gridX{ 0 }, gridY{ 0 };				//total grid size
	///////////////////////////////////////////////////////////////////////////
	//GRID VARIABLES
	cell* grid;								//The main grid array
	int buildingID{ 0 };					//THIS ID IS FOR TRACKING BUILDINGS!
	int totalPoints{};						//Synergy point tabulation
	std::vector<Vec2<int>> CurrentBuildingCells{};		//For calculating synergy area
	std::vector<Vec2<int>> CurrentSynergyArea{};		//For displaying synergy area
	//CACHED GRID INDEX
	static int previousIndex{ -1 };
	static int currentIndex{ -2 };
	///////////////////////////////////////////////////////////////////////////
	//EVENT RELATED VARIABLES
	const BuildingData* selectedBuilding{};			//Selected building from cardmanager event
	EventSystem::Event<void> onMergeBuildings;		//Event on merge
	EventSystem::Event<void> onBoardFull;
	EventSystem::Event<int>  onTotalSynergyUpdate;

	//DEBUG VARIABLES
	int terrainNum{ 2 };
	int randomNature{ 0 };
	int playableArea{ 0 };
	BuildingEnum::ORIENTATION TestOrientation{ BuildingEnum::RIGHT };
	std::vector<BuildingData> _testBuildingVector;
	///////////////////////////////////////////////////////////////////////////
	//INITIALISE GRID 								
	///////////////////////////////////////////////////////////////////////////
	void Initialize(std::string const& filePath) {
		// grid = { new cell[gridX * gridY]{} };
		grid = FileIOManager::LoadGridFromFile(filePath);
		//GRID SET UP
		//Init a grid with 0 tiles
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);
				Vec2<int> ScreenPos = WorldIndexToScreenPos(x, y);
				//This is 10 units up so we need to move x index and y index up by 10 units
				ScreenPos.y += (gridY * tileHeight) / 2;		//move the grid up by half its size (20 units / 2 = 10)
				grid[index].pos = ScreenPos;

				if (grid[index].isRenderable) playableArea++;

				// //basically we want the grid to be from -2 to 2, but since there's a 10 unit offset, we add 10
				// if (((x >= (mapPos + gridX / 2)) && (x <= (mapPos + gridX / 2 + mapSize))) && (y >= (mapPos + gridY / 2) && y <= (mapPos + gridY / 2 + mapSize))) {
				// 	grid[index].isRenderable = true;
				// 	playableArea++;
				// }
				// grid[index].ID = 0;
			}
		}
		AudioManager::PlayBGM(AudioManager::ClipName::BGM_GAME);
		//////////////////////////////////////////////////////////////////////
		//					SUBSCRIBE EVENTS 								//
		//////////////////////////////////////////////////////////////////////

		InputManager::SubscribeToKey(AEVK_C, InputManager::TRIGGERED, ClearGrid);
		InputManager::SubscribeToKey(AEVK_1, InputManager::TRIGGERED, SpawnResidential);
		InputManager::SubscribeToKey(AEVK_2, InputManager::TRIGGERED, SpawnCommerical);
		InputManager::SubscribeToKey(AEVK_3, InputManager::TRIGGERED, SpawnIndustrial);
		InputManager::SubscribeToKey(AEVK_Q, InputManager::TRIGGERED, SpawnBigResidential);
		InputManager::SubscribeToKey(AEVK_W, InputManager::TRIGGERED, SpawnBigResidential3x1);
		InputManager::SubscribeToKey(AEVK_E, InputManager::TRIGGERED, SpawnBigResidential);
		InputManager::SubscribeToKey(AEVK_S, InputManager::TRIGGERED, FileIOManager::SaveGridToFile);
		InputManager::SubscribeToKey(AEVK_N, InputManager::TRIGGERED, SpawnNature);
		InputManager::SubscribeToKey(AEVK_T, InputManager::TRIGGERED, ToggleTileRenderable);

		CardManager::onNewCardSelected.Subscribe(GetBuildingCard);
		CardManager::onCardPlaced.Subscribe(SpawnBuilding);
	}

	///////////////////////////////////////////////////////////////////////////
	//Spawns buildings at mouse position
	///////////////////////////////////////////////////////////////////////////
	void SpawnBuilding(Vec2<int>mousePos) {
		if(PauseManager::IsPaused()) return;
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
		if (!isCellSafe(SelectedCell)) {
			std::cout << "Debug " << __FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
			return;
		}
		AudioManager::PlayAudioClip(AudioManager::ClipName::SFX_GAINPOINT);
		int index = GetIndex(SelectedCell);
		grid[index].ID = ++buildingID;
		grid[index]._building.data = *selectedBuilding;
		grid[index]._building.buildingCells = CurrentBuildingCells;
		CheckCellNeighbor(grid, SelectedCell);
		ScoreManager::AddScore(totalPoints);
		CurrentSynergyArea.clear();
		CurrentBuildingCells.clear();
		playableArea--;

		//!TODO CHANGE THIS TO FIT BIGGER SIZES!!
		if (playableArea < 1) {
			onBoardFull.Invoke();
		}
	}

	const cell* GetGrid() {
		return grid;
	}



	///////////////////////////////////////////////////////////////////////////
	//Checks if the selected cell is safe to place a building on
	///////////////////////////////////////////////////////////////////////////
	bool isCellSafe(Vec2<int> selectedCell) {
		if (!isWithinGrid(selectedCell)) return false;
		// if ((((selectedCell.x) < 0) || ((selectedCell.x) > gridX)) || ((selectedCell.y) < 0 || (selectedCell.y) > gridY)) return false;
		//If the tile is not even renderable, we cannot place
		if (!grid[GetIndex(selectedCell.x, selectedCell.y)].isRenderable) return false;
		//If there already is a building on that tile, we cannot place.
		if (grid[GetIndex(selectedCell.x, selectedCell.y)].ID > 0) return false;
		return true;
	}
	bool isWithinGrid(Vec2<int> selectedCell) {
		return (GetIndex(selectedCell) >= 0 && GetIndex(selectedCell) < (gridX * gridY));
	}

	///////////////////////////////////////////////////////////////////////////
	//Changes the building orientation (debugging)
	///////////////////////////////////////////////////////////////////////////
	void ChangeOrientation() {
		TestOrientation = static_cast<BuildingEnum::ORIENTATION>(TestOrientation + 1);
		if (TestOrientation == BuildingEnum::ORIENTATION_LENGTH) {
			TestOrientation = static_cast<BuildingEnum::ORIENTATION>(0);
		}
	}

#pragma region DEBUG_SPAWN_FUNCTIONS

	void SpawnBigResidential() {
		//1x2
		if (PauseManager::IsPaused()) return;
		ClearGrid();
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
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
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
		//int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;
		// ChangeOrientation();
		// SetGridIndex(TestOrientation, BigResidential3x1Lvl1.data, SelectedCell.x, SelectedCell.y);

	}

	void SpawnResidential() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;
		BuildingData _residential{ BuildingManager::GetBuildingData(
			BuildingEnum::RESIDENTIAL,
			Vec2<int>{1,1},
			BuildingEnum::L1
		) };
		grid[index].ID = ++buildingID;
		grid[index]._building = _residential;
		grid[index]._building.buildingCells.push_back(SelectedCell);
		// grid[index]._building.GetSynergyArea();
		// for (Vec2<int> cell : grid[index]._building.synergyAreaCells) {
		// 	// grid[GetIndex(cell)].isRenderable = false;
		// }
		CheckCellNeighbor(grid, SelectedCell);
	}
	void SpawnCommerical() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;

		BuildingData _commercial{ BuildingManager::GetBuildingData(
			BuildingEnum::COMMERCIAL,
			Vec2<int>{1,1},
			BuildingEnum::L1
		) };
		grid[index].ID = ++buildingID;
		grid[index]._building = _commercial;
		grid[index]._building.buildingCells.push_back(SelectedCell);
		CheckCellNeighbor(grid, SelectedCell);
	}
	void SpawnIndustrial() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;

		BuildingData _industrial{ BuildingManager::GetBuildingData(
			BuildingEnum::INDUSTRIAL,
			Vec2<int>{1,1},
			BuildingEnum::L1
		) };
		grid[index].ID = ++buildingID;
		grid[index]._building = _industrial;
		grid[index]._building.buildingCells.push_back(SelectedCell);
		CheckCellNeighbor(grid, SelectedCell);
	}
	void SpawnNature() {
		if (PauseManager::IsPaused()) return;
		// FileIOManager::LoadBuildingDataFromFile(_testBuildingVector);
		// FileIOManager::SaveBuildingDataToFile(_testBuildingVector);
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;
		grid[index].ID = ++buildingID;
		grid[index]._building = BuildingManager::GetRandomNatureBuilding();
	}

	cell NatureCell() {
		return cell{
			Vec2<int>{0,0},
			true,
			++buildingID,
			BuildingManager::GetRandomNatureBuilding()
		};
	}

	void ToggleTileRenderable() {
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isWithinGrid(SelectedCell)) return;
		grid[index].isRenderable = !grid[index].isRenderable;
		//If the grid is not renderable, we reset the cell.
		if (!grid[index].isRenderable) {
			grid[index].ID = 0;
			grid[index]._building = Building{};
		}
	}
#pragma endregion

	///////////////////////////////////////////////////////////////////////////
	//Gets building from cardmanager event
	///////////////////////////////////////////////////////////////////////////
	void GetBuildingCard(const BuildingData* _data) {
		// if (_data != nullptr)
		selectedBuilding = _data;		//we're caching it 
	}

	///////////////////////////////////////////////////////////////////////////
	//Checks if the current building orientation/position is valid to be placed
	///////////////////////////////////////////////////////////////////////////
	bool IsBuildingValid(const BuildingData* _data, int _x, int _y) {
		Vec2<int> _size = _data->size;
		Vec2<int> _SelectedCell{ 0,0 };
		//We need to check the orientation and rotate it accordingly
		if (_data->orientation == BuildingEnum::RIGHT || _data->orientation == BuildingEnum::LEFT) {
			_size = Vec2<int>{ _size.y,_size.x };
		}

		//We have to check the grid and see if the current orientation can be placed on the grid
		for (int y{ 0 }; y < _size.y; ++y) {
			for (int x{ 0 }; x < _size.x; ++x) {
				switch (_data->orientation)
				{
				case BuildingEnum::RIGHT:
					if (!isCellSafe(Vec2<int>{_x + x, y + _y})) {
						std::cout << "Debug " << __FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
						return false;
					}
					break;
				case BuildingEnum::TOP:
					if (!isCellSafe(Vec2<int>{_x + x, _y - y})) {
						std::cout << "Debug " << __FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
						return false;
					}
					break;
				case BuildingEnum::LEFT:
					if (!isCellSafe(Vec2<int>{_x - x, _y - y})) {
						std::cout << "Debug " << __FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
						return false;
					}
					break;
				case BuildingEnum::DOWN:
					if (!isCellSafe(Vec2<int>{_x - x, y + _y})) {
						std::cout << "Debug " << __FILE__ << "ln" << __LINE__ << ": Invalid position!\n";
						return false;
					}
					break;
				}
			}
		}
		return true;
	}

	///////////////////////////////////////////////////////////////////////////
	//Gets all the current building cell positions from the selected building
	///////////////////////////////////////////////////////////////////////////
	std::vector<Vec2<int>> GetBuildingCells(const BuildingData* _data, int _x, int _y) {
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
				AllCells.push_back(_SelectedCell);
			}
		}
		return AllCells;
	}

	///////////////////////////////////////////////////////////////////////////
	//Gets the synergy area cells from the building cells
	///////////////////////////////////////////////////////////////////////////
	std::vector<Vec2<int>> GetSynergyArea(std::vector<Vec2<int>> _buildingCells)
	{
		//If for some reason the building cells are empty, we need to throw an error
		if (CurrentBuildingCells.empty()) {
			std::cerr << "Error " << __FILE__ << "ln" << __LINE__ << " : NO BUILDING CELLS TO GET AREA!\n";
			//AE_ASSERT(CurrentBuildingCells.size());
			assert(CurrentBuildingCells.size());
			//assert("Error " << __FILE__ << "ln" << __LINE__ << " : NO BUILDING CELLS TO GET AREA!\n");
		}
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
		//This is because we do not want to include duplicate cells
		std::sort(tempVec.begin(), tempVec.end(), [](Vec2<int> a, Vec2<int> b) {return a < b; });	//once we sort it we prune
		auto last = std::unique(tempVec.begin(), tempVec.end());
		tempVec.erase(last, tempVec.end());
		return tempVec;
	}

	///////////////////////////////////////////////////////////////////////////
	//Checks if the mouse has changed index (Update loop for gridmanager)
	///////////////////////////////////////////////////////////////////////////
	void UpdateMouseToGrid() {
		AudioManager::Update();
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos{ InputManager::GetMousePos() };
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
		currentIndex = GetIndex(SelectedCell);
		//If the mouse has moved out of the previous index, we need to update the synergy cells
		if (currentIndex != previousIndex) {
			//First we clear out the vector
			CurrentSynergyArea.clear();
			CurrentBuildingCells.clear();
			//DRAWING DEBUG TEXT
			//Then we set the grid index
			if (selectedBuilding != nullptr) {
				if (IsBuildingValid(selectedBuilding, SelectedCell.x, SelectedCell.y)) {
					CurrentBuildingCells = GetBuildingCells(selectedBuilding, SelectedCell.x, SelectedCell.y);
					CurrentSynergyArea = GetSynergyArea(CurrentBuildingCells);

#if DEBUG_TEXT
					RenderSystem::AddTextToBatch(
						RenderSystem::GAME_PIECES_BATCH,
						((float)InputManager::GetMousePos().x / AEGetWindowWidth() * 2) - 1,
						(((float)InputManager::GetMousePos().y / AEGetWindowHeight() * 2) - 1) * -1,
						FontManager::GetFont(FontManager::ROBOTO),
						20,
						std::to_string(InputManager::GetMousePosDelta().x) + " , " + std::to_string(InputManager::GetMousePosDelta().y),
						99,
						COLOR_BLACK
					);
#endif

				}


			}
		}
		previousIndex = currentIndex;
	}
	///////////////////////////////////////////////////////////////////////////
	//Clears the grid
	///////////////////////////////////////////////////////////////////////////
	void ClearGrid() {
		if (PauseManager::IsPaused()) return;

		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);
				grid[index].ID = NONE;
				grid[index]._building = Building{};
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	//Draw function for the grid
	///////////////////////////////////////////////////////////////////////////
	void PrepareTileRenderBatch() {

		///////////////////////////////////////////////////////////////////////////
		//TILE RENDERING
		///////////////////////////////////////////////////////////////////////////
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);
				//Draws the normal tiles (if it's water don't draw any)
				if (grid[index].ID > 0) {
					RenderSystem::AddRectToBatch(
						RenderSystem::GAME_PIECES_BATCH,
						static_cast<float>(grid[index].pos.x), static_cast<float>(grid[index].pos.y),
						tileWidth, tileWidth,
						grid[index]._building.data.TextureID, 99
					);
				}
				// Draw tile.
				if (grid[index].isRenderable) {
					RenderSystem::AddRectToBatch(RenderSystem::TILE_BATCH, static_cast<float>(grid[index].pos.x), static_cast<float>(grid[index].pos.y), tileWidth, tileWidth, TextureManager::TILE_TEX);
				}
				// Draw water.
				else {
					RenderSystem::AddRectToBatch(RenderSystem::TILE_BATCH, static_cast<float>(grid[index].pos.x), static_cast<float>(grid[index].pos.y), tileWidth, tileWidth, TextureManager::WATER_TEX);
				}
			}
		}
		if (selectedBuilding != nullptr) {
			//If we have a building selected from a card, draw it at the mouse pos 
			//If the building is bigger than 1x1, draw all of them
			if (!CurrentBuildingCells.empty()) {
				for (Vec2<int> cell : CurrentBuildingCells) {
					RenderSystem::AddRectToBatch(
						RenderSystem::GAME_PIECES_BATCH,
						static_cast<float>(grid[GetIndex(cell)].pos.x), static_cast<float>(grid[GetIndex(cell)].pos.y + 12.5f),
						tileWidth, tileWidth,
						selectedBuilding->TextureID, 99, 0
					);
				}
			}
			//loops through the synergy area for the building and draws it
			//also draws the points 
			if (!CurrentSynergyArea.empty()) {
				UI::TextBox pointText;
				Vec2<float> pointTextPos;
				int points{};
				totalPoints = 0;
				Vec3<float> pointsColor;
				Vec3<float> totalPointsColor;

				Vec2<int> mousePos = InputManager::GetMousePos();
				//Convert the mouse position into iso
				Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
				for (Vec2<int> cell : CurrentSynergyArea) {
					pointTextPos.x = static_cast<float>(grid[GetIndex(cell)].pos.x) + tileWidth/6.f;
					pointTextPos.y = static_cast<float>(grid[GetIndex(cell)].pos.y) - (tileHeight*1.2f);
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
						pointText = UI::TextBox(pointTextPos, std::to_string(points), UI::CENTER_JUSTIFY, 100, 25, pointsColor);
					}
					pointText.Render();
					//Draw synergy texture
					RenderSystem::AddRectToBatch(
						RenderSystem::GAME_PIECES_BATCH,
						static_cast<float>(grid[GetIndex(cell)].pos.x), static_cast<float>(grid[GetIndex(cell)].pos.y),
						tileWidth, tileWidth,
						TextureManager::SYNERGY
					);
				}

				onTotalSynergyUpdate.Invoke(totalPoints);

				if (totalPoints > 0) {
					totalPointsColor = COLOR_BOX_POSITIVE;
				}
				else if (totalPoints < 0) {
					totalPointsColor = COLOR_BOX_NEGATIVE;
				}
				else {
					totalPointsColor = COLOR_BOX_NEUTRAL;
				}
				Vec2<float> totalPointTexPos{ static_cast<float>(grid[GetIndex(SelectedCell)].pos.x-(tileWidth/4.f)),static_cast<float>(grid[GetIndex(SelectedCell)].pos.y) - (tileHeight) };
				pointText = UI::TextBox(totalPointTexPos, std::to_string(totalPoints), UI::CENTER_JUSTIFY, 240, 42, totalPointsColor);
				pointText.Render();
				return;

			}
		}
		onTotalSynergyUpdate.Invoke(0);
	}

	///////////////////////////////////////////////////////////////////////////
	//Gets the synergy point calculation for the cell
	///////////////////////////////////////////////////////////////////////////	
	int GetSynergyText(Vec2<int> cellToCheck, BuildingData _data) {
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
		// assert(0);
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////
	//Merge algorithm
	///////////////////////////////////////////////////////////////////////////
	void CheckCellNeighbor(cell* _grid, Vec2<int> cellIndex)
	{
		int gridIndex = GetIndex(cellIndex.x, cellIndex.y);
		int matchCount{ 1 };
		int matchedCells[3]{ _grid[gridIndex].ID };
		//We need to check for every cell that is in the building
		for (Vec2<int> cell : _grid[gridIndex]._building.buildingCells) {
			//The order to check is CLOCKWISE, so we go NORTH, EAST, SOUTH, WEST
			gridIndex = GetIndex(cell);
			int NorthIndex = GetIndex(cell.x, cell.y - 1);
			int EastIndex = GetIndex(cell.x + 1, cell.y);
			int SouthIndex = GetIndex(cell.x, cell.y + 1);
			int WestIndex = GetIndex(cell.x - 1, cell.y);
			if (_grid[NorthIndex].ID != 0 && _grid[NorthIndex]._building == _grid[gridIndex]._building && _grid[NorthIndex].ID != _grid[gridIndex].ID) {
				if (matchCount < 3) {
					if (!HasID(matchedCells, matchCount, _grid[NorthIndex].ID)) {
						matchedCells[matchCount] = _grid[NorthIndex].ID;
						matchCount++;
					}
				}
			}
			if (_grid[EastIndex].ID != 0 && _grid[EastIndex]._building == _grid[gridIndex]._building && _grid[EastIndex].ID != _grid[gridIndex].ID) {
				if (matchCount < 3) {
					if (!HasID(matchedCells, matchCount, _grid[EastIndex].ID)) {
						matchedCells[matchCount] = _grid[EastIndex].ID;
						matchCount++;
					}
				}
			}
			if (_grid[SouthIndex].ID != 0 && _grid[SouthIndex]._building == _grid[gridIndex]._building && _grid[SouthIndex].ID != _grid[gridIndex].ID) {
				if (matchCount < 3) {
					if (!HasID(matchedCells, matchCount, _grid[SouthIndex].ID)) {
						matchedCells[matchCount] = _grid[SouthIndex].ID;
						matchCount++;
					}
				}
			}
			if (_grid[WestIndex].ID != 0 && _grid[WestIndex]._building == _grid[gridIndex]._building && _grid[WestIndex].ID != _grid[gridIndex].ID) {
				if (matchCount < 3) {
					if (!HasID(matchedCells, matchCount, _grid[WestIndex].ID)) {
						matchedCells[matchCount] = _grid[WestIndex].ID;
						matchCount++;
					}
				}
			}
		}
		if (matchCount == 2) {
			//Now we check using the selected neighbor
			int selectedNeighbor = GetIndexFromID(matchedCells[1]);
			for (Vec2<int> cell : _grid[selectedNeighbor]._building.buildingCells) {
				selectedNeighbor = GetIndex(cell);
				int NorthIndex = selectedNeighbor - gridX;
				int EastIndex = selectedNeighbor + 1;
				int SouthIndex = selectedNeighbor + gridX;
				int WestIndex = selectedNeighbor - 1;
				//If it is not the cell you came from and the building types match
				if ((NorthIndex != gridIndex) && _grid[NorthIndex]._building == _grid[selectedNeighbor]._building && _grid[NorthIndex].ID != _grid[selectedNeighbor].ID) {
					if (matchCount < 3) {
						if (!HasID(matchedCells, matchCount, _grid[NorthIndex].ID)) {
							matchedCells[matchCount] = _grid[NorthIndex].ID;
							matchCount++;
						}
					}
				}
				if ((EastIndex != gridIndex) && _grid[EastIndex]._building == _grid[selectedNeighbor]._building && _grid[EastIndex].ID != _grid[selectedNeighbor].ID) {
					if (matchCount < 3) {
						if (!HasID(matchedCells, matchCount, _grid[EastIndex].ID)) {
							matchedCells[matchCount] = _grid[EastIndex].ID;
							matchCount++;
						}
					}
				}
				if ((SouthIndex != gridIndex) && _grid[SouthIndex]._building == _grid[selectedNeighbor]._building && _grid[SouthIndex].ID != _grid[selectedNeighbor].ID) {
					if (matchCount < 3) {
						if (!HasID(matchedCells, matchCount, _grid[SouthIndex].ID)) {
							matchedCells[matchCount] = _grid[SouthIndex].ID;
							matchCount++;
						}
					}
				}
				if ((WestIndex != gridIndex) && _grid[WestIndex]._building == _grid[selectedNeighbor]._building && _grid[WestIndex].ID != _grid[selectedNeighbor].ID) {
					if (matchCount < 3) {
						if (!HasID(matchedCells, matchCount, _grid[WestIndex].ID)) {
							matchedCells[matchCount] = _grid[WestIndex].ID;
							matchCount++;
						}
					}
				}
			}
		}
		std::cout << "match count is " << matchCount << '\n';
		if (matchCount == 3 && _grid[gridIndex]._building.data.level < BuildingEnum::L3) {

			for (int i{ 0 }; i < matchCount; ++i) {
				std::cout << "MATCH!\n" << "Match ID's : " << matchedCells[i] << '\n';
				int index = GetIndexFromID(matchedCells[i]);
				if (grid[index].ID == _grid[gridIndex].ID) {
					std::cout << "CONTINUED!\n";
					for (Vec2<int>cell : _grid[gridIndex]._building.buildingCells) {
						_grid[GetIndex(cell)]._building.LevelUp();
					}
					continue;
				}
				// grid[c].ID = 0;
				if (_grid[index]._building.data.size != Vec2<int>{1, 1}) {
					std::vector<Vec2<int>> tempCells{ _grid[index]._building.buildingCells };
					for (Vec2<int> cell : tempCells) {
						// std::cout << "Building Cells to destroy : " << cell <<", ID : " << matchedCells[i] <<'\n';
						_grid[GetIndex(cell.x, cell.y)].ID = 0;
						_grid[GetIndex(cell.x, cell.y)]._building = Building{};
					}
				}
				else {
					_grid[index].ID = 0;
					_grid[index]._building = Building{};
				}
			}
			 AudioManager::PlayAudioClip(AudioManager::ClipName::SFX_MERGE1);
			// if(_grid[gridIndex]._building.data.level == BuildingEnum::L3) AudioManager::PlayAudioClip(AudioManager::ClipName::SFX_MERGE2);
			onMergeBuildings.Invoke();
			//then we recurse and check again till no matches
			CheckCellNeighbor(_grid, cellIndex);
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


	///////////////////////////////////////////////////////////////////////////
	//Free function for the grid
	///////////////////////////////////////////////////////////////////////////
	void Free() {
		delete[] grid;
		CurrentSynergyArea.clear();
		CurrentBuildingCells.clear();
		selectedBuilding = nullptr;
		InputManager::UnsubscribeKey(AEVK_C, InputManager::TRIGGERED, ClearGrid);
		InputManager::UnsubscribeKey(AEVK_1, InputManager::TRIGGERED, SpawnResidential);
		InputManager::UnsubscribeKey(AEVK_2, InputManager::TRIGGERED, SpawnCommerical);
		InputManager::UnsubscribeKey(AEVK_3, InputManager::TRIGGERED, SpawnIndustrial);
		InputManager::UnsubscribeKey(AEVK_Q, InputManager::TRIGGERED, SpawnBigResidential);
		InputManager::UnsubscribeKey(AEVK_W, InputManager::TRIGGERED, SpawnBigResidential3x1);
		InputManager::UnsubscribeKey(AEVK_E, InputManager::TRIGGERED, SpawnBigResidential);
		InputManager::UnsubscribeKey(AEVK_S, InputManager::TRIGGERED, SpawnBigResidential);
		InputManager::UnsubscribeKey(AEVK_N, InputManager::TRIGGERED, SpawnNature);

		CardManager::onNewCardSelected.Unsubscribe(GetBuildingCard);
		CardManager::onCardPlaced.Unsubscribe(SpawnBuilding);

	}


	///////////////////////////////////////////////////////////////////////////
	//GRID HELPER FUNCTIONS
	///////////////////////////////////////////////////////////////////////////
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

	float area(int x1, int y1, int x2, int y2, int x3, int y3) {
		return static_cast<float>(abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0f));
	}

	bool isInside(int _mouseX, int _mouseY, int x1, int y1, int x2, int y2, int x3, int y3) {
		float A = area(x1, y1, x2, y2, x3, y3);
		float B = area(_mouseX, _mouseY, x2, y2, x3, y3);
		float C = area(x1, y1, _mouseX, _mouseY, x3, y3);
		float D = area(x1, y1, x2, y2, _mouseX, _mouseY);
		return (A == (B + C + D));
	}

	Vec2<int> ToScreen(int x, int y) {
		int originX = AEGetWindowWidth() / 2 / tileWidth;
		int originY = AEGetWindowHeight() / 2 / tileHeight;
		return Vec2<int>{
			(originX* tileWidth) + (x - y) * (tileWidth / 2),
				(originY* tileHeight) + (x + y) * (tileHeight / 2)
		};
	}

	Vec2<int> WorldIndexToScreenPos(int x, int y) {
		return Vec2<int>{   //we need to keep the tile height and width a float here!
			static_cast<int>((x - y)* (tileWidth / 2.f)),
			static_cast<int>((x + y) * -(tileHeight / 2.f) + (tileHeight/2.f))		//offset for the correct pos because of the height diff
		};
	}
	Vec2<int> ScreenPosToIso(Vec2<int> cellPos) {
		return ScreenPosToIso(cellPos.x, cellPos.y);
	}
	Vec2<int> ScreenPosToIso(int xPos, int yPos) {
		//MOUSE INPUTS (Tile width = 100, tile height = 50)
		int cellX = xPos / tileWidth;
		int cellY = yPos / tileHeight;
		//int index = cellX + 30 * cellY;

		int xOffset = xPos % tileWidth;
		int yOffset = yPos % tileHeight;
		//Origin -> screen/tile. For now I use numbers
		int originX = AEGetWindowWidth() / 2 / tileWidth;
		int originY = AEGetWindowHeight() / 2 / tileHeight;

		Vec2<int> SelectedCell{
			((cellX - originX) + (cellY - originY) + gridX / 2) - 1,	//x
			(cellY - originY) - (cellX - originX) + gridY / 2			//y
		};
		//TOP LEFT
		if (isInside(xOffset, yOffset, 0, 0, 0, tileHeight / 2, tileWidth / 2, 0))SelectedCell.x--;
		//BOTTOM LEFT
		if (isInside(xOffset, yOffset, 0, tileHeight / 2, 0, tileWidth / 2, tileWidth / 2, tileHeight))SelectedCell.y++;
		//TOP RIGHT
		if (isInside(xOffset, yOffset, tileWidth / 2, 0, tileWidth, 0, tileWidth, tileHeight / 2))SelectedCell.y--;
		//BOTTOM RIGHT
		if (isInside(xOffset, yOffset, tileWidth / 2, tileHeight, tileWidth, tileHeight, tileWidth, tileHeight / 2))SelectedCell.x++;
		return SelectedCell;
	}
	Vec2<int> MouseToCell(int mouseX, int mouseY) {
		return Vec2<int>{
			mouseX / tileWidth,
				mouseY / tileHeight
		};
	}

	Vec2<int> MouseCellOffset(int mouseX, int mouseY)
	{
		return Vec2<int>{
			mouseX% tileWidth,
				mouseY% tileHeight
		};
	}

}