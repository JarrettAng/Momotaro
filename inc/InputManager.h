/*!************************************************************************
\file:          InputManager.h
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

#include <MomoMaths.h>
#include <EventSystem.h>

namespace InputManager {
	extern Vec2<int> mousePos;

	extern EventSystem::Event<Vec2<int>> onMouseClick;
	extern EventSystem::Event<Vec2<int>> onMouseRightClick;
	extern EventSystem::Event<void> onButtonPressed;
	extern EventSystem::Event<void> onEscPressed;
	extern EventSystem::Event<void> onCKeyPressed;
	extern EventSystem::Event<void> onRKeyPressed;
	extern EventSystem::Event<Vec2<int>> on1KeyPressed;
	extern EventSystem::Event<Vec2<int>> on2KeyPressed;
	extern EventSystem::Event<Vec2<int>> on3KeyPressed;
	extern EventSystem::Event<Vec2<int>> onNKeyPressed;

	void HandleInput();
}
