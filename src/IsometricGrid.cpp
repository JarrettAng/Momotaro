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
namespace isometric
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
} // namespace isometric
