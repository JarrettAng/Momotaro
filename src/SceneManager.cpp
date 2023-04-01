/*!************************************************************************
\file:          SceneManager.cpp
\author:        Jarrett Ang
\par DP email:  a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This source file implements the SceneManager header file, it handles the 
loading and swapping of "scenes" in the game.

The functions include:
- Initialize
Starts the game loop and loads the scene given in the parameter
- LoadScene
Changes the current scene to the one given in the parameter
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <AEEngine.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <PauseManager.h>
#include <DebugManager.h>

#include <SceneSplashscreen.h>
#include <SceneMainMenu.h>
#include <SceneGameLevel.h>
#include <SceneCredits.h>
#include <SceneLevelEditor.h>
#include <SceneTutorial.h>
#include <SceneLevelSelect.h>
///////////////////////////////////////////////////////////////////////////

namespace SceneManager {
	// Scene states
	SCENES_ENUM previous, current, next;

	// All scenes
	Scene* currentScene;

	SceneSplashscreen splashScreen;
	SceneMainMenu	  mainMenu;
	SceneGameLevel	  gameLevel;
	SceneCredits	  credits;
	SceneLevelEditor  editor;
	SceneTutorial	  tutorial;
	SceneLevelSelect  levelSelect;

	///////////////////////////////////////////////////////////////////////
	// Forward Declarations
	void SceneManagerLoop();
	void SwitchScene();
	///////////////////////////////////////////////////////////////////////

	// Change the current scene (Public; used by other files)
	void LoadScene(SCENES_ENUM nextScene) {
		next = nextScene;
	}

	// Loads the funcs to call in the loop below based on "current"
	void SwitchScene() {
		switch (current) {
		case SPLASHSCREEN:
			currentScene = &splashScreen;
			break;
		case MAIN_MENU:
			currentScene = &mainMenu;
			break;
		case GAME_LEVEL:
			currentScene = &gameLevel;
			break;
		case CREDITS:
			currentScene = &credits;
			break;
		case EDITOR:
			currentScene = &editor;
			break;
		case CONTROLS:
			currentScene = &tutorial;
			break;
		case LVL_SELECT:
			currentScene = &levelSelect;
			break;
		}
	}

	// Main game loop
	void SceneManagerLoop() {
		while (current != QUIT) {
			if (current == RESTART) {
				next = current = previous;
			}
			else {
				SwitchScene();
				currentScene->Load();
			}

			Debug::EnableControls();		// Debug needs persistence
			currentScene->Initialize();

			// Game update loop
			while (current == next) {
				AESysFrameStart();

				AEInputUpdate();
				InputManager::HandleInput();

				currentScene->Update();

				Debug::ShowDebugStatus();	// Debug needs persistence
				currentScene->Draw();

				AESysFrameEnd();
			}

			currentScene->Free();

			if (next != RESTART) {
				currentScene->Unload();
			}

			previous = current;
			current = next;
		}

		Free();
	}

	// Which scene to load at the very beginning of the game
	void Initialize(SCENES_ENUM startingScene) {
		current = previous = next = startingScene;

		SwitchScene();
		SceneManagerLoop();
	}

	// Things to do at the very end of the game
	void Free() {
		InputManager::Free();
	}
}
