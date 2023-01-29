
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
	extern EventSystem::Event<void> on1KeyPressed;
	extern EventSystem::Event<void> on2KeyPressed;
	extern EventSystem::Event<void> on3KeyPressed;

	void HandleInput();
}
