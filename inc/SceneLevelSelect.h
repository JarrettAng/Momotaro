/*!************************************************************************
\file:          LevelSelect.hpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          29-03-2023
\brief


The functions include:
-
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <Scene.h>
///////////////////////////////////////////////////////////////////////////

class SceneLevelSelect : public Scene {
public:
	void Load();
	void Initialize();

	void Update();
	void Draw();

	void Free();
	void Unload();
};