/*!************************************************************************
\file:          SceneGameLevel.h
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

#include <Scene.h>

class SceneGameLevel : public Scene {
public:
	void Load();
	void Initialize();

	void Update();
	void Draw();

	void Free();
	void Unload();
};

void MapToLoad(std::string const& mapFilePath);
std::string const& GetCurrentMapName();
