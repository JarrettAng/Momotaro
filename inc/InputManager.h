
#pragma once

#include <MomoMaths.h>
#include <EventSystem.h>

namespace InputManager {
	extern Vec2<int> mousePos;

	extern EventSystem::Event<Vec2<int>> onMouseClick;

	void HandleInput();
}
