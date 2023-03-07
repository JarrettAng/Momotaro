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
#include <GridManager.h>

namespace FileIOManager {
	// Implementation here
	// void to_json(json& _json, BuildingData const& _building){
	// }
	void SaveGridToFile(){
		std::ofstream mapFile("Assets/JSON_Data/Maps/map.momomaps");
		if(!mapFile.is_open()){
			std::cerr << "Unable to write to file!\n";
			assert(0);
		}
		mapFile << "Width " << GridManager::gridX <<'\n';
		mapFile << "Height " << GridManager::gridY <<'\n';
		for(int y{0}; y < GridManager::gridY; ++y){
			for(int x{0}; x < GridManager::gridX; ++x){
				mapFile << static_cast<int>((GridManager::GetGrid()+GridManager::GetIndex(Vec2<int>{x,y}))->isRenderable);
				if(x < GridManager::gridX-1) mapFile << ' ';
			}
			mapFile<<'\n';
		}
	}

	void ReadBuildingsData(std::vector<BuildingData>& buildingsData) {
		std::ifstream dataFile{ "Assets/JSON_Data/buildingsData.json" };
		// json data = json::parse(dataFile);
		if (!dataFile) {
			std::cerr << "Error " <<__FILE__ << "ln" << __LINE__ << ": Unable to read buildings data JSON file!\n";
		}

		// BuildingData testBuilding{};
		// from_json(data,testBuilding);
		// std::cout << "=====TEST BUILDING=====\n";
		// std::cout << testBuilding << '\n';
		// dataFile.close();

// return;

		// Store data in the buildingsData vector (array)
		// Temp only!! To be replaced with actual reading of the JSON file on top ^^
		BuildingData newBuilding{ 
			BuildingEnum::RESIDENTIAL, 
			Vec2<int>{1,1},
			BuildingEnum::L1, 
			BuildingEnum::RIGHT,
			1, 3, -5, 1,
			"Small house", "Starter red house with an actual working chimney!",
			TextureManager::RESIDENTIAL_1X1_L1};
		buildingsData.push_back(newBuilding);
		BuildingData newBuilding2{
			 BuildingEnum::RESIDENTIAL,
			 Vec2<int>{1,1}, 
			 BuildingEnum::L2,
			 BuildingEnum::RIGHT,
			 1, 3, -5, 1, 
			 "Terrace house", "Bigger blue house with some lovely flowers.",
			 TextureManager::RESIDENTIAL_1X1_L2};
		buildingsData.push_back(newBuilding2);
		BuildingData newBuilding3{ 
			BuildingEnum::RESIDENTIAL,
			Vec2<int>{1,1},
			BuildingEnum::L3,
			BuildingEnum::RIGHT,
			1, 3, -5, 1, 
			"Shiny house", "For the rich people, we made your roof shine too.",
			TextureManager::RESIDENTIAL_1X1_L3};
		buildingsData.push_back(newBuilding3);

		//BuildingData newBuilding4{ RESIDENTIAL, _2X1, L1, -1, 5, -8, 3, "Small house", "Small lovely house", 010, 000 };
		//buildingsData.push_back(newBuilding4);
		//BuildingData newBuilding5{ RESIDENTIAL, _2X1, L2, -1, 5, -8, 3, "Small house", "Small lovely house", 011, 000 };
		//buildingsData.push_back(newBuilding5);
		//BuildingData newBuilding6{ RESIDENTIAL, _2X1, L3, -1, 5, -8, 3, "Small house", "Small lovely house", 012, 000 };
		//buildingsData.push_back(newBuilding6);

		BuildingData newBuilding7{ 
			BuildingEnum::COMMERCIAL,
			Vec2<int>{1,1}, 
			BuildingEnum::L1, 
			BuildingEnum::RIGHT,
			3, -1, 1, 1, 
			"Street Stall","Can use your CDC vouchers here",
			TextureManager::COMMERCIAL_1X1_L1};
		buildingsData.push_back(newBuilding7);
		BuildingData newBuilding8{ 
			BuildingEnum::COMMERCIAL, 
			Vec2<int>{1,1}, 
			BuildingEnum::L2, 
			BuildingEnum::RIGHT,
			3, -1, 1, 1, 
			"Small Shop", "Lovely shop with lovely goods", 
			TextureManager::COMMERCIAL_1X1_L2};
		buildingsData.push_back(newBuilding8);
		BuildingData newBuilding9{ 
			BuildingEnum::COMMERCIAL, 
			Vec2<int>{1,1},
			BuildingEnum::L3, 
			BuildingEnum::RIGHT,
			3, -1, 1, 1, 
			"Big Shophouse", "Big shophouse which sells many goods", 
			TextureManager::COMMERCIAL_1X1_L3};
		buildingsData.push_back(newBuilding9);

		//BuildingData newBuilding10{ COMMERCIAL, _2X1, L1, 5, 0, -5, 1, "Small house", "Small lovely house", 110, 000 };
		//buildingsData.push_back(newBuilding10);
		//BuildingData newBuilding11{ COMMERCIAL, _2X1, L2, 5, 0, -5, 1, "Small house", "Small lovely house", 111, 000 };
		//buildingsData.push_back(newBuilding11);
		//BuildingData newBuilding12{ COMMERCIAL, _2X1, L3, 5, 0, -5, 1, "Small house", "Small lovely house", 112, 000 };
		//buildingsData.push_back(newBuilding12);

		BuildingData newBuilding13{ 
			BuildingEnum::INDUSTRIAL, 
			Vec2<int>{1,1}, 
			BuildingEnum::L1, 
			BuildingEnum::RIGHT,
			-5, 1, 5, 0, 
			"Storage Box", "A small boxy building to store boxes.",
			TextureManager::INDUSTRIAL_1X1_L1};
		buildingsData.push_back(newBuilding13);
		BuildingData newBuilding14{ 
			BuildingEnum::INDUSTRIAL, 
			Vec2<int>{1,1}, 
			BuildingEnum::L2, 
			BuildingEnum::RIGHT,
			-5, 1, 5, 0, 
			"Big Lab", "Discover the secrets of life here. Hint: It's ligma.", 
			TextureManager::INDUSTRIAL_1X1_L2};
		buildingsData.push_back(newBuilding14);
		BuildingData newBuilding15{ 
			BuildingEnum::INDUSTRIAL, 
			Vec2<int>{1,1}, 
			BuildingEnum::L3, 
			BuildingEnum::RIGHT,
			-5, 1, 5, 0, 
			"Power Plant", "Powered by the dead.", 
			TextureManager::INDUSTRIAL_1X1_L3};
		buildingsData.push_back(newBuilding15);

		//BuildingData newBuilding16{ INDUSTRIAL, _2X2, L1, -8, -5, 8, 0, "Small house", "Small lovely house", 210, 000 };
		//buildingsData.push_back(newBuilding16);
		//BuildingData newBuilding17{ INDUSTRIAL, _2X2, L2, -8, -5, 8, 0, "Small house", "Small lovely house", 211, 000 };
		//buildingsData.push_back(newBuilding17);
		//BuildingData newBuilding18{ INDUSTRIAL, _2X2, L3, -8, -5, 8, 0, "Small house", "Small lovely house", 212, 000 };
		//buildingsData.push_back(newBuilding18);

		dataFile.close();
	}
}
