/*!************************************************************************
\file:          BuildingManager.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <vector>
#include <cstdlib>

#include <FileIOManager.h>
#include <BuildingManager.h>

#include <iostream>

namespace BuildingManager {
	std::vector<BuildingData> buildingsData; // Template for all buildings
	std::vector<Building> buildings;		 // Array of the actual buildings


	void Initialize() {
		FileIOManager::ReadBuildingsData(buildingsData);
	}

	BuildingData GetBuildingData(BuildingEnum::TYPE type, BuildingEnum::SIZE size, BuildingEnum::LEVEL level) {
		for (BuildingData building : buildingsData) {
			if (building.type == type && building.size == size && building.level == level) {
				return building;
			}
		}
		// TODO: Error check
		std::cerr << "BuildingManager: TRIED TO GET INVALID BUILDING DATA OF TYPE " << type << ", SIZE " << size << ", LEVEL " << level << "\n";

		return buildingsData[0];
	}

	BuildingData GetRandomBuildingData(BuildingEnum::TYPE type) {
		BuildingEnum::SIZE randSize = (BuildingEnum::SIZE)(rand() % BuildingEnum::SIZE_LENGTH);
		BuildingEnum::LEVEL randLevel = (BuildingEnum::LEVEL)(rand() % BuildingEnum::LEVEL_LENGTH);

		// TODO: Remove after prototype
		randSize = BuildingEnum::_1X1;

		for (BuildingData building : buildingsData) {
			if (building.type == type && building.size == randSize && building.level == randLevel) {
				return building;
			}
		}
		// TODO: Error check
		std::cerr << "BuildingManager: TRIED TO GET (RANDOM) INVALID BUILDING DATA OF TYPE " << type << ", SIZE " << randSize << ", LEVEL " << randLevel << "\n";

		return buildingsData[0];
	}

	BuildingData GetRandomBuildingData(BuildingEnum::LEVEL level) {
		BuildingEnum::TYPE randType = (BuildingEnum::TYPE)(rand() % BuildingEnum::TYPE_LENGTH);
		BuildingEnum::SIZE randSize = (BuildingEnum::SIZE)(rand() % BuildingEnum::SIZE_LENGTH);

		// TODO: Remove after prototype
		randSize = BuildingEnum::_1X1;

		for (BuildingData building : buildingsData) {
			if (building.type == randType && building.size == randSize && building.level == level) {
				return building;
			}
		}
		// TODO: Error check
		std::cerr << "BuildingManager: TRIED TO GET (RANDOM) INVALID BUILDING DATA OF TYPE " << randType << ", SIZE " << randSize << ", LEVEL " << level << "\n";

		return buildingsData[0];
	}

	void CreateBuilding(BuildingEnum::TYPE type, BuildingEnum::SIZE size, BuildingEnum::LEVEL level) {
		buildings.emplace_back(GetBuildingData(type, size, level));
	}

	void CreateRandomBuilding() {
		BuildingEnum::TYPE randType = (BuildingEnum::TYPE)(rand() % BuildingEnum::TYPE_LENGTH);
		BuildingEnum::SIZE randSize = (BuildingEnum::SIZE)(rand() % BuildingEnum::SIZE_LENGTH);
		BuildingEnum::LEVEL randLevel = (BuildingEnum::LEVEL)(rand() % BuildingEnum::LEVEL_LENGTH);

		CreateBuilding(randType, randSize, randLevel);
	}

	//void CreateRandomBuilding(TYPE type) {
	//	SIZE randSize = (SIZE)(rand() % SIZE_LENGTH);
	//	LEVEL randLevel = (LEVEL)(rand() % LEVEL_LENGTH);

	//	CreateBuilding(type, randSize, randLevel);
	//}

	//void CreateRandomBuilding(SIZE size) {
	//	TYPE randType = (TYPE)(rand() % TYPE_LENGTH);
	//	LEVEL randLevel = (LEVEL)(rand() % LEVEL_LENGTH);

	//	CreateBuilding(randType, size, randLevel);
	//}

	//void CreateRandomBuilding(LEVEL level) {
	//	TYPE randType = (TYPE)(rand() % TYPE_LENGTH);
	//	SIZE randSize = (SIZE)(rand() % SIZE_LENGTH);

	//	CreateBuilding(randType, randSize, level);
	//}

	void Clear() {
		buildingsData.clear();
	}
}
