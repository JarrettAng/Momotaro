/*!************************************************************************
\file:          GridManager.cpp
\author:		Amadeus Chia
\par DP email:	amadeusjinhan.chia@digipen.edu
\par Course:    CSD1451B
\par Software Engineering Project
\date:          30-01-2023
\brief
This source file implements functions that manage the game grid.

The important functions to take note are :
- CheckCellNeighbor 
Function that handles merging (recurses to handle multiple merges in 1 frame)

- PrepareRenderBatch
Draws all the items in the grid

- SpawnBuilding
Handles the spawning of buildings on mouseclick

Check header file for more detailed explanations for functions

All content � 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
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
#include <DebugManager.h>
namespace GridManager {
	///////////////////////////////////////////////////////////////////////////
	//GRID CONSTANTS
	const int tileWidth{ 140 };
	const int tileHeight{ tileWidth / 2 };
	int gridX{ 0 }, gridY{ 0 };				//total grid size
	///////////////////////////////////////////////////////////////////////////
	//GRID VARIABLES
	cell* grid;								//The main grid array
	int buildingID{ 1 };					//THIS ID IS FOR TRACKING BUILDINGS!
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
	void InstantWin();
	///////////////////////////////////////////////////////////////////////////
	//INITIALISE GRID 								
	///////////////////////////////////////////////////////////////////////////
	void Initialize(std::string const& filePath) {
		playableArea =0;
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

				if (grid[index].isRenderable&&grid[index]._building.data.type == BuildingEnum::NONE) playableArea++;
			}
		}
		AudioManager::PlayBGM(AudioManager::ClipName::BGM_GAME);
		//////////////////////////////////////////////////////////////////////
		//					SUBSCRIBE EVENTS 								//
		//////////////////////////////////////////////////////////////////////
		//(DEBUG MODE EVENTS!)
		InputManager::SubscribeToKey(AEVK_C, InputManager::TRIGGERED, ClearGrid);
		InputManager::SubscribeToKey(AEVK_1, InputManager::TRIGGERED, SpawnResidential);
		InputManager::SubscribeToKey(AEVK_2, InputManager::TRIGGERED, SpawnCommerical);
		InputManager::SubscribeToKey(AEVK_3, InputManager::TRIGGERED, SpawnIndustrial);
		InputManager::SubscribeToKey(AEVK_W, InputManager::TRIGGERED, InstantWin);
		//(IN GAME EVENTS!)
		CardManager::onNewCardSelected.Subscribe(GetBuildingCard);
		CardManager::onCardPlaced.Subscribe(SpawnBuilding);
	}

	///////////////////////////////////////////////////////////////////////////
	//Causes an instant win (DEBUG MODE)
	///////////////////////////////////////////////////////////////////////////
	void InstantWin(){
		if(Debug::IsDebugModeOn()) onBoardFull.Invoke();
	}
	///////////////////////////////////////////////////////////////////////////
	//Spawns buildings at mouse position
	///////////////////////////////////////////////////////////////////////////
	void SpawnBuilding(Vec2<int>mousePos) {
		if(PauseManager::IsPaused()) return;
		//First we convert the mouse pos to isometric grid pos, then we check if the cell is placeable
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
		if (!isCellSafe(SelectedCell)) {
			std::string debugLog("Debug ");
			Debug::Print(debugLog + __FILE__ + "ln" + std::to_string(__LINE__) + ": Invalid position!\n");
			return;
		}
		//If the cell is placeable, we play the sounds and do the necessary things
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
		if (playableArea < 1) {
			onBoardFull.Invoke();
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//Draws a tile over the cursor for the editor
	///////////////////////////////////////////////////////////////////////////	
	void DrawSelectedTile(Vec2<int> mousePos){
		Vec2<int> SelectedCell{ScreenPosToIso(mousePos.x,mousePos.y)};
		if(!isWithinGrid(SelectedCell)){
			std::string debugLog("Debug ");
			Debug::Print(debugLog + __FILE__ + "ln" + std::to_string(__LINE__) + ": Invalid position!\n");
		}
		int index = GetIndex(SelectedCell);
		RenderSystem::AddRectToBatch(
						RenderSystem::UI_BATCH,
						static_cast<float>(grid[index].pos.x), static_cast<float>(grid[index].pos.y),
						tileWidth, tileWidth,
						TextureManager::SYNERGY,
						-1
					);
	}

	///////////////////////////////////////////////////////////////////////////
	//Checks if the selected cell is safe to place a building on
	///////////////////////////////////////////////////////////////////////////
	bool isCellSafe(Vec2<int> selectedCell) {
		if (!isWithinGrid(selectedCell)) return false;
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

	///////////////////////////////////////////////////////////////////////////
	//Debug functions related to spawning (in editor)
	///////////////////////////////////////////////////////////////////////////
#pragma region DEBUG_SPAWN_FUNCTIONS

	//Spawns a residential building
	void SpawnResidential() {
		if (PauseManager::IsPaused() || !Debug::IsDebugModeOn()) return;
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
	
		CheckCellNeighbor(grid, SelectedCell);
	}

	//Spawns a commercial building
	void SpawnCommerical() {
		if (PauseManager::IsPaused() || !Debug::IsDebugModeOn()) return;
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

	//Spawns an industrial building
	void SpawnIndustrial() {
		if (PauseManager::IsPaused() || !Debug::IsDebugModeOn()) return;
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

	//Spawns a nature tile
	void SpawnNature() {
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;
		grid[index].ID = 1;
		grid[index]._building = BuildingManager::GetRandomNatureBuilding();
	}

	//Gets a random nature cell (used in the level editor and level loading)
	cell NatureCell() {
		return cell{
			Vec2<int>{1,1},
			true,
			1,
			BuildingManager::GetRandomNatureBuilding()
		};
	}

	//Toggles a renderable tile on or off (used in editor)
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
						return false;
					}
					break;
				case BuildingEnum::TOP:
					if (!isCellSafe(Vec2<int>{_x + x, _y - y})) {
						return false;
					}
					break;
				case BuildingEnum::LEFT:
					if (!isCellSafe(Vec2<int>{_x - x, _y - y})) {
						return false;
					}
					break;
				case BuildingEnum::DOWN:
					if (!isCellSafe(Vec2<int>{_x - x, y + _y})) {
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
		for (int y{ 0 }; y < _size.y; ++y) {
			for (int x{ 0 }; x < _size.x; ++x) {
				int otherIndex{ 0 };
				//This accounts for orientation for bigger buildings
				//*Note: sadly bigger buildings were cut out but this works!
				switch (_data->orientation)
				{
				case BuildingEnum::RIGHT:
					otherIndex = GetIndex(_x + x, y + _y);
					_SelectedCell = { _x + x,_y + y };
					break;
				case BuildingEnum::TOP:
					otherIndex = GetIndex(_x + x, _y - y);
					_SelectedCell = { _x + x,_y - y };
					break;
				case BuildingEnum::LEFT:
					otherIndex = GetIndex(_x - x, _y - y);
					_SelectedCell = { _x - x,_y - y };
					break;
				case BuildingEnum::DOWN:
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
			assert(CurrentBuildingCells.size());
		}
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
					if(Debug::IsDebugModeOn()){
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
					}
				}
			}
		}
		previousIndex = currentIndex;
	}
	///////////////////////////////////////////////////////////////////////////
	//Clears the grid (DEBUG MODE FUNCTION!)
	///////////////////////////////////////////////////////////////////////////
	void ClearGrid() {
		if (PauseManager::IsPaused() || !Debug::IsDebugModeOn()) return;
		playableArea = 0;
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);
				grid[index].ID = NONE;
				grid[index]._building = Building{};
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	//Resets the grid by removing all player placed buildings
	///////////////////////////////////////////////////////////////////////////
	void ResetGrid(){
		for (int y{ 0 }; y < gridY; ++y) {
			for (int x{ 0 }; x < gridX; ++x) {
				int index = GetIndex(x, y);
				switch(grid[index]._building.data.type){
					case BuildingEnum::NATURE: break;
					case BuildingEnum::NONE: break;
					default:
						grid[index].ID = NONE;
						grid[index]._building = Building{};
					break;
				}
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
		UI::TextBox debugTextBox;
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
					if(Debug::IsDebugModeOn()){
						debugTextBox = UI::TextBox(
							Vec2<float>{
								static_cast<float>(grid[index].pos.x) - tileWidth/6.f,
								static_cast<float>(grid[index].pos.y) - (tileHeight*1.2f)},
							 std::to_string(grid[index].ID) + ", " + 
							 std::to_string(grid[index]._building.data.type) + ", " +
							 std::to_string(grid[index]._building.data.level)
							 , 
							 UI::CENTER_JUSTIFY, 
							 240, 22, 
							 COLOR_RED);
						debugTextBox.Render();
					}
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

		//If we have a building selected from a card, draw it at the mouse pos 
		if (selectedBuilding != nullptr) {
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
						RenderSystem::UI_BATCH,
						static_cast<float>(grid[GetIndex(cell)].pos.x), static_cast<float>(grid[GetIndex(cell)].pos.y),
						tileWidth, tileWidth,
						TextureManager::SYNERGY,
						-1
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
		
		Debug::PrintError(__FILE__, __LINE__, "UNABLE TO GET SYNERGY POINTS!");
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
		if (matchCount == 3 && _grid[gridIndex]._building.data.level < BuildingEnum::L3) {

			for (int i{ 0 }; i < matchCount; ++i) {
				int index = GetIndexFromID(matchedCells[i]);
				if (grid[index].ID == _grid[gridIndex].ID) {
					for (Vec2<int>cell : _grid[gridIndex]._building.buildingCells) {
						_grid[GetIndex(cell)]._building.LevelUp();
					}
					continue;
				}
				// grid[c].ID = 0;
				if (_grid[index]._building.data.size != Vec2<int>{1, 1}) {
					std::vector<Vec2<int>> tempCells{ _grid[index]._building.buildingCells };
					for (Vec2<int> cell : tempCells) {
						_grid[GetIndex(cell.x, cell.y)].ID = 0;
						_grid[GetIndex(cell.x, cell.y)]._building = Building{};
					}
				}
				else {
					_grid[index].ID = 0;
					_grid[index]._building = Building{};
				}
			}
			playableArea+=2;
			 AudioManager::PlayAudioClip(AudioManager::ClipName::SFX_MERGE1);
			onMergeBuildings.Invoke();
			//then we recurse and check again till no matches
			CheckCellNeighbor(_grid, cellIndex);
		}
	}


	///////////////////////////////////////////////////////////////////////////
	//Free function for the grid
	///////////////////////////////////////////////////////////////////////////
	void Free() {
		delete[] grid;
		// buildingID = 0;
		CurrentSynergyArea.clear();
		CurrentBuildingCells.clear();
		selectedBuilding = nullptr;
		InputManager::UnsubscribeKey(AEVK_C, InputManager::TRIGGERED, ClearGrid);
		InputManager::UnsubscribeKey(AEVK_1, InputManager::TRIGGERED, SpawnResidential);
		InputManager::UnsubscribeKey(AEVK_2, InputManager::TRIGGERED, SpawnCommerical);
		InputManager::UnsubscribeKey(AEVK_3, InputManager::TRIGGERED, SpawnIndustrial);
		InputManager::UnsubscribeKey(AEVK_W, InputManager::TRIGGERED, InstantWin);
		CardManager::onNewCardSelected.Unsubscribe(GetBuildingCard);
		CardManager::onCardPlaced.Unsubscribe(SpawnBuilding);

	}


	///////////////////////////////////////////////////////////////////////////
	//GRID HELPER FUNCTIONS
	///////////////////////////////////////////////////////////////////////////

	//Accessor for the grid (used to save in fileio)
	const cell* GetGrid() {
		return grid;
	}

	//Setter for building IDs (used in fileio)
	void SetBuildingID(int _id){
		buildingID = _id;
	}

	//Helper function to convert 2 ints into grid index
	int GetIndex(int x, int y)
	{
		return x + gridX * y;
	}

	//Overloaded helper function to convert Vec2<int> to grid index
	int GetIndex(Vec2<int> cell) {
		return cell.x + gridX * cell.y;
	}

	//Checks if a tile with the specified ID exists in the grid
	bool HasID(int* array, int count, int ID) {
		if (count < 0) return false;
		for (int i{ 0 }; i < count; ++i) {
			if (*(array + i) == ID) {
				return true;
			}
		}
		return false;
	}

	//Gets the building type from the cell specified
	BuildingEnum::TYPE GetTypeFromIndex(Vec2<int> cell) {
		return grid[GetIndex(cell)]._building.data.type;
	}

	//Returns the index of the building from the ID given
	int GetIndexFromID(int ID) {
		for (int i{ 0 }; i < (gridX * gridY); ++i) {
			if (grid[i].ID == ID) return i;
		}

		std::string debugLog;
		Debug::PrintError(__FILE__, __LINE__, "UNABLE TO FIND INDEX FROM ID!");
		return 0;
	}

	//Function to calculate the area of a triangle using 3 points (used for isometric corner checking)
	float area(int x1, int y1, int x2, int y2, int x3, int y3) {
		return static_cast<float>(abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0f));
	}

	//Checks if the mouse is within the area of the triangle specified
	bool isInside(int _mouseX, int _mouseY, int x1, int y1, int x2, int y2, int x3, int y3) {
		float A = area(x1, y1, x2, y2, x3, y3);
		float B = area(_mouseX, _mouseY, x2, y2, x3, y3);
		float C = area(x1, y1, _mouseX, _mouseY, x3, y3);
		float D = area(x1, y1, x2, y2, _mouseX, _mouseY);
		return (A == (B + C + D));
	}

	//Converts 2 ints into vec2<int> of the screen
	Vec2<int> ToScreen(int x, int y) {
		int originX = AEGetWindowWidth() / 2 / tileWidth;
		int originY = AEGetWindowHeight() / 2 / tileHeight;
		return Vec2<int>{
			(originX* tileWidth) + (x - y) * (tileWidth / 2),
				(originY* tileHeight) + (x + y) * (tileHeight / 2)
		};
	}

	//Converts the grid index to screen position
	Vec2<int> WorldIndexToScreenPos(int x, int y) {
		return Vec2<int>{   //we need to keep the tile height and width a float here!
			static_cast<int>((x - y)* (tileWidth / 2.f)),
			static_cast<int>((x + y) * -(tileHeight / 2.f) + (tileHeight/2.f))		//offset for the correct pos because of the height diff
		};
	}

	//Overloaded helper function to convert screen pos in the form of Vec2<int> to Vec2<int>
	Vec2<int> ScreenPosToIso(Vec2<int> cellPos) {
		return ScreenPosToIso(cellPos.x, cellPos.y);
	}

	//Helper function to convert the screen pos to vec2<int> cell index  
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

	//Function that directly converts mousePosition to non-isometric grid index
	Vec2<int> MouseToCell(int mouseX, int mouseY) {
		return Vec2<int>{
			mouseX / tileWidth,
				mouseY / tileHeight
		};
	}

	//Function that calculates mouse offset into grid cell
	Vec2<int> MouseCellOffset(int mouseX, int mouseY)
	{
		return Vec2<int>{
			mouseX% tileWidth,
				mouseY% tileHeight
		};
	}


	///////////////////////////////////////////////////////////////////////////
	//DEPRECATED/SCRAPPED BUT WORKING FEATURES
	///////////////////////////////////////////////////////////////////////////
	#pragma region Deprecated Code
	//F in the chat for my bigger buildings
	void SpawnBigResidential() {
		//1x2
		if (PauseManager::IsPaused()) return;
		Vec2<int> mousePos = InputManager::GetMousePos();
		Vec2<int> SelectedCell{ ScreenPosToIso(mousePos.x,mousePos.y) };
		int index = GetIndex(SelectedCell.x, SelectedCell.y);
		if (!isCellSafe(SelectedCell)) return;
		ChangeOrientation();
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

	}
	#pragma endregion
}