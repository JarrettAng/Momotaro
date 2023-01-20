
#include <vector>
#include <cstdlib>

#include <BuildingManager.h>
#include <FileIOManager.h>
#include <Building.h>

namespace BuildingManager {
	std::vector<Building> buildingsData;	// Template for all buildings
	std::vector<Building> buildings;		// Array of the actual buildings

	void Initialize() {
		FileIOManager::ReadBuildingsData(buildingsData);
	}

	Building* CreateBuilding(BuildingManager::TYPE type, BuildingManager::SIZE size, BuildingManager::LEVEL level) {
		for (auto building : buildings) {
			if (building.type == type && building.size == size && building.level == level) {
				buildings.emplace_back(building);
				return &building;
			}
		}

		return nullptr;
	}

	void CreateRandomBuilding() {
		BuildingManager::TYPE randType = (BuildingManager::TYPE)(rand() % BuildingManager::TYPE_LENGTH);
		BuildingManager::SIZE randSize = (BuildingManager::SIZE)(rand() % BuildingManager::SIZE_LENGTH);
		BuildingManager::LEVEL randLevel = (BuildingManager::LEVEL)(rand() % BuildingManager::LEVEL_LENGTH);

		CreateBuilding(randType, randSize, randLevel);
	}

	void CreateRandomBuilding(BuildingManager::TYPE type) {
		BuildingManager::SIZE randSize = (BuildingManager::SIZE)(rand() % BuildingManager::SIZE_LENGTH);
		BuildingManager::LEVEL randLevel = (BuildingManager::LEVEL)(rand() % BuildingManager::LEVEL_LENGTH);

		CreateBuilding(type, randSize, randLevel);
	}

	void CreateRandomBuilding(BuildingManager::SIZE size) {
		BuildingManager::TYPE randType = (BuildingManager::TYPE)(rand() % BuildingManager::TYPE_LENGTH);
		BuildingManager::LEVEL randLevel = (BuildingManager::LEVEL)(rand() % BuildingManager::LEVEL_LENGTH);

		CreateBuilding(randType, size, randLevel);
	}

	void CreateRandomBuilding(BuildingManager::LEVEL level) {
		BuildingManager::TYPE randType = (BuildingManager::TYPE)(rand() % BuildingManager::TYPE_LENGTH);
		BuildingManager::SIZE randSize = (BuildingManager::SIZE)(rand() % BuildingManager::SIZE_LENGTH);

		CreateBuilding(randType, randSize, level);
	}

	void Clear() {
		buildingsData.clear();
	}
}
