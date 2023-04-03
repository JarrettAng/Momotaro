/*!************************************************************************
\file:          FileIOMAnager.h
\author:		Amadeus Chia
\par DP email:	amadeusjinhan.chia@digipen.edu
\par Course:    CSD1451B
\par Software Engineering Project
\date:          24-03-2023
\brief
This header file handles everything FileIO related. 

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <Building.h>
#include <GridManager.h>
#include <AudioManager.h>
#include <CardManager.h>
///////////////////////////////////////////////////////////////////////////
namespace FileIOManager {

    ///////////////////////////////////////////////////////////////////////
    // Loading Functions
    /**
     * @brief Loads audioclips from file to the audioclip array for the audio manager
     * (Uses AEAudio functions to load!)
     * @param audioData Array of audio data to load audio into 
     */
    void LoadAudioClipsFromFile(std::array<AudioManager::AudioClip,AudioManager::ClipName::MAX_COUNT>& audioData);

    /**
     * @brief Loads building data from file to the building data vector
     * (This loads data like building names, type, desc and synergy!)
     * @param buildingsData Vector of building dadta to load into
     */
    void LoadBuildingDataFromFile(std::vector<BuildingData> &buildingsData);

    /**
     * @brief Directly loads highscore values into the scoremanager
     * (Scoremanager always saves the highest score)
     */
    void LoadHighScoreFromFile();

    /**
     * @brief Directly loads the last played hand into the cardmanager
     * (This should only be used when resuming a game!)
     * @return std::vector<BuildingData> Building Data to initialise hand in cardmanager with.
     */
    std::vector<BuildingData> LoadHandFromFile();

    /**
     * @brief Loads a map and it's saved score and level from file
     * (This also loads the score and it's threshold! They are all map specific)
     * @param fileName Filepath of the map to load
     * @return GridManager::cell* Cell Array of the map
     */
    GridManager::cell* LoadGridFromFile(std::string fileName);


    //////////////////////////////////////////////////////////////////////
    // Saving Functions
    /**
     * @brief Saves the current grid to file
     * (This also saves the current score and its threshold)
     * @param fileName filepath to save the grid to
     */
    void SaveGridToFile(std::string fileName);

    /**
     * @brief Saves the current hand to file
     * (This takes the hand and only saves the buildingData from the hand)
     * @param _hand vector of cards to read from (from CardManager.cpp)
     */
    void SaveHandToFile(std::vector<Card> const& _hand);

    /**
     * @brief Saves the highscore to file (from ScoreManager.cpp)
     * (ScoreManager should only save the highest score!)
     */
    void SaveHighScoreToFile();

    ///////////////////////////////////////////////////////////////////////
    // Helper Functions (DEPRECATED)

    /**
     * @brief Helper function to convert old map saves into the new map saves
     * (This is just so we don't have to manually retype it all)
     */
    void ConvertMapFile();
    /**
     * @brief Helper function to help save hardcoded data to file 
     * (This just makes it so we don't have to type out the whole building data save)
     * @param buildingData vector of building data to save from
     */
    void SaveBuildingDataToFile(std::vector<BuildingData>& buildingData);

    ///////////////////////////////////////////////////////////////////////
    //!DEPRECATED CODE! DO NOT USE!
	void ReadBuildingsData(std::vector<BuildingData>& buildingsData);
}

