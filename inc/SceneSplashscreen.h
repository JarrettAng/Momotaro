/*!************************************************************************
\file:          SceneSplashscreen.h
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

class SceneSplashscreen : public Scene {
public:
	void Load();
	void Initialize();

	void Update();
	void FixedUpdate();
	void Draw();

	void Free();
	void Unload();
};
