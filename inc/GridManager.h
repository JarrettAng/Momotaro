
#pragma once
#include <IsometricGrid.h>
namespace GridManager {
	namespace iso = IsometricGrid;

	enum UpgradeLevel {
		SMALL = 0,
		MEDIUM = 100,
		LARGE = 200,
	};
	
	void Initialize();
	void UpdateMouseToGrid();
	void PrepareTileRenderBatch();
	void CheckCellNeighbor(iso::cell* grid,iso::vec2i cellIndex);
	int GetIndex(int x,int y);
	iso::cell& GetNeighbor(iso::cell* &grid,iso::vec2i indexToCheck, iso::vec2i startingIndex);
}
