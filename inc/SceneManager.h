#pragma once

enum SCENES_ENUM {
	MAIN_MENU,
	GAME_LEVEL,

	RESTART,
	QUIT
};

SCENES_ENUM previous, current, next;

void LoadScene(SCENES_ENUM nextScene);
