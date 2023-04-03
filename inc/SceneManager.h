/*!************************************************************************
\file:          SceneManager.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This header file handles the loading and swapping of "scenes" in the game.

The functions include:
- Initialize
Starts the game loop and loads the scene given in the parameter
- LoadScene
Changes the current scene to the one given in the parameter

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
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

	extern Scene* currentScene; // The current scene, SceneManager will call the functions of this
	extern SCENES_ENUM previous, current, next;

	// Which scene to load at the very beginning of the game
	void Initialize(SCENES_ENUM startingScene);

	// Things to do at the very end of the game
	void Free();

	// Change the current scene
	void LoadScene(SCENES_ENUM nextScene);
}
