
#include <PauseManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <UIManager.h>
#include <TextureManager.h>

namespace PauseManager {
	bool isPaused;

	EventSystem::Event<bool> onTogglePause;

#pragma region Forward Delcarations
	void TogglePause();
	//void TogglePauseMenu();
#pragma endregion

	void Initialize() {
		isPaused = false;
		InputManager::onEscPressed.Subscribe(TogglePause);

		onTogglePause.Invoke(isPaused);
	}

	void PauseManager::Update() {
		if (isPaused) {
			UIManager::AddRectToBatch(-750, 750, 1500, 1500, 0, TextureManager::GetTexure(TextureManager::PAUSE_WINDOW));
			UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).M, -0.1f, 0.0f, 0, "PAUSED", { (0.0f), (0.0f), (0.0f) });
		}
	}

	void TogglePause() {
		isPaused = !isPaused;
		//TogglePauseMenu();
		onTogglePause.Invoke(isPaused);
	}

	void Free() {
		InputManager::onEscPressed.Unsubscribe(TogglePause);
	}
}
