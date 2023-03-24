/*!************************************************************************
\file:          AudioManager.h
\author:		Amadeus Chia
\par DP email:	amadeusjinhan.chia@digipen.edu
\par Course:    CSD1451B
\par Software Engineering Project
\date:          24-03-2023
\brief
This header file handles everything audio related

The functions include:
- Initialize
Reads the building data from the JSON file upon loading into the game scene
- Clear
Resets the data read on game scene exit
- GetBuildingData
Returns the building information of the given type, size, level
- GetRandomBuildingData
Returns a random building information, can be controlled by parameters
**************************************************************************/
#pragma once
#include <AEAudio.h>

namespace AudioManager {
	void Initialize();
	void Load();
	void Unload();
}