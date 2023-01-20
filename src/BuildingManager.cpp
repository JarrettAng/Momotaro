
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

	void CreateBuilding(BuildingManager::TYPE type, BuildingManager::SIZE size, BuildingManager::LEVEL level) {
		buildingsData.emplace_back(buildingsData[type + size + level]);
	}

	void CreateRandomBuilding() {
		BuildingManager::TYPE randType = (BuildingManager::TYPE)(rand() % BuildingManager::TYPE_LAST);
		BuildingManager::SIZE randSize = (BuildingManager::SIZE)(rand() % BuildingManager::SIZE_LAST);
		BuildingManager::LEVEL randLevel = (BuildingManager::LEVEL)(rand() % BuildingManager::LEVEL_LAST);

		CreateBuilding(randType, randSize, randLevel);
	}

	void CreateRandomBuilding(BuildingManager::TYPE type) {
		BuildingManager::SIZE randSize = (BuildingManager::SIZE)(rand() % BuildingManager::SIZE_LAST);
		BuildingManager::LEVEL randLevel = (BuildingManager::LEVEL)(rand() % BuildingManager::LEVEL_LAST);

		CreateBuilding(type, randSize, randLevel);
	}

	void CreateRandomBuilding(BuildingManager::SIZE size) {
		BuildingManager::TYPE randType = (BuildingManager::TYPE)(rand() % BuildingManager::TYPE_LAST);
		BuildingManager::LEVEL randLevel = (BuildingManager::LEVEL)(rand() % BuildingManager::LEVEL_LAST);

		CreateBuilding(randType, size, randLevel);
	}

	void CreateRandomBuilding(BuildingManager::LEVEL level) {
		BuildingManager::TYPE randType = (BuildingManager::TYPE)(rand() % BuildingManager::TYPE_LAST);
		BuildingManager::SIZE randSize = (BuildingManager::SIZE)(rand() % BuildingManager::SIZE_LAST);

		CreateBuilding(randType, randSize, level);
	}

	void Clear() {
		buildingsData.clear();
	}
}
