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
