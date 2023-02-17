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
- CreateBuilding

- CreateRandomBuilding

**************************************************************************/

#pragma once

#include <Building.h>

namespace BuildingManager {
	void Initialize();
	void Clear();

	BuildingData GetBuildingData(BuildingEnum::TYPE type, Vec2<int> size, BuildingEnum::LEVEL level);
	BuildingData GetRandomBuildingData(BuildingEnum::TYPE type);
	BuildingData GetRandomBuildingData(BuildingEnum::LEVEL level);

	void CreateBuilding(BuildingEnum::TYPE type, Vec2<int> size, BuildingEnum::LEVEL level);
	void CreateRandomBuilding();
	/*void CreateRandomBuilding(BuildingEnum::TYPE type);
	void CreateRandomBuilding(BuildingEnum::SIZE size);
	void CreateRandomBuilding(BuildingEnum::LEVEL level);*/
}
