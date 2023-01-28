
#include <PauseManager.h>

namespace PauseManager {
	bool isPaused;

	EventSystem::Event<bool> onTogglePause;

	void Initialize() {
		isPaused = false;
		onTogglePause.Invoke(isPaused);
	}

	void TogglePause() {
		isPaused = !isPaused;
		onTogglePause.Invoke(isPaused);
	}
}
