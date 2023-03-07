/*!************************************************************************
\file:          BuildingManager.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This header file handles the creation/deletion of buildings, and the loading 
of building data.

The functions include:
- Initialize
Reads the building data from the JSON file upon loading into the game scene
- Clear
Resets the data read on game scene exit
- GetBuildingData
Returns the building information of the given type, size, level
- GetRandomBuildingData
Returns a random building information, can be controlled by parameters
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <Building.h>
///////////////////////////////////////////////////////////////////////////

namespace BuildingManager {
	void Initialize();	// Loads all the building data from File IO at the start of the level
	void Clear();		// At the end of the level, clear the buildingsData vector

	// Get all the information of a specific building (type, size, and level)
	BuildingData GetBuildingData(BuildingEnum::TYPE type, Vec2<int> size, BuildingEnum::LEVEL level);
	
	BuildingData GetRandomBuildingData(BuildingEnum::TYPE type);	// Get a random building that matches the type (e.g. residential)
	BuildingData GetRandomBuildingData(BuildingEnum::LEVEL level);	// Get a random building that matches the level (e.g. L1 or L2)
	BuildingData GetRandomBuildingData();							// Get any random building
}
