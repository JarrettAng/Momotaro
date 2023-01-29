
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
			(cellX - originX) + (cellY - originY)+10,		//x
			(cellY - originY) - (cellX - originX)+10		//y
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
		if((((SelectedCell.x) < 0) || ((SelectedCell.x) > gridX)) || ((SelectedCell.y) < 0 || (SelectedCell.y) > gridY)) return; 
		index = (SelectedCell.x)+gridX*(SelectedCell.y);
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
					case iso::RESIDENTIAL+100:
					RenderSystem::AddSpriteBatch(RenderSystem::BUILDING_BATCH, RenderSystem::BUILDING,TextureManager::RESIDENTIAL_M, grid[index].pos.x, grid[index].pos.y);
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

    void GridManager::CheckCellNeighbor(iso::cell* grid,iso::vec2i cellIndex)
    {
		//The order to check is CLOCKWISE, so we go NORTH, EAST, SOUTH, WEST
		// // //Note that Alpha engine Y minus GOES UP!!!
		// iso::vec2i NorthCell{cellIndex.x,cellIndex.y--};
		// iso::vec2i EastCell{cellIndex.x++,cellIndex.y};
		// iso::vec2i SouthCell{cellIndex.x,cellIndex.y++};
		// iso::vec2i WestCell{cellIndex.x--,cellIndex.y};
		// iso::cell test{GetNeighbor(grid,NorthCell,cellIndex)};
		int matchCount{0};
		int matchedCells[2];
		int gridIndex = GetIndex(cellIndex.x,cellIndex.y);
		int NorthIndex = GetIndex(cellIndex.x,cellIndex.y-1);
		int EastIndex = GetIndex(cellIndex.x+1,cellIndex.y);
		int SouthIndex = GetIndex(cellIndex.x,cellIndex.y+1);
		int WestIndex = GetIndex(cellIndex.x-1,cellIndex.y);
		//NORTH
		// std::cout << "Cell index : " << cellIndex.x<< ", " << cellIndex.y << '\n';
		std::cout << "Selected : "<< grid[gridIndex].ID << '\n';
		std::cout << "North : " << grid[NorthIndex].ID << '\n';
		std::cout << "West : " << grid[WestIndex].ID << '\n';
		std:: cout << "Is true? : " << (grid[NorthIndex].ID == grid[gridIndex].ID) << '\n';
		std:: cout << "Is true? : " << (grid[SouthIndex].ID == grid[gridIndex].ID) << '\n';
		std:: cout << "Is true? : " << (grid[EastIndex].ID == grid[gridIndex].ID) << '\n';
		std:: cout << "Is true? : " << (grid[WestIndex].ID == grid[gridIndex].ID) << '\n';
		if(grid[NorthIndex].ID == grid[gridIndex].ID && grid[NorthIndex].ID){
			if(matchCount < 2) {
				matchedCells[matchCount] = NorthIndex;
				matchCount++;
			}	
		}
		if(grid[EastIndex].ID == grid[gridIndex].ID && grid[EastIndex].ID){
			if(matchCount < 2){
				matchedCells[matchCount] = EastIndex;
				matchCount++;
			}
		}
		if(grid[SouthIndex].ID == grid[gridIndex].ID && grid[SouthIndex].ID){
			if(matchCount < 2){
				matchedCells[matchCount] = SouthIndex;
				matchCount++;
			}
		}
		if(grid[WestIndex].ID == grid[gridIndex].ID && grid[WestIndex].ID){
			if(matchCount< 2){
				matchedCells[matchCount] = WestIndex;
				matchCount++;
			}
		}
		if(matchCount == 1){
			//Now we check using the selected neighbor
			int selectedNeighbor = matchedCells[0];
			int NorthIndex = selectedNeighbor-gridX;
			int EastIndex = selectedNeighbor +1;
			int SouthIndex = selectedNeighbor+gridX;
			int WestIndex = selectedNeighbor-1;

			if(grid[NorthIndex].ID == grid[selectedNeighbor].ID && (NorthIndex != gridIndex)){
				if(matchCount <2){
					matchedCells[matchCount] = NorthIndex;
					matchCount++;
				}
			}
			if(grid[EastIndex].ID == grid[selectedNeighbor].ID && (EastIndex != gridIndex)){
				if(matchCount <2){
					matchedCells[matchCount] = EastIndex;
					matchCount++;
				}
			}
			if(grid[SouthIndex].ID == grid[selectedNeighbor].ID && (SouthIndex != gridIndex)){
				if(matchCount <2){
					matchedCells[matchCount] = SouthIndex;
					matchCount++;
				}
			}
			if(grid[WestIndex].ID == grid[selectedNeighbor].ID && (WestIndex != gridIndex)){
				if(matchCount <2){
					matchedCells[matchCount] = WestIndex;
					matchCount++;
				}
			}
		}
		std::cout << matchCount <<'\n';
		if (matchCount == 2){
			for(int c : matchedCells){
				grid[c].ID = iso::NONE;
			}
			grid[gridIndex].ID += 100;
		}
    }

	iso::cell& GetNeighbor(iso::cell*&grid,iso::vec2i indexToCheck, iso::vec2i selectedCell){
		int selectedIndex = GetIndex(selectedCell.x,selectedCell.y);	//The starting index
		int NorthIndex = GetIndex(indexToCheck.x,indexToCheck.y--);		
		std::cout << "Selected : " << selectedCell.x << ", " << selectedCell.y << '\n';
		std::cout << "North : " << indexToCheck.x << ", " << --indexToCheck.y << '\n';
		// std::cout << " og index : " << selectedIndex << " , north index : " << NorthIndex << '\n';
		//If the index I'm checking has a north neighbor and it's NOT the cell I came from
		if((NorthIndex >= 0) && (NorthIndex <= (gridX*gridY)) && (NorthIndex!=selectedIndex)){
			//if it's the same tile type, we want to keep track of it
			if(grid[NorthIndex].ID == grid[GetIndex(selectedCell.x,selectedCell.y)]._tileType) return grid[NorthIndex];
		}

		int EastIndex = GetIndex(indexToCheck.x++,indexToCheck.y);
		if((EastIndex >=0) && (EastIndex <= gridX*gridY) && (EastIndex!=selectedIndex)){
			if(grid[EastIndex].ID == grid[GetIndex(selectedCell.x,selectedCell.y)]._tileType) return grid[EastIndex];
		}

		int SouthIndex = GetIndex(indexToCheck.x,indexToCheck.y++);
		if((SouthIndex >=0) && (SouthIndex <= gridX*gridY) && (SouthIndex!=selectedIndex)){
			if(grid[SouthIndex].ID == grid[GetIndex(selectedCell.x,selectedCell.y)]._tileType) return grid[SouthIndex];
		}

		int WestIndex = GetIndex(indexToCheck.x--,indexToCheck.y);
		if((WestIndex >=0) && (WestIndex <= gridX*gridY) && (WestIndex!=selectedIndex)){
			if(grid[WestIndex].ID == grid[GetIndex(selectedCell.x,selectedCell.y)]._tileType) return grid[WestIndex];
		}
		return grid[selectedIndex];
	}
    int GetIndex(int x, int y)
    {
        return x+gridX * y;
    }
}