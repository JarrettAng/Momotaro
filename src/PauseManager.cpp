/*!************************************************************************
\file:          PauseManager.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <PauseManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <UIManager.h>
#include <TextureManager.h>

namespace PauseManager {
	bool isPaused;
	UIManager::UIData pauseButtonData{};
	UIManager::UIData continueButtonData{};
	UIManager::UIData exitButtonData{};
	std::vector<UIManager::UIData> pauseMenuBtns;
	EventSystem::Event<bool> onTogglePause;

#pragma region Forward Delcarations;
	void GetButtonStats();
	void Quitgame();
#pragma endregion

	bool IsPaused()
	{
		return isPaused;
	}

	void Initialize()
	{
		isPaused = false;
		InputManager::SubscribeToKey(AEVK_ESCAPE, InputManager::TRIGGERED, TogglePause);

		onTogglePause.Invoke(isPaused);

		pauseButtonData.isActive = true;
		TransformDataToUIData(pauseButtonData, 700.0f, -425.0f, 70.0f, 70.0f);
		pauseButtonData.func = PauseManager::TogglePause;

		continueButtonData.isActive = false;
		TransformDataToUIData(continueButtonData, -250.0f, 50.0f, 250.0f, 100.0f);
		continueButtonData.func = PauseManager::TogglePause;
		continueButtonData.text.text = "CONTINUE";

		exitButtonData.isActive = false;
		TransformDataToUIData(exitButtonData, 50.0f, 50.0f, 250.0f, 100.0f);
		exitButtonData.func = Quitgame;


		pauseMenuBtns.push_back(pauseButtonData);
		pauseMenuBtns.push_back(continueButtonData);
		pauseMenuBtns.push_back(exitButtonData);
		InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, GetButtonStats);
	}

	void PauseManager::Update() {
		pauseMenuBtns[1].isActive = PauseManager::IsPaused();
		pauseMenuBtns[2].isActive = PauseManager::IsPaused();

		if (isPaused) {
			//"WINDOW"
			UIManager::AddRectToBatch(-780, 750, 1500, 1500, 0, TextureManager::GetTexture(TextureManager::PAUSE_WINDOW));
			//TEXT
			UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).L, -0.1f, 0.0f, 0, "PAUSED", { (0.0f), (0.0f), (0.0f) });

			//UNPAUSE
			UIManager::AddButtonToBatch(-250.0f, -50.0f, 30.0f, 30.0f, 0,
				(UIManager::GetFont(UIManager::ROBOTO).M), "CONTINUE",
				{ (1.0f), (1.0f), (1.0f), (1.0f) },
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

	void PauseManager::Draw() {
		if (!IsPaused()) {
			UIManager::AddRectToBatch(700.0f, 425.0f, 70.0f, 70.0f, 0, TextureManager::GetTexture(TextureManager::PAUSE_BUTTON));
		}
	}

	void Quitgame() {
		SceneManager::LoadScene(SceneManager::QUIT);
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
		InputManager::UnsubscribeKey(AEVK_ESCAPE, InputManager::TRIGGERED, TogglePause);
	}

	void GetButtonStats() {
		Vec2<int> mousePos = InputManager::GetMousePos();
		// std::cout << "Mouse has been clicked in UIMANAGER!" << '\n';
		// std::cout << "LIST  COUNT : " << ListOfButtons.size() << '\n';
		for (auto& button : pauseMenuBtns) {
			#if DEBUG
			std::cout << "ButtonStats : " << button.text.text << '\n';
			std::cout << "Button Pos : " << button.transform.x << ", " << button.transform.y << '\n';
			std::cout << "Button Size : " << button.transform.width << ", " << button.transform.height << '\n';
			std::cout << "Mouse Pos : " << mousePos.x << ", " << mousePos.y << '\n';
			std::cout << "Mouse pos offset : " << (mousePos.x - AEGetWindowWidth() / 2) << ", " << (mousePos.y - AEGetWindowHeight() / 2) << '\n';
			std::cout << "Button pos offset : " << button.transform.y + button.transform.height << '\n';
			std::cout << "==================================================================" << '\n';
			#endif
			//Left and right bounds (since mouse is offset by half screen size)
			if (((mousePos.x - AEGetWindowWidth() / 2) > button.transform.x) && (mousePos.x - AEGetWindowWidth() / 2 < button.transform.x + button.transform.width)) {
				// std::cout << "Continue is " << continueButtonData.isActive << '\n';
				//Top and bottom bounds
				if (((mousePos.y - AEGetWindowHeight() / 2) > button.transform.y) && ((mousePos.y - AEGetWindowHeight() / 2) < button.transform.y + button.transform.height)) {
					// std::cout << "BUTTON CLICKED : " << button.text.text << '\n';
					if (button.isActive) button.func();
					// PauseManager::TogglePause();
				}
			}
		}
	}
}
