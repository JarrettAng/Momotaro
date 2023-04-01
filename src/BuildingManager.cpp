/*!************************************************************************
\file:          BuildingManager.cpp
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This source file implements the BuildingManager header file, it handles the
loading of buildings from the file and passing of building data

The functions include:
- Initialize
Loads all the building data from File IO at the start of the level
- GetBuildingData
Gets all the information of a specific building (type, size, and level)
- GetRandomBuildingData
Gets any random building (except nature)
- GetRandomNatureBuilding
Returns a random building of nature type (Rocks, pond, etc)
- GetBuildingDataVector
Get the vector containing every building type
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <vector>
#include <cstdlib>

#include <FileIOManager.h>
#include <BuildingManager.h>
#include <DebugManager.h>

#include <iostream>
///////////////////////////////////////////////////////////////////////////

namespace BuildingManager {
	std::vector<BuildingData> buildingsData; // Stores the information of all buildings

	///////////////////////////////////////////////////////////////////////
	// Loads all the building data from File IO at the start of the level
	void Initialize() {
		FileIOManager::LoadBuildingDataFromFile(buildingsData);
	}

	// At the end of the level, clear the buildingsData vector
	void Clear() {
		buildingsData.clear();
	}

	///////////////////////////////////////////////////////////////////////
	// Get all the information of a specific building (type, size, and level)
	BuildingData GetBuildingData(BuildingEnum::TYPE type, Vec2<int> size, BuildingEnum::LEVEL level) {
		for (BuildingData building : buildingsData) {
			if (building.type == type && building.size == size && building.level == level) {
				return building;
			}
		}
		// Error warning
		Debug::PrintError(__FILE__, __LINE__, "TRIED TO GET INVALID BUILDING DATA OF TYPE " + std::to_string(type) + ", SIZE " + std::to_string(size.x) + std::to_string(size.y) + ", LEVEL " + std::to_string(level));
		return buildingsData[0];
	}

	///////////////////////////////////////////////////////////////////////
	// Get a random building that matches the type (e.g. residential)
	BuildingData GetRandomBuildingData(BuildingEnum::TYPE type) {
		// Get a random size (for now 1x1) and level
		Vec2<int>			randSize  = Vec2<int>{1,1};
		BuildingEnum::LEVEL randLevel = (BuildingEnum::LEVEL)(rand() % (BuildingEnum::LEVEL_LENGTH - 1)); // -1 to exclude the LEVEL_LENGTH type itself

		for (BuildingData building : buildingsData) { // Find the matching building in the buildingsData vector
			if (building.type == type && building.size == randSize && building.level == randLevel) {
				return building;
			}
		}
		// Error warning
		Debug::PrintError(__FILE__, __LINE__, "TRIED TO GET (RANDOM) INVALID BUILDING DATA OF TYPE " + std::to_string(type) + ", SIZE " + std::to_string(randSize.x) + std::to_string(randSize.y) + ", LEVEL " + std::to_string(randLevel));
		return buildingsData[0];
	}

	// Get a random building that matches the level (e.g. L1 or L2)
	BuildingData GetRandomBuildingData(BuildingEnum::LEVEL level) {
		// Get a random type and level
		BuildingEnum::TYPE randType = (BuildingEnum::TYPE)((rand() % (BuildingEnum::TYPE_LENGTH - 2)) + 1); // -2 to exclude the TYPE_LENGTH and TYPE_NONE types
		Vec2<int>		   randSize = Vec2<int>{1,1};

		for (BuildingData building : buildingsData) { // Find the matching building in the buildingsData vector
			if (building.type == randType && building.size == randSize && building.level == level) {
				return building;
			}
		}
		// Error warning
		Debug::PrintError(__FILE__, __LINE__, "TRIED TO GET (RANDOM) INVALID BUILDING DATA OF TYPE " + std::to_string(randType) + ", SIZE " + std::to_string(randSize.x) + std::to_string(randSize.y) + ", LEVEL " + std::to_string(level));
		return buildingsData[0];
	}

	// Gets any random building (except for nature)
	BuildingData GetRandomBuildingData() {
		// Get a random size (for now 1x1), type, and level
		BuildingEnum::TYPE  randType  = (BuildingEnum::TYPE)((rand() % (BuildingEnum::TYPE_LENGTH - 2)) + 1);	// -2 to exclude the TYPE_LENGTH and TYPE_NONE types
		BuildingEnum::LEVEL randLevel = (BuildingEnum::LEVEL)(rand() % (BuildingEnum::LEVEL_LENGTH - 1));		// -1 to exclude the LEVEL_LENGTH type itself
		Vec2<int> randSize = Vec2<int>{ 1,1 };

		for (BuildingData building : buildingsData) { // Find the matching building in the buildingsData vector
			if (building.type == randType && building.size == randSize && building.level == randLevel) {
				return building;
			}
		}
		// Error warning
		Debug::PrintError(__FILE__, __LINE__, "TRIED TO GET (RANDOM) INVALID BUILDING DATA OF TYPE " + std::to_string(randType) + ", SIZE " + std::to_string(randSize.x) + std::to_string(randSize.y) + ", LEVEL " + std::to_string(randLevel));
		return buildingsData[0];
	}

	// Returns a random building of nature type (Rocks, pond, etc)
	Building GetRandomNatureBuilding() {		
		Building _randNature{
			BuildingData{
				BuildingEnum::NATURE,
				Vec2<int>{1,1},
				BuildingEnum::L1,
				BuildingEnum::RIGHT,
				0,0,0,0,
				"Nature", "You're not supposed to see this!",
				(TextureManager::TEX_TYPE)((rand()%3)+15)
			}
		};
		return _randNature;
	}

	// Get the vector containing every building type
    std::vector<BuildingData> GetBuildingDataVector() {
        return buildingsData;
    }
}
