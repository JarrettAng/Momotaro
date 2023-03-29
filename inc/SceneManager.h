/*!************************************************************************
\file:          SceneManager.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <Scene.h>
///////////////////////////////////////////////////////////////////////////

namespace SceneManager {
	enum SCENES_ENUM { // Enum for all scenes in the game
		SPLASHSCREEN,
		MAIN_MENU,
		EDITOR,
		CONTROLS,
		GAME_LEVEL,
		CREDITS,
		LVL_SELECT,

		RESTART,
		QUIT
	};

	extern Scene* currentScene;
	extern SCENES_ENUM previous, current, next;

	// Which scene to load at the very beginning of the game
	void Initialize(SCENES_ENUM startingScene);

	// Things to do at the very end of the game
	void Free();

	// Change the current scene
	void LoadScene(SCENES_ENUM nextScene);
}
