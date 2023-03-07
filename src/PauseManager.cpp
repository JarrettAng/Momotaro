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
#include <TextureManager.h>
#include <RenderSystem.h>
#include <FontManager.h>

namespace PauseManager {
	bool isPaused;
	RenderSystem::Interactable pauseButtonData{};
	RenderSystem::Interactable continueButtonData{};
	RenderSystem::Interactable exitButtonData{};
	std::vector<RenderSystem::Interactable> pauseMenuBtns;
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

		pauseButtonData.render.rect.transform.pos.x = 700.0f;
		pauseButtonData.render.rect.transform.pos.y = -425.0f;
		pauseButtonData.render.rect.transform.size.x = 70.0f;
		pauseButtonData.render.rect.transform.size.y = 70.0f;
		pauseButtonData.isActive = true;
		pauseButtonData.func = TogglePause;

		continueButtonData.render.rect.transform.pos.x = -250.0f;
		continueButtonData.render.rect.transform.pos.y = 50.0f;
		continueButtonData.render.rect.transform.size.x = 250.0f;
		continueButtonData.render.rect.transform.size.y = 100.0f;
		continueButtonData.isActive = false;
		continueButtonData.func = TogglePause;



		exitButtonData.render.rect.transform.pos.x = 50.0f;
		exitButtonData.render.rect.transform.pos.y = 50.0f;
		exitButtonData.render.rect.transform.size.x = 250.0f;
		exitButtonData.render.rect.transform.size.y = 100.0f;
		exitButtonData.isActive = false;
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
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, -755, 750, 1500, 1500, TextureManager::PAUSE_WINDOW);
			//TEXT
			RenderSystem::AddTextToBatch(RenderSystem::UI_BATCH, -0.1f, 0.0f, FontManager::GetFont(FontManager::ROBOTO), 60, "PAUSED", 0, { 0,0,0 });

			//UNPAUSE

			RenderSystem::AddButtonToBatch(RenderSystem::UI_BATCH, -250.0f, -50.0f, 30.0f, 30.0f, FontManager::GetFont(FontManager::ROBOTO), 40, "CONTINUE", 0, { 1,1,1,1 }, { 0,0,0 });

			//EXIT
			RenderSystem::AddButtonToBatch(RenderSystem::UI_BATCH, 50.0f, -50.0f, 80.0f, 30.0f, FontManager::GetFont(FontManager::ROBOTO), 40, "EXIT", 0, { 1,1,1,1 }, { 0,0,0 });

			//Check if button is pressed
			//InputManager::onButtonPressed.Subscribe(Free);

		}
	}

	void PauseManager::Draw() {
		if (!IsPaused()) {
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, 700.0f, 425.0f, 70.0f, 70.0f, TextureManager::PAUSE_BUTTON);
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
			if (((mousePos.x - AEGetWindowWidth() / 2) > button.render.rect.transform.pos.x) && (mousePos.x - AEGetWindowWidth() / 2 < button.render.rect.transform.pos.x + button.render.rect.transform.size.x)) {
				// std::cout << "Continue is " << continueButtonData.isActive << '\n';
				//Top and bottom bounds
				if (((mousePos.y - AEGetWindowHeight() / 2) > button.render.rect.transform.pos.y) && ((mousePos.y - AEGetWindowHeight() / 2) < button.render.rect.transform.pos.y + button.render.rect.transform.size.y)) {
					// std::cout << "BUTTON CLICKED : " << button.text.text << '\n';
					if (button.isActive) button.func();
					// PauseManager::TogglePause();
				}
			}
		}
	}
}
