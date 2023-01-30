/*!************************************************************************
\file:          BuildingManager.h
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#pragma once

#include <Building.h>

namespace BuildingManager {
	void Initialize();
	void Clear();

	BuildingData GetBuildingData(BuildingEnum::TYPE type, BuildingEnum::SIZE size, BuildingEnum::LEVEL level);
	BuildingData GetRandomBuildingData(BuildingEnum::TYPE type);
	BuildingData GetRandomBuildingData(BuildingEnum::LEVEL level);

	void CreateBuilding(BuildingEnum::TYPE type, BuildingEnum::SIZE size, BuildingEnum::LEVEL level);
	void CreateRandomBuilding();
	/*void CreateRandomBuilding(BuildingEnum::TYPE type);
	void CreateRandomBuilding(BuildingEnum::SIZE size);
	void CreateRandomBuilding(BuildingEnum::LEVEL level);*/
}
