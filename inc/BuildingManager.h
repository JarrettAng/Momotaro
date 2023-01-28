
#pragma once

#include <Building.h>

namespace BuildingManager {
	void Initialize();
	void Clear();

	BuildingData GetBuildingData(BuildingEnum::TYPE type, BuildingEnum::SIZE size, BuildingEnum::LEVEL level);
	void CreateBuilding(BuildingEnum::TYPE type, BuildingEnum::SIZE size, BuildingEnum::LEVEL level);

	void CreateRandomBuilding();
	/*void CreateRandomBuilding(BuildingEnum::TYPE type);
	void CreateRandomBuilding(BuildingEnum::SIZE size);
	void CreateRandomBuilding(BuildingEnum::LEVEL level);*/
}
