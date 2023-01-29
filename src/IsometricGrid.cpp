/*!************************************************************************
\file IsometricGrid.cpp
\author Amadeus Chia
\par DP email: amadeusjinhan.chia@digipen.edu
\par Course: CSD1171B
\par Programming Lab #1
\date 12/01/2023
\brief
This source file is a test file for an isometric grid
*/
#include "AEEngine.h"
#include <IsometricGrid.h>
#include <iostream>

/// @brief 
namespace IsometricGrid
{
	//MATHS 
	/*
	Cell.X = MouseX/TileWidth;
	Cell.y = MouseY/TileHeight;
	If you want to center the mouse,
	OffsetX = MouseX % CellWidth
	OffsetY = MouseY % CellHeight

	Cell.X/ScreenWidth
	Cell.Y/ScreenHeight

	Can technically do it in a 1D array with
	CellIndex = CellY * CellWidth + CellX

	Isometric Calculations (left down is Y, right down is X, starts from top 0,0)
	Cells are 2 times wider than height.
	ScreenX = (x-y)*(CellWidth/2)       this is because as you move down x isometrically, your X moves .5 cells right, and .5 cells down
	ScreenY = (x+y)*(CellHeight/2)      this is because as you move isometrically down y, your y moves .5 cells left, and .5 cells down
	note that you have to offset the origin also!

	*/
	const int tileWidth{ 100 };
	const int tileHeight{ 50 };
	/*
	  GridManager::GridManager(int x, int y){

	  }*/




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
	vec2i ToScreen(int x, int y){
		int originX = AEGetWindowWidth() / 2 / 100;
		int originY = AEGetWindowHeight() / 2 / 50;
		return vec2i{
			(originX*tileWidth) + (x-y)*(tileWidth/2),
			(originY*tileHeight)+(x+y)*(tileHeight/2)
		};
	}

	vec2i WorldIndexToScreenPos(int x, int y) {
		return vec2i{   //we need to keep the tile height and width a float here!
			static_cast<int>((x - y) * (50.f)),
			static_cast<int>((x + y) * (-25.f)+25.f)		//offset for the correct pos because of the height diff
		};
	}
	vec2i ScreenPosToIso(int xPos, int yPos) {
		//MOUSE INPUTS (Tile width = 100, tile height = 50)
		int cellX = xPos / 100;
		int cellY = yPos / 50;
		int index = cellX + 20 * cellY;

		int xOffset = xPos % 100;
		int yOffset = yPos % 50;
		//Origin -> screen/tile. For now I use numbers
		int originX = AEGetWindowWidth() / 2 / 100;
		int originY = AEGetWindowHeight() / 2 / 50;

		// int selectX = (cellX - originX) + (cellY - originY);
		// int selectY = (cellY - originY) - (cellX - originX);

		vec2i SelectedCell{
			(cellX - originX) + (cellY - originY)+10,		//x
			(cellY - originY) - (cellX - originX)+10		//y
		};
		//TOP LEFT
		if (isInside(xOffset, yOffset, 0, 0, 0, 25, 50, 0))SelectedCell.x--;
		//BOTTOM LEFT
		if (isInside(xOffset, yOffset, 0, 25, 0, 50, 50, 50))SelectedCell.y++;
		//TOP RIGHT
		if (isInside(xOffset, yOffset, 50, 0, 100, 0, 100, 25))SelectedCell.y--;
		//BOTTOM RIGHT
		if (isInside(xOffset, yOffset, 50, 50, 100, 50, 100, 25))SelectedCell.x++;

		return SelectedCell;
		// //MOUSE INPUTS (Tile width = 100, tile height = 50)
		// int cellX = x / 100;
		// int cellY = y / 50;

		// //Origin -> screen/tile. For now I use numbers
		// int originX = AEGetWindowWidth()/2 / 100;
		// int originY = AEGetWindowHeight()/2 / 50;

		// int selectX = (cellX - originX) + (cellY - originY);
		// int selectY = (cellY - originY) - (cellX - originX);
		// return vec2i{
		// 	static_cast<int>((x / (tileWidth / 2)) + (y / (tileHeight / 2))),
		// 	static_cast<int>((y / (tileHeight / 2)) - (x / (tileWidth / 2)))
		// };
	}
	vec2i MouseToCell(int mouseX, int mouseY) {
		return vec2i{
			mouseX / tileWidth,
			mouseY / tileHeight
		};
	}
   
    vec2i MouseCellOffset(int mouseX, int mouseY)
    {
        return vec2i{
			mouseX % tileWidth,
			mouseY % tileHeight
		};
    }

#pragma region  old lambda code
#if 0
	auto area = [&](int x1, int y1, int x2, int y2, int x3, int y3) {
		return float{ static_cast<float>(abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0f)) };
	};

	auto isInside = [&](int _mouseX, int _mouseY, int x1, int y1, int x2, int y2, int x3, int y3) {
		float A = area(x1, y1, x2, y2, x3, y3);
		float B = area(_mouseX, _mouseY, x2, y2, x3, y3);
		float C = area(x1, y1, _mouseX, _mouseY, x3, y3);
		float D = area(x1, y1, x2, y2, _mouseX, _mouseY);
		return (A == (B + C + D));
	};

	auto WorldIndexToScreenPos = [&](int x, int y) {
		return vec2i{
			static_cast<int>((x - y) * (50.f)),
			static_cast<int>((x + y) * (-25.f)) - 25
		};
	};
	auto ScreenPosToIso = [&](int x, int y) {

	};
#endif
#pragma endregion
} // namespace isometric
