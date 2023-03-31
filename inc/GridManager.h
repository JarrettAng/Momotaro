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

#include <EventSystem.h>
// #include "MomoMaths.h"
#include <Building.h>

namespace GridManager {
	extern const int tileWidth;
    extern const int tileHeight;
	extern int gridX;
	extern int gridY;

	extern EventSystem::Event<void> onMergeBuildings;
	extern EventSystem::Event<void> onBoardFull;
	extern EventSystem::Event<int>  onTotalSynergyUpdate;

    enum TileType{
        NONE = 0,
        RESIDENTIAL = 1,
        INDUSTRIAL = 2,
        COMMERCIAL = 3,
        TREE = 999,
        ROCK = 1999,
    };
   
    struct cell{
        Vec2<int> pos{0,0};
        bool isRenderable{false};
        int ID{0};
        Building _building;
        // TileType _tileType{};
	};

    const cell* GetGrid();

    /// @brief Returns the area of the triangle
    /// @param x1 x1 of the 1st point of the triangle
    /// @param y1 y1 of the 1st point of the triangle
    /// @param x2 x2 of the 2nd point of the triangle
    /// @param y2 y2 of the 2nd point of the triangle
    /// @param x3 x3 of the 3rd point of the triangle
    /// @param y3 y3 of the 3rd point of the triangle
    /// @return returns area of triangle as a float
    float area(int x1, int y1, int x2, int y2, int x3, int y3);

    /// @brief Checks if mouse is in the triangle area specified
    /// @param _mouseX Mouse X position
    /// @param _mouseY Mouse Y position
    /// @param x1 x1 of the 1st point of the triangle
    /// @param y1 y1 of the 1st point of the triangle
    /// @param x2 x2 of the 2nd point of the triangle
    /// @param y2 y2 of the 2nd point of the triangle
    /// @param x3 x3 of the 3rd point of the triangle
    /// @param y3 y3 of the 3rd point of the triangle
    /// @return 
    bool isInside(int _mouseX, int _mouseY, int x1, int y1, int x2, int y2, int x3, int y3);


    /// @brief Converts World Cell Index to Screen Position.
    /// @param x x index of the world cell
    /// @param y y index of the world cell
    /// @return Screen position in the form of vec2i
    Vec2<int> WorldIndexToScreenPos(int x, int y);
    Vec2<int> ToScreen(int x, int y);
    Vec2<int> ScreenPosToIso(int x, int y);
    Vec2<int> ScreenPosToIso(Vec2<int> cellPos);    //overload to take in vec2 int


    Vec2<int> MouseToCell(int mouseX, int mouseY);

	/**
	 * @brief Initializes the grid and spawns in the tiles. Also sets which tiles are to be rendered
	 *
	 */
	void Initialize(std::string const& filePath);

    void SpawnBuilding(Vec2<int> mousePos);
	void Free();
    /**
	 * @brief Helper boolean to check if the cell is safe to be placed on. Checks if it's within bounds and if the cell is renderable.
	 * Takes in CELL INDEX. DOES NOT CHECK IF THE CELL IS ALREADY OCCUPIED OR NOT!!
	 * @param x x index of the cell
	 * @param y y index of the cell
	 * @return true if a building can be placed. (even if there's an existing building)
	 * @return false if a building cannot be placed.
	 */
	bool isCellSafe(Vec2<int> selectedCell);
	bool isWithinGrid(Vec2<int> selectedCell);

	/**
	 * @brief Spawns a residential building at mouse pos. Overwrites any existing structure.
	 *
	 */
    void SpawnResidential();

    /// @brief 
    void SpawnBigResidential();
    void SpawnBigResidential3x1();

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
	cell NatureCell();
	cell BuildingCell(int);
	void ToggleTileRenderable();

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

    int GetSynergyText(Vec2<int> cellToCheck, BuildingData _data);

    /**
	 * @brief Checks the cell neighbors and merges them. The order of merge goes clock wise from North (top right)
	 * The order of merge goes clock wise from North (top right) -> East (bottom right) -> South (bottom left) -> West (top left)
	 * @param grid the grid to check
	 * @param cellIndex the current index to check
	 */
	void CheckCellNeighbor(cell* grid, Vec2<int> cellIndex);

	/**
	 * @brief Get the Index object using the x + gridX * y conversion
	 * Basically converts the vector2 index to an int index
	 * @param x x index
	 * @param y y index
	 * @return int the converted index from a vector 2
	 */
	int GetIndex(int x, int y);

    int GetIndex(Vec2<int> cell);

    bool HasID(int* array, int count, int ID);

    BuildingEnum::TYPE GetTypeFromIndex(Vec2<int> cell);

    int GetIndexFromID(int ID);



    /**
	 * @brief Clears the whole grid of any structure
	 *
	 */
	void ClearGrid();


    void GetBuildingCard(const BuildingData *_data);
	bool IsBuildingValid(const BuildingData* _data, int _x, int _y);
    std::vector<Vec2<int>> GetBuildingCells(const BuildingData* _data, int _x, int _y);
	std::vector<Vec2<int>> GetSynergyArea(std::vector<Vec2<int>> _buildingCells);
    // std::vector<Vec2<int>> GetBuildingCells(BuildingData _data, int _x, int _y);
	void SetBuildingID(int _id);

    void ChangeOrientation();
	// iso::cell& GetNeighbor(iso::cell* &grid,iso::vec2i indexToCheck, iso::vec2i startingIndex);
}
