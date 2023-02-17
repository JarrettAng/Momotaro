/*!************************************************************************
\file:          BuildingManager.cpp
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
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

	BuildingData GetBuildingData(BuildingEnum::TYPE type, Vec2<int> size, BuildingEnum::LEVEL level) {
		for (BuildingData building : buildingsData) {
			if (building.type == type && building.size == size && building.level == level) {
				return building;
			}
		}
		// TODO: Error check
		std::cerr << "BuildingManager ln39: TRIED TO GET INVALID BUILDING DATA OF TYPE " << type << ", SIZE " << size << ", LEVEL " << level << "\n";

		return buildingsData[0];
	}

	BuildingData GetRandomBuildingData(BuildingEnum::TYPE type) {
		Vec2<int> randSize = Vec2<int>{1,1};
		BuildingEnum::LEVEL randLevel = (BuildingEnum::LEVEL)((rand() % BuildingEnum::LEVEL_LENGTH-1)+1);

		// TODO: Remove after prototype
		// randSize = BuildingEnum::_1X1;

		for (BuildingData building : buildingsData) {
			if (building.type == type && building.size == randSize && building.level == randLevel) {
				return building;
			}
		}
		// TODO: Error check
		std::cerr << "BuildingManager ln57: TRIED TO GET (RANDOM) INVALID BUILDING DATA OF TYPE " << type << ", SIZE " << randSize << ", LEVEL " << randLevel << "\n";

		return buildingsData[0];
	}

	BuildingData GetRandomBuildingData(BuildingEnum::LEVEL level) {
		BuildingEnum::TYPE randType = (BuildingEnum::TYPE)((rand() % BuildingEnum::LEVEL_LENGTH-1)+1);
		Vec2<int> randSize = Vec2<int>{1,1};

		// TODO: Remove after prototype
		// randSize = Vec2<int>{1,1};

		for (BuildingData building : buildingsData) {
			if (building.type == randType && building.size == randSize && building.level == level) {
				return building;
			}
		}
		// TODO: Error check
		std::cerr << "BuildingManager ln75: TRIED TO GET (RANDOM) INVALID BUILDING DATA OF TYPE " << randType << ", SIZE " << randSize << ", LEVEL " << level << "\n";

		return buildingsData[0];
	}

	void CreateBuilding(BuildingEnum::TYPE type, Vec2<int> size, BuildingEnum::LEVEL level) {
		buildings.emplace_back(GetBuildingData(type, size, level));
	}

	void CreateRandomBuilding() {
		BuildingEnum::TYPE randType = (BuildingEnum::TYPE)(rand() % BuildingEnum::TYPE_LENGTH);
		Vec2<int> randSize = Vec2<int>{1,1};
		BuildingEnum::LEVEL randLevel = (BuildingEnum::LEVEL)(rand() % BuildingEnum::LEVEL_LENGTH);

		CreateBuilding(randType, randSize, randLevel);
	}

	void Clear() {
		buildingsData.clear();
	}
}
