
#include <iostream>
#include <fstream>

#include <FileIOManager.h>
#include <BuildingManager.h>

namespace FileIOManager {
	void ReadBuildingsData(std::vector<Building>& buildingsData) {
		std::ifstream dataFile{ "Assets/JSON_Data/buildingsData.json" };
		if (!dataFile) {
			std::cerr << "Unable to read buildings data JSON file!\n";
		}

		// Implementation here


		// Store data in the buildingsData vector (array)
		// Temp only!! To be replaced with actual reading of the JSON file on top ^^
		buildingsData.emplace_back(BuildingManager::RESIDENTIAL, BuildingManager::_1X1, BuildingManager::L1, 1, 3, -3, 2);
		buildingsData.emplace_back(BuildingManager::RESIDENTIAL, BuildingManager::_1X1, BuildingManager::L2, 1, 3, -3, 2);
		buildingsData.emplace_back(BuildingManager::RESIDENTIAL, BuildingManager::_1X1, BuildingManager::L3, 1, 3, -3, 2);

		buildingsData.emplace_back(BuildingManager::RESIDENTIAL, BuildingManager::_2X1, BuildingManager::L1, -1, 5, -8, 3);
		buildingsData.emplace_back(BuildingManager::RESIDENTIAL, BuildingManager::_2X1, BuildingManager::L2, -1, 5, -8, 3);
		buildingsData.emplace_back(BuildingManager::RESIDENTIAL, BuildingManager::_2X1, BuildingManager::L3, -1, 5, -8, 3);

		buildingsData.emplace_back(BuildingManager::COMMERCIAL, BuildingManager::_1X1, BuildingManager::L1, 3, -1, -1, 1);
		buildingsData.emplace_back(BuildingManager::COMMERCIAL, BuildingManager::_1X1, BuildingManager::L2, 3, -1, -1, 1);
		buildingsData.emplace_back(BuildingManager::COMMERCIAL, BuildingManager::_1X1, BuildingManager::L3, 3, -1, -1, 1);

		buildingsData.emplace_back(BuildingManager::COMMERCIAL, BuildingManager::_2X1, BuildingManager::L1, 5, 0, -5, 1);
		buildingsData.emplace_back(BuildingManager::COMMERCIAL, BuildingManager::_2X1, BuildingManager::L2, 5, 0, -5, 1);
		buildingsData.emplace_back(BuildingManager::COMMERCIAL, BuildingManager::_2X1, BuildingManager::L3, 5, 0, -5, 1);

		buildingsData.emplace_back(BuildingManager::INDUSTRIAL, BuildingManager::_2X1, BuildingManager::L1, -3, -1, 5, 1);
		buildingsData.emplace_back(BuildingManager::INDUSTRIAL, BuildingManager::_2X1, BuildingManager::L2, -3, -1, 5, 1);
		buildingsData.emplace_back(BuildingManager::INDUSTRIAL, BuildingManager::_2X1, BuildingManager::L3, -3, -1, 5, 1);

		buildingsData.emplace_back(BuildingManager::INDUSTRIAL, BuildingManager::_2X2, BuildingManager::L1, -8, -5, 8, 0);
		buildingsData.emplace_back(BuildingManager::INDUSTRIAL, BuildingManager::_2X2, BuildingManager::L2, -8, -5, 8, 0);
		buildingsData.emplace_back(BuildingManager::INDUSTRIAL, BuildingManager::_2X2, BuildingManager::L3, -8, -5, 8, 0);

		dataFile.close();
	}
}
