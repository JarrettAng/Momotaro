#pragma once

namespace SceneManager {
	enum SCENES_ENUM {
		MAIN_MENU,
		GAME_LEVEL,

		RESTART,
		QUIT
	};

	extern SCENES_ENUM previous, current, next;

	void Initialize(SCENES_ENUM startingScene);

	void LoadScene(SCENES_ENUM nextScene);
}
