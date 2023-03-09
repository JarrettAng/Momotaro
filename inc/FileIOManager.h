/*!************************************************************************
\file:          FileIOManager.h
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
#include <string>
#include <vector>
#include <Building.h>
#include <GridManager.h>

namespace FileIOManager {
	void ReadBuildingsData(std::vector<BuildingData>& buildingsData);
    void SaveBuildingDataToFile(std::vector<BuildingData>& buildingData);
    void LoadBuildingDataFromFile(std::vector<BuildingData> &buildingsData);
    void SaveGridToFile();
    GridManager::cell* LoadGridFromFile(std::string fileName);

}

