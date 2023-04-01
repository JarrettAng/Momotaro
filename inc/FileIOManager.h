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
#include <AudioManager.h>
#include <CardManager.h>


namespace FileIOManager {
    void LoadAudioClipsFromFile(std::array<AudioManager::AudioClip,AudioManager::ClipName::MAX_COUNT>& audioData);
	void ReadBuildingsData(std::vector<BuildingData>& buildingsData);
    void SaveBuildingDataToFile(std::vector<BuildingData>& buildingData);
    void LoadBuildingDataFromFile(std::vector<BuildingData> &buildingsData);
    void ConvertMapFile();
    void SaveGridToFile(std::string fileName);
    void SaveHandToFile(std::vector<Card> const& _hand);
    void LoadHighScoreFromFile();
    void SaveHighScoreToFile();
    std::vector<BuildingData> LoadHandFromFile();
    GridManager::cell* LoadGridFromFile(std::string fileName);
}

