/*!************************************************************************
\file:          PauseManager.h
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

#include <EventSystem.h>

namespace PauseManager {
	extern EventSystem::Event<bool> onTogglePause;
	bool IsPaused();
	void Initialize();
	void Update();
	void Free();
	void TogglePause();
}
