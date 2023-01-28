
#include <PauseManager.h>
#include <InputManager.h>
#include <SceneManager.h>

namespace PauseManager {
	bool isPaused;

	EventSystem::Event<bool> onTogglePause;

	#pragma region Forward Delcarations
	void TogglePause();
	void TogglePauseMenu();
	#pragma endregion

	void Initialize() {
		isPaused = false;
		InputManager::onEscPressed.Subscribe(TogglePause);

		onTogglePause.Invoke(isPaused);
	}

	void TogglePause() {
		isPaused = !isPaused;
		TogglePauseMenu();
		onTogglePause.Invoke(isPaused);
	}

	void Free() {
		InputManager::onEscPressed.Unsubscribe(TogglePauseMenu);
	}

	void TogglePauseMenu() {
		// No pause screen, just quit for now
		if (isPaused) {
			SceneManager::LoadScene(SceneManager::QUIT);
		}
	}
}
