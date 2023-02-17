/*!************************************************************************
\file:          FileIOManager.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <iostream>
#include <fstream>

#include <FileIOManager.h>
#include <BuildingManager.h>

namespace FileIOManager {
	void ReadBuildingsData(std::vector<BuildingData>& buildingsData) {
		std::ifstream dataFile{ "Assets/JSON_Data/buildingsData.json" };
		if (!dataFile) {
			std::cerr << "Unable to read buildings data JSON file!\n";
		}

		// Implementation here


		// Store data in the buildingsData vector (array)
		// Temp only!! To be replaced with actual reading of the JSON file on top ^^
		// BuildingData newBuilding{ 
		// 	BuildingEnum::RESIDENTIAL, 
		// 	BuildingEnum::_1X1, 
		// 	BuildingEnum::L1, 
		// 	BuildingEnum::RIGHT,
		// 	1, 3, -3, 2,
		// 	"Small house", "Small lovely house",
		// 	TextureManager::RESIDENTIAL_1X1_L1, RenderSystem::BUILDING};
		// buildingsData.push_back(newBuilding);
		// BuildingData newBuilding2{
		// 	 BuildingEnum::RESIDENTIAL,
		// 	 BuildingEnum::_1X1, BuildingEnum::L2,
		// 	 BuildingEnum::RIGHT,
		// 	 1, 3, -3, 2, 
		// 	 "Terrace house", "Bigger lovelier house",
		// 	 TextureManager::RESIDENTIAL_1X1_L2, RenderSystem::BUILDING};
		// buildingsData.push_back(newBuilding2);
		// BuildingData newBuilding3{ 
		// 	BuildingEnum::RESIDENTIAL,
		// 	BuildingEnum::_1X1,
		// 	BuildingEnum::L3,
		// 	BuildingEnum::RIGHT,
		// 	1, 3, -3, 2, 
		// 	"Big house", "Biggest loveliest house",
		// 	TextureManager::RESIDENTIAL_1X1_L3, RenderSystem::BUILDING};
		// buildingsData.push_back(newBuilding3);

		// //BuildingData newBuilding4{ RESIDENTIAL, _2X1, L1, -1, 5, -8, 3, "Small house", "Small lovely house", 010, 000 };
		// //buildingsData.push_back(newBuilding4);
		// //BuildingData newBuilding5{ RESIDENTIAL, _2X1, L2, -1, 5, -8, 3, "Small house", "Small lovely house", 011, 000 };
		// //buildingsData.push_back(newBuilding5);
		// //BuildingData newBuilding6{ RESIDENTIAL, _2X1, L3, -1, 5, -8, 3, "Small house", "Small lovely house", 012, 000 };
		// //buildingsData.push_back(newBuilding6);

		// BuildingData newBuilding7{ 
		// 	BuildingEnum::COMMERCIAL,
		// 	BuildingEnum::_1X1, 
		// 	BuildingEnum::L1, 
		// 	BuildingEnum::RIGHT,
		// 	3, -1, -1, 1, 
		// 	"Street Stall","Can use your CDC vouchers here",
		// 	TextureManager::COMMERCIAL_1X1_L1, RenderSystem::BUILDING};
		// buildingsData.push_back(newBuilding7);
		// BuildingData newBuilding8{ 
		// 	BuildingEnum::COMMERCIAL, 
		// 	BuildingEnum::_1X1, 
		// 	BuildingEnum::L2, 
		// 	BuildingEnum::RIGHT,
		// 	3, -1, -1, 1, 
		// 	"Small Shop", "Lovely shop with lovely goods", 
		// 	TextureManager::RESIDENTIAL_1X1_L2, RenderSystem::BUILDING };
		// buildingsData.push_back(newBuilding8);
		// BuildingData newBuilding9{ 
		// 	BuildingEnum::COMMERCIAL, 
		// 	BuildingEnum::_1X1, 
		// 	BuildingEnum::L3, 
		// 	BuildingEnum::RIGHT,
		// 	3, -1, -1, 1, 
		// 	"Big Shophouse", "Big shophouse which sells many goods", 
		// 	TextureManager::RESIDENTIAL_1X1_L3, RenderSystem::BUILDING };
		// buildingsData.push_back(newBuilding9);

		// //BuildingData newBuilding10{ COMMERCIAL, _2X1, L1, 5, 0, -5, 1, "Small house", "Small lovely house", 110, 000 };
		// //buildingsData.push_back(newBuilding10);
		// //BuildingData newBuilding11{ COMMERCIAL, _2X1, L2, 5, 0, -5, 1, "Small house", "Small lovely house", 111, 000 };
		// //buildingsData.push_back(newBuilding11);
		// //BuildingData newBuilding12{ COMMERCIAL, _2X1, L3, 5, 0, -5, 1, "Small house", "Small lovely house", 112, 000 };
		// //buildingsData.push_back(newBuilding12);

		// BuildingData newBuilding13{ 
		// 	BuildingEnum::INDUSTRIAL, 
		// 	BuildingEnum::_1X1, 
		// 	BuildingEnum::L1, 
		// 	BuildingEnum::RIGHT,
		// 	-3, -1, 5, 1, 
		// 	"Storage Lot", "Small lot to store some goods",
		// 	TextureManager::INDUSTRIAL_1X1_L1, RenderSystem::BUILDING};
		// buildingsData.push_back(newBuilding13);
		// BuildingData newBuilding14{ 
		// 	BuildingEnum::INDUSTRIAL, 
		// 	BuildingEnum::_1X1, 
		// 	BuildingEnum::L2, 
		// 	BuildingEnum::RIGHT,
		// 	1-3, -1, 5, 1, 
		// 	"Small Warehouse", "Bigger warehouse to store even more goods", 
		// 	TextureManager::INDUSTRIAL_1X1_L2,RenderSystem::BUILDING };
		// buildingsData.push_back(newBuilding14);
		// BuildingData newBuilding15{ 
		// 	BuildingEnum::INDUSTRIAL, 
		// 	BuildingEnum::_1X1, 
		// 	BuildingEnum::L3, 
		// 	BuildingEnum::RIGHT,
		// 	-3, -1, 5, 1, 
		// 	"Big Warehouse", "Big enough to fit 2 IKEAs inside", 
		// 	TextureManager::INDUSTRIAL_1X1_L3, RenderSystem::BUILDING };
		// buildingsData.push_back(newBuilding15);

		//BuildingData newBuilding16{ INDUSTRIAL, _2X2, L1, -8, -5, 8, 0, "Small house", "Small lovely house", 210, 000 };
		//buildingsData.push_back(newBuilding16);
		//BuildingData newBuilding17{ INDUSTRIAL, _2X2, L2, -8, -5, 8, 0, "Small house", "Small lovely house", 211, 000 };
		//buildingsData.push_back(newBuilding17);
		//BuildingData newBuilding18{ INDUSTRIAL, _2X2, L3, -8, -5, 8, 0, "Small house", "Small lovely house", 212, 000 };
		//buildingsData.push_back(newBuilding18);

		dataFile.close();
	}
}
