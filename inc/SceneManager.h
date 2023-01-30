/*!************************************************************************
\file:          SceneManager.h
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

namespace SceneManager {
	enum SCENES_ENUM {
		SPLASHSCREEN,
		MAIN_MENU,
		GAME_LEVEL,

		RESTART,
		QUIT
	};

	extern Scene* currentScene;

	extern SCENES_ENUM previous, current, next;

	void Initialize(SCENES_ENUM startingScene);

	void Free();

	void LoadScene(SCENES_ENUM nextScene);
}
