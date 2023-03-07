/*!************************************************************************
\file:          SceneManager.cpp
\author:        Jarrett Ang
\par DP email:  a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <AEEngine.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <PauseManager.h>

#include <SceneGameLevel.h>
#include <SceneSplashscreen.h>
///////////////////////////////////////////////////////////////////////////

namespace SceneManager {
	// Scene states
	SCENES_ENUM previous, current, next;

	// All scenes
	Scene* currentScene;

	SceneSplashscreen splashScreen;
	SceneGameLevel gameLevel;
	// SceneMainMenu mainMenu;
	// SceneSettings settings;
	// SceneCredits  credits;
	// SceneEditor   editor;
	// 
	///////////////////////////////////////////////////////////////////////
	// Forward Declarations
	void UpdatePausedState(bool newPausedState);
	///////////////////////////////////////////////////////////////////////

	bool isPaused;

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
		case GAME_LEVEL:
			currentScene = &gameLevel;
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

			currentScene->Initialize();

			// Game update loop
			while (current == next) {
				AESysFrameStart();

				AEInputUpdate();
				InputManager::HandleInput();

				currentScene->Update();

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

		PauseManager::onTogglePause.Subscribe(UpdatePausedState);

		SwitchScene();
		SceneManagerLoop();
	}

	// Things to do at the very end of the game
	void Free() {
		PauseManager::onTogglePause.UnsubscribeAll();
		FontManager::Unload();
		TextureManager::Unload();
	}

	void UpdatePausedState(bool newPausedState) {
		isPaused = newPausedState;
	}
}
