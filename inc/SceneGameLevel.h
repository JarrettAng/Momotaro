/*!************************************************************************
\file:          SceneGameLevel.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This header file handles the main gameplay scene for the game.

The functions include:
- MapToLoad
Sets which map to load for the main gameplay (Used by LevelSelect)
- GetCurrentMapName
Gets which map is currently loaded
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <Scene.h>
///////////////////////////////////////////////////////////////////////////

class SceneGameLevel : public Scene {
public:
	void Load();
	void Initialize();

	void Update();
	void Draw();

	void Free();
	void Unload();
};

///////////////////////////////////////////////////////////////////////////

void MapToLoad(std::string const& mapFilePath); // Sets which map to load for the main gameplay (Used by LevelSelect)
std::string const& GetCurrentMapName();			// Gets which map is currently loaded
