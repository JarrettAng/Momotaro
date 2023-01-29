#pragma once

#include <EventSystem.h>

namespace PauseManager {
	extern EventSystem::Event<bool> onTogglePause;

	void Initialize();
	void Update();
	void Free();
}
