
#include <PauseManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <UIManager.h>
#include <TextureManager.h>

namespace PauseManager {
	bool isPaused;

	EventSystem::Event<bool> onTogglePause;

#pragma region Forward Delcarations
	//void UnPause();
	//void TogglePauseMenu();
#pragma endregion

    bool IsPaused()
    {
        return isPaused;
    }

    void Initialize()
    {
        isPaused = false;
		InputManager::onEscPressed.Subscribe(TogglePause);

		onTogglePause.Invoke(isPaused);
    }

    void PauseManager::Update() {
		if (isPaused) {
			//"WINDOW"
			UIManager::AddRectToBatch(-780, 750, 1500, 1500, 0, TextureManager::GetTexture(TextureManager::PAUSE_WINDOW));
			//TEXT
			UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).L, -0.1f, 0.0f, 0, "PAUSED", { (0.0f), (0.0f), (0.0f) });

			//UNPAUSE
			UIManager::AddButtonToBatch(-250.0f, -50.0f, 30.0f, 30.0f, 0, 
										(UIManager::GetFont(UIManager::ROBOTO).M), "CONTINUE", 
										{ (1.0f), (1.0f), (1.0f), (1.0f) } , 
										{ (0.0f), (0.0f), (0.0f) });

			//EXIT
			UIManager::AddButtonToBatch(50.0f, -50.0f, 80.0f, 30.0f, 0,
										(UIManager::GetFont(UIManager::ROBOTO).M), "EXIT",
										{ (1.0f), (1.0f), (1.0f), (1.0f) },
										{ (0.0f), (0.0f), (0.0f) });

			//Check if button is pressed
			//InputManager::onButtonPressed.Subscribe(Free);

		}
	}

	void TogglePause() {
		isPaused = !isPaused;
		//TogglePauseMenu();
		onTogglePause.Invoke(isPaused);
	}

	//void UnPause() {
	//	isPaused = isPaused;
	//	//TogglePauseMenu();
	//	onTogglePause.Invoke(isPaused);
	//}

	void Free() {
		InputManager::onEscPressed.Unsubscribe(TogglePause);
	}
}
