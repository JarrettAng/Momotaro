/*!************************************************************************
\file GameOverManager.cpp
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 29-02-2023
\brief
This source file handles the game over state of the game.
**************************************************************************/
#include <GameOverManager.h>

namespace GameOverManager {
	/*!***********************************************************************
	* Const variables.
	*************************************************************************/
	const Vec2<float> POINTER_OFFSET = { 80.0f, 10.0f };
	const float multiplier{1.f};

	// Game over state of the game.
	static bool GAME_OVER = false;

	/*!***********************************************************************
	* UI.
	*************************************************************************/
	RenderSystem::Interactable retryBtn;
	RenderSystem::Interactable quitBtn;
	
	RenderSystem::Renderable gameOverDisplay;

	std::vector<RenderSystem::Interactable> buttons;

	/*!***********************************************************************
	* Forward Declarations.
	*************************************************************************/
	void Restart();
	void LoadMainMenu();
	void GameOver();
	void HandleBtnHover();
	void HandleBtnClick();
	void DrawGameOverUI();
	void InitializeGameOverUI();

	/*!***********************************************************************
	\brief
		Initialize the GameOverManager.
	*************************************************************************/
	void Initialize() {
		// Initialize UI.
		InitializeGameOverUI();

		GAME_OVER = false;

		// Subscribe to events to trigger game over.
		GridManager::onBoardFull.Subscribe(GameOver);
		CardManager::onHandEmpty.Subscribe(GameOver);

		// Subscribe to key events.
		InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleBtnClick);
	}

	/*!***********************************************************************
	\brief
		Update the GameOverManager.
	*************************************************************************/
	void Update() {
		if (!GAME_OVER) return;
	}

	/*!***********************************************************************
	\brief
		Draw the GameOverManager.
	*************************************************************************/
	void Draw() {
		if (!GAME_OVER) return;

		// Draw visual feedback.
		HandleBtnHover();
		// Draw UI.
		DrawGameOverUI();
	}

	/*!***********************************************************************
	\brief
		Free the GameOverManager.
	*************************************************************************/
	void Free() {
		buttons.clear();
		// Unsubscribe from events.
		GridManager::onBoardFull.Unsubscribe(GameOver);
		CardManager::onHandEmpty.Unsubscribe(GameOver);
		InputManager::UnsubscribeKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleBtnClick);
	}

	/*!***********************************************************************
	\brief
		Check if it is game over.
	\return
		True / false.
	*************************************************************************/
    bool IsGameOver() {
        return GAME_OVER;
    }

	/*!***********************************************************************
	\brief
		Enable game over.
	*************************************************************************/
    void GameOver() {
		if(!GAME_OVER) AudioManager::PlayAudioClip(AudioManager::ClipName::SFX_GAMEOVER);
		GAME_OVER = true;
	}

	/*!***********************************************************************
	\brief
		Initialize game over UI.
	*************************************************************************/
	void InitializeGameOverUI() {
		// GAME OVER BACKGROUND
		gameOverDisplay.rect.graphics.tex = TextureManager::GAMEOVER_BG;
		gameOverDisplay.layer = 10;

		gameOverDisplay.rect.transform.pos.x = GetWorldXByPercentage(13.0f);
		gameOverDisplay.rect.transform.pos.y = GetWorldYByPercentage(90.0f);

		gameOverDisplay.rect.transform.size.x = 1200.0f*multiplier;
		gameOverDisplay.rect.transform.size.y = 690.0f*multiplier;

		// RETRY BUTTON.
		retryBtn.render.rect.graphics.tex = TextureManager::RETRY_BTN;
		retryBtn.render.layer = 11;
		retryBtn.func = Restart;

		retryBtn.render.rect.transform.pos.x = GetWorldXByPercentage(30.0f);
		retryBtn.render.rect.transform.pos.y = GetWorldYByPercentage(37.0f);

		retryBtn.render.rect.transform.size.x = 230.0f*multiplier;
		retryBtn.render.rect.transform.size.y = 100.0f*multiplier;
		retryBtn.render.rect.transform.cachedSize = retryBtn.render.rect.transform.size;

		buttons.push_back(retryBtn);

		// QUIT BUTTON.
		quitBtn.render.rect.graphics.tex = TextureManager::QUIT_BTN;
		quitBtn.render.layer = 11;
		quitBtn.func = LoadMainMenu;

		quitBtn.render.rect.transform.pos.x = GetWorldXByPercentage(60.0f);
		quitBtn.render.rect.transform.pos.y = GetWorldYByPercentage(37.0f);

		quitBtn.render.rect.transform.size.x = 170.0f*multiplier;
		quitBtn.render.rect.transform.size.y = 100.0f*multiplier;
		quitBtn.render.rect.transform.cachedSize = quitBtn.render.rect.transform.size;

		buttons.push_back(quitBtn);
	}

	/*!***********************************************************************
	\brief
		Handle visual feedback when button is hovered.
	*************************************************************************/
	void HandleBtnHover() {
		// Cache mouse position.
		Vec2<int> mousePos = InputManager::GetMousePos();

		// Convert to world space position.
		mousePos.x -= (int)AEGfxGetWinMaxX();
		mousePos.y -= (int)AEGfxGetWinMaxY();

		// Loop through all buttons.
		for (RenderSystem::Interactable& btn : buttons) {
			// Skip if button is not visible or clickable.
			if (!btn.isActive || !btn.isClickable) continue;

			// Check if mouse is hovering button.
			if (MouseInsideButton(mousePos, btn.render.rect.transform.pos, btn.render.rect.transform.size)) {
				// Scale btn for visual feedback.
				btn.render.rect.transform.size = btn.render.rect.transform.cachedSize * 1.1f;

				// Dont draw pointer for quit/restart button.
				if (btn.render.rect.graphics.tex == TextureManager::PAUSE_BUTTON) continue;
				if (btn.render.rect.graphics.tex == TextureManager::RESTART_BUTTON) continue;

				// Draw pointer.
				RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x - POINTER_OFFSET.x, btn.render.rect.transform.pos.y - POINTER_OFFSET.y, 60, 90, TextureManager::POINTER, 11);
			}
			else {
				// Scale btn to original size.
				btn.render.rect.transform.size = btn.render.rect.transform.cachedSize;
			}
		}
	}

	/*!***********************************************************************
	\brief
		Handle player click on a button.
	*************************************************************************/
	void HandleBtnClick() {
		if (!GAME_OVER) return;

		// Cache mouse position.
		Vec2<int> mousePos = InputManager::GetMousePos();

		// Convert to world space position.
		mousePos.x -= (int)AEGfxGetWinMaxX();
		mousePos.y -= (int)AEGfxGetWinMaxY();

		// Loop through all buttons.
		for (RenderSystem::Interactable& btn : buttons) {
			// Skip if button is not visible or clickable.
			if (!btn.isActive || !btn.isClickable) continue;

			// Check if mouse is hovering button.
			if (MouseInsideButton(mousePos, btn.render.rect.transform.pos, btn.render.rect.transform.size)) {
				btn.func();
				return;
			}
		}
	}

	/*!***********************************************************************
	\brief
		Draw game over UI.
	*************************************************************************/
	void DrawGameOverUI() {
		// Draw game over background.
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, gameOverDisplay.rect.transform.pos.x, gameOverDisplay.rect.transform.pos.y, gameOverDisplay.rect.transform.size.x, gameOverDisplay.rect.transform.size.y, gameOverDisplay.rect.graphics.tex, gameOverDisplay.layer);

		// Draw buttons.
		for (RenderSystem::Interactable const& i : buttons) {
			if (i.isActive) {
				RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, i.render.rect.transform.pos.x, i.render.rect.transform.pos.y, i.render.rect.transform.size.x, i.render.rect.transform.size.y, i.render.rect.graphics.tex, i.render.layer);
			}
		}
	}

	/*!***********************************************************************
	\brief
		Restart scene.
	*************************************************************************/
	void Restart() {
		SceneManager::LoadScene(SceneManager::RESTART);
	}

	/*!***********************************************************************
	\brief
		Back to main menu scene.
	*************************************************************************/
	void LoadMainMenu() {
		SceneManager::LoadScene(SceneManager::MAIN_MENU);
	}
}