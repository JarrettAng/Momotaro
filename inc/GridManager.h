/*!************************************************************************
\file:          GridManager.cpp
\author:		Amadeus Chia
\par DP email:	amadeusjinhan.chia@digipen.edu
\par Course:    CSD1451B
\par Software Engineering Project
\date:          30-01-2023
\brief
This header file contains the declaration for functions that manage the game grid.
The source file documents the important functions to take note of. 
The header file will contain the detailed explanations for each function
All content � 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once

#include <EventSystem.h>
#include <Building.h>

namespace GridManager {
	///////////////////////////////////////////////////////////////////////////
	//GLOBAL VARIABLES
	extern const int tileWidth;
    extern const int tileHeight;	
	extern int gridX;				//Grid width
	extern int gridY;				//Grid height

	///////////////////////////////////////////////////////////////////////////
	//EVENT RELATED 
	extern EventSystem::Event<void> onMergeBuildings;
	extern EventSystem::Event<void> onBoardFull;
	extern EventSystem::Event<int>  onTotalSynergyUpdate;

	///////////////////////////////////////////////////////////////////////////
	// DATA TYPES

    //Enum for the tile type
    enum TileType{
        NONE = 0,
        RESIDENTIAL = 1,
        INDUSTRIAL = 2,
        COMMERCIAL = 3,
        TREE = 999,
        ROCK = 1999,
    };
	
	//Data type for the grid cell
    struct cell{
        Vec2<int> pos{0,0};
        bool isRenderable{false};
        int ID{0};
        Building _building;
	};


	///////////////////////////////////////////////////////////////////////////
	//GRID MANAGER FUNCTIONS

	/**
	 * @brief Initializes the grid and spawns in the tiles. Also sets which tiles are to be rendered
	 *
	 */
	void Initialize(std::string const& filePath);

	/**
	 * @brief Checks if the mouse has changed index (Update loop for gridmanager)
	 *
	 */
	void UpdateMouseToGrid();


	/**
	 * @brief Iterates through the grid and sends the data of whichever tiles need to be rendered to the render system
	 * (Basically the draw function of the grid)
	 */
	void PrepareTileRenderBatch();

	/**
	 * @brief Frees the grid and it's subscribed functions. Also resets variables 
	 * 
	 */
	void Free();

	
	///////////////////////////////////////////////////////////////////////////
	//CORE GRID FUNCTIONS

	/**
	 * @brief Checks the cell neighbors and merges them. The order of merge goes clock wise from North (top right)
	 * The order of merge goes clock wise from North (top right) -> East (bottom right) -> South (bottom left) 
	 * 	-> 	West (top left)
	 * @param grid the grid to check
	 * @param cellIndex the current index to check
	 */
	void CheckCellNeighbor(cell* grid, Vec2<int> cellIndex);

	/**
	 * @brief Spawns building at the mouse position. Nothing will spawn if the cell is an invalid/occupied cell
	 * 
	 * @param mousePos Mouse position 
	 */
	void SpawnBuilding(Vec2<int> mousePos);

	///////////////////////////////////////////////////////////////////////////
	//ISOMETRIC GRID FUNCTIONS

	/**
	 * @brief Returns the area of a triangle from 3 points (used for isometric calculations)
	 * 
	 * @param x1 x1 of the 1st point of the triangle
	 * @param y1 y1 of the 1st point of the triangle
	 * @param x2 x2 of the 2nd point of the triangle
	 * @param y2 y2 of the 2nd point of the triangle
	 * @param x3 x3 of the 3rd point of the triangle
	 * @param y3 y3 of the 3rd point of the triangle
	 * @return float returns area of triangle
	 */
    float area(int x1, int y1, int x2, int y2, int x3, int y3);

	/**
	 * @brief Checks if mouse is in the triangle area specified
	 * 	(Used for isometric grid index checking in the corners)
	 * @param _mouseX Mouse X position
	 * @param _mouseY Mouse Y position
	 * @param x1 x1 of the 1st point of the triangle
	 * @param y1 y1 of the 1st point of the triangle
	 * @param x2 x2 of the 2nd point of the triangle
	 * @param y2 y2 of the 2nd point of the triangle
	 * @param x3 x3 of the 3rd point of the triangle
	 * @param y3 y3 of the 3rd point of the triangle
	 * @return true if the mouse is indeed in the corner specified
	 * @return false if the mouse is NOT in the corner specified
	 */
    bool isInside(int _mouseX, int _mouseY, int x1, int y1, int x2, int y2, int x3, int y3);

	/**
	 * @brief Converts World Cell Index to Screen Position.
	 * 
	 * @param x x index of the world cell
	 * @param y y index of the world cell
	 * @return Vec2<int> Screen position
	 */
    Vec2<int> WorldIndexToScreenPos(int x, int y);

	/**
	 * @brief Converts 2 ints into vec2<int> of the screen
	 * 
	 * @param x x position of the cell index to convert
	 * @param y y position of the cell index to convert
	 * @return Vec2<int> of the point from cell index
	 */
    Vec2<int> ToScreen(int x, int y);

	/**
	 * @brief Helper function to convert the screen pos to vec2<int> cell index (usually it's the mouse pos)
	 * 
	 * @param x x of the point to convert 
	 * @param y y of the point to convert
	 * @return Vec2<int> of the cell index
	 */
    Vec2<int> ScreenPosToIso(int x, int y);

	/**
	 * @brief Overloaded helper function to convert screen pos in the form of Vec2<int> to Vec2<int>
	 * 
	 * @param cellPos Screen position of point to convert
	 * @return Vec2<int> Cell index 
	 */
    Vec2<int> ScreenPosToIso(Vec2<int> cellPos);  

	/**
	 * @brief Returns the non-isometric grid index of the mousse
	 * 
	 * @param mouseX x position of the mouse
	 * @param mouseY y position of the mouse
	 * @return Vec2<int> index of the non-iso cell the mouse is in
	 */
    Vec2<int> MouseToCell(int mouseX, int mouseY);

	///////////////////////////////////////////////////////////////////////////
	//HELPER FUNCTIONS
	/**
	 * @brief Spawns a residential building at mouse pos. Overwrites any existing structure.
	 *
	 */
    void SpawnResidential();

    /**
	 * @brief Spawns a commerical building at mouse pos. Overwrites any existing structure.
	 */
	void SpawnCommerical();
	/**
	 * @brief Spawns a industrial building at mouse pos. Overwrites any existing structure.
	 *
	 * @param mousePos Player's Mouse position
	 */
	void SpawnIndustrial();
	/**
	 * @brief Spawns a nature structure at mouse pos. Overwrites any existing structure.
	 * Randomly picks between rock or tree (50/50 chance) (can just change to a loop or smth next time)
	 * @param mousePos Player's Mouse position
	 */
	void SpawnNature();

	/**
	 * @brief Returns a nature cell of random nature texture
	 * 
	 * @return cell that is a nature building
	 */
	cell NatureCell();

	/**
	 * @brief Toggles a renderable tile on or off (used in editor)
	 * 
	 */
	void ToggleTileRenderable();


	/**
	 * @brief Get the Grid object (used in fileIO)
	 * Basically a read-only accessor for grid
	 * @return const cell* of the grid
	 */
    const cell* GetGrid();
	
	/**
	 * @brief Helper boolean to check if the cell is safe to be placed on. Checks if it's within bounds and if the cell is renderable.
	 * Takes in CELL INDEX. DOES NOT CHECK IF THE CELL IS ALREADY OCCUPIED OR NOT!!
	 * @param x x index of the cell
	 * @param y y index of the cell
	 * @return true if a building can be placed. (even if there's an existing building)
	 * @return false if a building cannot be placed.
	 */
	bool isCellSafe(Vec2<int> selectedCell);

	/**
	 * @brief Helper boolean to check if the cell is within the grid!
	 * Use this to check for anything WITHIN THE SCREEN! 
	 * To check if placeable, use isCellSafe instead!!
	 * @param selectedCell index of cell to check
	 * @return true if it is within the screen/bounds of the grid
	 * @return false if its out of the screen or out of range 
	 */
	bool isWithinGrid(Vec2<int> selectedCell);
	
	/**
	 * @brief Gets the synergy point calculation for the cell
	 * 
	 * @param cellToCheck index of the cell to check
	 * @param _data data of the building
	 * @return int number of synergy points
	 */
    int GetSynergyText(Vec2<int> cellToCheck, BuildingData _data);



	/**
	 * @brief Get the Index object using the x + gridX * y conversion
	 * Basically converts the vector2 index to an int index
	 * @param x x index
	 * @param y y index
	 * @return int the converted index from a vector 2
	 */
	int GetIndex(int x, int y);

	/**
	 * @brief Get the Index object (overloaded)
	 * See above, same implementation
	 * @param cell index of cell
	 * @return int the converted index from a Vec2<int>
	 */
    int GetIndex(Vec2<int> cell);

	/**
	 * @brief Checks if a tile with the specified ID exists in the grid
	 * It's the user's responsibility to ensure the ID is valid!
	 * @param array Array to check
	 * @param count Count of the array
	 * @param ID ID to check for
	 * @return true if the ID exists
	 * @return false returns an error
	 */
    bool HasID(int* array, int count, int ID);

	/**
	 * @brief Get the Type From Index object
	 * 
	 * @param cell index to get type from
	 * @return BuildingEnum::TYPE type of the building in the cell
	 */
    BuildingEnum::TYPE GetTypeFromIndex(Vec2<int> cell);

	/**
	 * @brief Get the Index From ID provided
	 * 
	 * @param ID to check for
	 * @return int index of the found ID
	 */
    int GetIndexFromID(int ID);



    /**
	 * @brief Clears the whole grid of any structure
	 * (Keeps renderable tiles as renderable but wipes all nature tiles!)
	 */
	void ClearGrid();

	/**
	 * @brief Resets the grid back to its original state
	 * (This keeps nature buildings!)
	 */
	void ResetGrid();

	/**
	 * @brief Get the Building Card object from card manager
	 * (Gets cached in a local variable)
	 * @param _data read-only building data from card manager
	 */
    void GetBuildingCard(const BuildingData *_data);

	/**
	 * @brief Checks if the current building orientation/position is valid to be placed
	 * 
	 * @param _data data of building to check
	 * @param _x x position
	 * @param _y y position
	 * @return true if the position is valid
	 * @return false if the position is invalid
	 */
	bool IsBuildingValid(const BuildingData* _data, int _x, int _y);

	/**
	 * @brief Get the Building Cells objects all the buildings of the size specified. (for now it's just 1x1 since 2x1,2x2 was scrapped)
	 * @param _data data of the building
	 * @param _x x position
	 * @param _y y position
	 * @return std::vector<Vec2<int>> All the building cells related to the building
	 */
    std::vector<Vec2<int>> GetBuildingCells(const BuildingData* _data, int _x, int _y);

	/**
	 * @brief Get the Synergy Area object
	 * 
	 * @param _buildingCells cells relating to the building to check for
	 * @return std::vector<Vec2<int>> of cells of the synergy area
	 */
	std::vector<Vec2<int>> GetSynergyArea(std::vector<Vec2<int>> _buildingCells);

	/**
	 * @brief Set the Building ID (used by fileIO)
	 * 
	 * @param _id ID to set
	 */
	void SetBuildingID(int _id);

	/**
	 * @brief Draws a tile at mouse pos (used in editor only!)
	 * 
	 * @param mousePos mouse position
	 */
	void DrawSelectedTile(Vec2<int> mousePos);

	///////////////////////////////////////////////////////////////////////////
	//DEPRECATED FUNCTIONS
    void SpawnBigResidential();
    void SpawnBigResidential3x1();
    void ChangeOrientation();
}
