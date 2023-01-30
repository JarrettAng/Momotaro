 /*!************************************************************************
 \file:         GridManager.h
 \author:		Amadeus Chia
 \par DP email:	amadeusjinhan.chia@digipen.edu
 \par Course:   CSD1171B
 \par Software Engineering Project
 \date:         30-01-2023
 \brief
 GridManager handles the grid logic with merging, synergy and placement of the 
 buildings, it also handles * map generation.

 The functions include:
 -
 **************************************************************************/

#pragma once

#include <IsometricGrid.h> 
namespace GridManager {
	namespace iso = IsometricGrid;

	/**
	 * @brief Upgrade level is an offset used to denote the levels of the buildings
	 * Level2 buildings onwards will have an ID higher than 100. We can use %100 to figure out what building type it is
	 */
	enum UpgradeLevel {
		SMALL = 0,
		MEDIUM = 100,
		LARGE = 200,
	};
	
	/**
	 * @brief Initializes the grid and spawns in the tiles. Also sets which tiles are to be rendered
	 * 
	 */
	void Initialize();

	/**
	 * @brief Helper boolean to check if the cell is safe to be placed on. Checks if it's within bounds and if the cell is renderable.
	 * Takes in CELL INDEX. DOES NOT CHECK IF THE CELL IS ALREADY OCCUPIED OR NOT!!
	 * @param x x index of the cell
	 * @param y y index of the cell
	 * @return true if a building can be placed. (even if there's an existing building)
	 * @return false if a building cannot be placed.
	 */
    bool isCellSafe(int x, int y);

	/**
	 * @brief Spawns a residential building at mouse pos. Overwrites any existing structure.
	 * 
	 * @param mousePos Player's Mouse position
	 */
    void SpawnResidential(Vec2<int> mousePos);

	/**
	 * @brief Spawns a commerical building at mouse pos. Overwrites any existing structure.
	 * 
	 * @param mousePos Player's Mouse position
	 */
    void SpawnCommerical(Vec2<int> mousePos);
	/**
	 * @brief Spawns a industrial building at mouse pos. Overwrites any existing structure.
	 * 
	 * @param mousePos Player's Mouse position
	 */
    void SpawnIndustrial(Vec2<int> mousePos);
	/**
	 * @brief Spawns a nature structure at mouse pos. Overwrites any existing structure.
	 * Randomly picks between rock or tree (50/50 chance) (can just change to a loop or smth next time)
	 * @param mousePos Player's Mouse position
	 */
    void SpawnNature(Vec2<int> mousePos);
	
	/**
	 * @brief Unused
	 * 
	 */
    void UpdateMouseToGrid();

	/**
	 * @brief Iterates through the grid and sends the data of whichever tiles need to be rendered to the render system
	 * 
	 */
    void PrepareTileRenderBatch();

	/**
	 * @brief Checks the cell neighbors and merges them. The order of merge goes clock wise from North (top right) 
	 * The order of merge goes clock wise from North (top right) -> East (bottom right) -> South (bottom left) -> West (top left)
	 * @param grid the grid to check
	 * @param cellIndex the current index to check
	 */
	void CheckCellNeighbor(iso::cell* grid,iso::vec2i cellIndex);

	/**
	 * @brief Get the Index object using the x + gridX * y conversion
	 * Basically converts the vector2 index to an int index
	 * @param x x index
	 * @param y y index
	 * @return int the converted index from a vector 2
	 */
	int GetIndex(int x,int y);

	/**
	 * @brief Clears the whole grid of any structure
	 * 
	 */
	void ClearGrid();

	/**
	 * @brief Randomises the terrain in the grid. Currently has 3 preset maps
	 * 
	 */
	void RandomiseTerrain();

	// iso::cell& GetNeighbor(iso::cell* &grid,iso::vec2i indexToCheck, iso::vec2i startingIndex);
}
