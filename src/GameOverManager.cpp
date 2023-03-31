/*!************************************************************************
\file GameOverManager.cpp
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 29-02-2023
\brief
This source file declares

**************************************************************************/
#include <GameOverManager.h>

namespace GameOverManager {
	const Vec2<float> POINTER_OFFSET = { 80.0f, 10.0f };

	static bool GAME_OVER = false;

	RenderSystem::Interactable retryBtn;
	RenderSystem::Interactable quitBtn;

	RenderSystem::Renderable gameOverDisplay;

	std::vector<RenderSystem::Interactable> buttons;

#pragma region Forward Delcarations
	void Restart();
	void LoadMainMenu();
	void GameOver();
	void HandleBtnHover();
	void HandleBtnClick();
	void DrawGameOverUI();
	void InitializeGameOverUI();
	//bool IsGameOver() { return GAME_OVER; }
#pragma endregion

	void Initialize() {
		InitializeGameOverUI();

		GAME_OVER = false;

		GridManager::onBoardFull.Subscribe(GameOver);
		CardManager::onHandEmpty.Subscribe(GameOver);
		InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleBtnClick);
	}

	void Update() {
		if (!GAME_OVER) return;
	}
	void Draw() {
		if (!GAME_OVER) return;

		HandleBtnHover();
		DrawGameOverUI();
	}
	void Free() {
		buttons.clear();
		GridManager::onBoardFull.Unsubscribe(GameOver);
		CardManager::onHandEmpty.Unsubscribe(GameOver);
		InputManager::UnsubscribeKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleBtnClick);
	}

    bool IsGameOver() {
        return GAME_OVER;
    }

    void GameOver() {
		AudioManager::PlayAudioClip(AudioManager::ClipName::SFX_GAMEOVER);
		GAME_OVER = true;
	}

	void InitializeGameOverUI() {
		// GAME OVER BACKGROUND
		gameOverDisplay.rect.graphics.tex = TextureManager::GAMEOVER_BG;
		gameOverDisplay.layer = 10;

		gameOverDisplay.rect.transform.pos.x = GetWorldXByPercentage(13.0f);
		gameOverDisplay.rect.transform.pos.y = GetWorldYByPercentage(90.0f);

		gameOverDisplay.rect.transform.size.x = 1200.0f;
		gameOverDisplay.rect.transform.size.y = 690.0f;

		// RETRY BUTTON.
		retryBtn.render.rect.graphics.tex = TextureManager::RETRY_BTN;
		retryBtn.render.layer = 11;
		retryBtn.func = Restart;

		retryBtn.render.rect.transform.pos.x = GetWorldXByPercentage(30.0f);
		retryBtn.render.rect.transform.pos.y = GetWorldYByPercentage(37.0f);

		retryBtn.render.rect.transform.size.x = 230.0f;
		retryBtn.render.rect.transform.size.y = 100.0f;
		retryBtn.render.rect.transform.cachedSize = retryBtn.render.rect.transform.size;

		buttons.push_back(retryBtn);

		// QUIT BUTTON.
		quitBtn.render.rect.graphics.tex = TextureManager::QUIT_BTN;
		quitBtn.render.layer = 11;
		quitBtn.func = LoadMainMenu;

		quitBtn.render.rect.transform.pos.x = GetWorldXByPercentage(60.0f);
		quitBtn.render.rect.transform.pos.y = GetWorldYByPercentage(37.0f);

		quitBtn.render.rect.transform.size.x = 170.0f;
		quitBtn.render.rect.transform.size.y = 100.0f;
		quitBtn.render.rect.transform.cachedSize = quitBtn.render.rect.transform.size;

		buttons.push_back(quitBtn);
	}

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

	void HandleBtnClick() {
		if (!GAME_OVER) return;

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

	void Restart() {
		SceneManager::LoadScene(SceneManager::RESTART);
	}
	void LoadMainMenu() {
		SceneManager::LoadScene(SceneManager::MAIN_MENU);
	}
}