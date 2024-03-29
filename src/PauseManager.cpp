/*!************************************************************************
\file			PauseManager.cpp
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			22-03-2023
\brief
This source file handles the pause state of the game.

All content � 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <PauseManager.h>
#include <FileIOManager.h>
#include <SceneGameLevel.h>
#include <CardManager.h>
#include <ScoreManager.h>
///////////////////////////////////////////////////////////////////////////

namespace PauseManager {
	/*!***********************************************************************
	* Const variables.
	*************************************************************************/
	const Vec2<float> POINTER_OFFSET = { 80.0f, 10.0f };
	
	/*!***********************************************************************
	* UI.
	*************************************************************************/
	RenderSystem::Interactable pauseBtn{};
	RenderSystem::Interactable restartBtn{};
	RenderSystem::Interactable continueBtn{};
	RenderSystem::Interactable exitBtn{};
	RenderSystem::Interactable quitYesBtn{};
	RenderSystem::Interactable quitNoBtn{};

	RenderSystem::Renderable pausePrompt{};
	RenderSystem::Renderable confirmQuitPrompt{};

	std::vector<RenderSystem::Interactable> buttons;

	/*!***********************************************************************
	* Variables.
	*************************************************************************/
	bool isPaused;			// Is the game currently paused?
	bool showQuitConfirm;	// Is the confirm quit prompt shown?
	bool showRestartConfirm;// Is the confirm restart prompt shown?

	/*!***********************************************************************
	* Forward Declaration.
	*************************************************************************/
	void InitializePauseUI();
	void DrawPauseUI();
	void TogglePause();
	void ToggleQuitConfirm();
	void ToggleRestartConfirm();
	void RestartLevel();
	void HandlePauseUIClick();
    void LoadMainMenu();
    void HandleBtnHover();
	bool IsPaused() { return isPaused || showRestartConfirm; }
	
	/*!***********************************************************************
	\brief
		Initialize PauseManager.
	*************************************************************************/
	void Initialize()
	{
		// Reset.
		isPaused = false;
		showQuitConfirm = false;
		showRestartConfirm = false;

		// Initialize UI.
		InitializePauseUI();

		// Subscribe to key events.
		InputManager::SubscribeToKey(AEVK_ESCAPE, InputManager::TRIGGERED, TogglePause);
		InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandlePauseUIClick);
	}

	/*!***********************************************************************
	\brief
		Initialize UI.
	*************************************************************************/
	void InitializePauseUI() {
		// PAUSE BUTTON.
		pauseBtn.render.rect.graphics.tex = TextureManager::PAUSE_BUTTON;
		pauseBtn.func = TogglePause;

		pauseBtn.render.rect.transform.pos.x = GetWorldXByPercentage(93.0f);
		pauseBtn.render.rect.transform.pos.y = GetWorldYByPercentage(97.0f);

		pauseBtn.render.rect.transform.size.x = 72.0f;
		pauseBtn.render.rect.transform.size.y = 80.0f;
		pauseBtn.render.rect.transform.cachedSize = pauseBtn.render.rect.transform.size;

		buttons.push_back(pauseBtn);

		//RESTART BUTTON.
		restartBtn.render.rect.graphics.tex = TextureManager::RESTART_BUTTON;
		restartBtn.func = ToggleRestartConfirm;

		restartBtn.render.rect.transform.pos.x = GetWorldXByPercentage(86.0f);
		restartBtn.render.rect.transform.pos.y = GetWorldYByPercentage(97.0f);

		restartBtn.render.rect.transform.size.x = 72.0f;
		restartBtn.render.rect.transform.size.y = 80.0f;
		restartBtn.render.rect.transform.cachedSize = restartBtn.render.rect.transform.size;

		buttons.push_back(restartBtn);

		// PAUSE PROMPT.
		pausePrompt.rect.graphics.tex = TextureManager::PAUSE_WINDOW;

		pausePrompt.rect.transform.pos.x = GetWorldXByPercentage(25.0f);
		pausePrompt.rect.transform.pos.y = GetWorldYByPercentage(80.0f);

		pausePrompt.rect.transform.size.x = 850.0f;
		pausePrompt.rect.transform.size.y = 390.0f;
		pausePrompt.layer = 6;

		// QUIT CONFIRMATION PROMPT
		confirmQuitPrompt.rect.graphics.tex = TextureManager::CONFIRM_PROMPT;

		confirmQuitPrompt.rect.transform.pos.x = GetWorldXByPercentage(25.0f);
		confirmQuitPrompt.rect.transform.pos.y = GetWorldYByPercentage(80.0f);

		confirmQuitPrompt.rect.transform.size.x = 850.0f;
		confirmQuitPrompt.rect.transform.size.y = 390.0f;
		confirmQuitPrompt.layer = 8;

		// QUIT YES BUTTON
		quitYesBtn.render.rect.graphics.tex = TextureManager::YES_BTN;
		quitYesBtn.func = LoadMainMenu;
		quitYesBtn.isActive = false;

		quitYesBtn.render.rect.transform.pos.x = GetWorldXByPercentage(37.0f);
		quitYesBtn.render.rect.transform.pos.y = GetWorldYByPercentage(57.0f);

		quitYesBtn.render.rect.transform.size.x = 130.0f;
		quitYesBtn.render.rect.transform.size.y = 100.0f;
		quitYesBtn.render.rect.transform.cachedSize = quitYesBtn.render.rect.transform.size;

		quitYesBtn.render.layer = 9;
		buttons.push_back(quitYesBtn);

		// QUIT NO BUTTON
		quitNoBtn.render.rect.graphics.tex = TextureManager::NO_BTN;
		quitNoBtn.func = ToggleQuitConfirm;
		quitNoBtn.isActive = false;

		quitNoBtn.render.rect.transform.pos.x = GetWorldXByPercentage(58.7f);
		quitNoBtn.render.rect.transform.pos.y = GetWorldYByPercentage(57.0f);

		quitNoBtn.render.rect.transform.size.x = 100.0f;
		quitNoBtn.render.rect.transform.size.y = 100.0f;
		quitNoBtn.render.rect.transform.cachedSize = quitNoBtn.render.rect.transform.size;

		quitNoBtn.render.layer = 9;
		buttons.push_back(quitNoBtn);

		// CONTINUE BUTTON.
		continueBtn.render.rect.graphics.tex = TextureManager::RESUME_BTN;
		continueBtn.func = TogglePause;
		continueBtn.isActive = false;

		continueBtn.render.rect.transform.pos.x = GetWorldXByPercentage(33.0f);
		continueBtn.render.rect.transform.pos.y = GetWorldYByPercentage(57.0f);

		continueBtn.render.rect.transform.size.x = 270.0f;
		continueBtn.render.rect.transform.size.y = 100.0f;
		continueBtn.render.rect.transform.cachedSize = continueBtn.render.rect.transform.size;

		continueBtn.render.layer = 7;
		buttons.push_back(continueBtn);

		// QUIT TO MENU BUTTON.
		exitBtn.render.rect.graphics.tex = TextureManager::QUIT_BTN;
		exitBtn.func = ToggleQuitConfirm;
		exitBtn.isActive = false;

		exitBtn.render.rect.transform.pos.x = GetWorldXByPercentage(60.0f);
		exitBtn.render.rect.transform.pos.y = GetWorldYByPercentage(57.0f);

		exitBtn.render.rect.transform.size.x = 170.0f;
		exitBtn.render.rect.transform.size.y = 100.0f;
		exitBtn.render.rect.transform.cachedSize = exitBtn.render.rect.transform.size;

		exitBtn.render.layer = 7;
		buttons.push_back(exitBtn);
	}

	/*!***********************************************************************
	\brief
		Update PauseManager.
	*************************************************************************/
	void PauseManager::Update() {
		return;
	}

	/*!***********************************************************************
	\brief
		Draw the PauseManager.
	*************************************************************************/
	void PauseManager::Draw() {
		// Draw visual feedback.
		HandleBtnHover();
		// Draw UI.
		DrawPauseUI();
	}

	/*!***********************************************************************
	\brief
		Handle visual feedback of buttons.
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
				RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x - POINTER_OFFSET.x, btn.render.rect.transform.pos.y - POINTER_OFFSET.y, 60, 90, TextureManager::POINTER, 9);
			}
			else {
				// Scale btn to original size.
				btn.render.rect.transform.size = btn.render.rect.transform.cachedSize;
			}
		}
	}

	/*!***********************************************************************
	\brief
		Draw UI.
	*************************************************************************/
	void DrawPauseUI() {
		// Draw pause prompt.
		if (IsPaused()) {
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, pausePrompt.rect.transform.pos.x, pausePrompt.rect.transform.pos.y, pausePrompt.rect.transform.size.x, pausePrompt.rect.transform.size.y, pausePrompt.rect.graphics.tex, pausePrompt.layer);
		}

		// Draw confirm quit prompt.
		if (showQuitConfirm) {
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, confirmQuitPrompt.rect.transform.pos.x, confirmQuitPrompt.rect.transform.pos.y, confirmQuitPrompt.rect.transform.size.x, confirmQuitPrompt.rect.transform.size.y, confirmQuitPrompt.rect.graphics.tex, confirmQuitPrompt.layer);
		}

		// Draw confirm quit prompt.
		if (showRestartConfirm) {
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, confirmQuitPrompt.rect.transform.pos.x, confirmQuitPrompt.rect.transform.pos.y, confirmQuitPrompt.rect.transform.size.x, confirmQuitPrompt.rect.transform.size.y, confirmQuitPrompt.rect.graphics.tex, confirmQuitPrompt.layer);
		}

		// Draw buttons.
		for (RenderSystem::Interactable const& i : buttons) {
			if (i.isActive) {
				RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, i.render.rect.transform.pos.x, i.render.rect.transform.pos.y, i.render.rect.transform.size.x, i.render.rect.transform.size.y, i.render.rect.graphics.tex, i.render.layer);
			}
		}
	}

	/*!***********************************************************************
	\brief
		Toggle restart button based on given bool.
	\param state
		True / false.
	*************************************************************************/
	void ToggleShowRestart(bool state){
		for (RenderSystem::Interactable& i : buttons) {
			if(i.render.rect.graphics.tex == TextureManager::RESTART_BUTTON){
				i.isActive = state;
				i.isClickable = state;
			}
		}
	}
	/*!***********************************************************************
	\brief
		Toggle pause button based on given bool.
	\param state
		True / false.
	*************************************************************************/
	void ToggleShowPause(bool state){
		for (RenderSystem::Interactable& i : buttons) {
			if(i.render.rect.graphics.tex == TextureManager::PAUSE_BUTTON){
				i.isActive = state;
				i.isClickable = state;
			}
		}
	}

	/*!***********************************************************************
	\brief
		Load main menu scene.
	*************************************************************************/
	void LoadMainMenu() {
		// Close prompts and reset active / clickability.
		ToggleQuitConfirm();
		//ToggleRestartConfirm();
		TogglePause();
		//We also save the map data and hand to file 

		// Load menu scene.
		SceneManager::LoadScene(SceneManager::MAIN_MENU);
	}

	/*!***********************************************************************
	\brief
		Restart game level.
	*************************************************************************/
	void RestartLevel() {
		ScoreManager::SetLevel(0);
		ScoreManager::SetScore(0);
		GridManager::ResetGrid();
		CardManager::Free();
		CardManager::Initialize();
		SceneManager::LoadScene(SceneManager::RESTART);
	}

	/*!***********************************************************************
	\brief
		Toggle pause UI.
	*************************************************************************/
	void TogglePause() {
		isPaused = !isPaused;
		CardManager::ToggleClickable(!isPaused);
		// Buttons.
		for (RenderSystem::Interactable& i : buttons) {
			// Toggle the visibility and clickability of back and quit btn.
			if (i.render.rect.graphics.tex == TextureManager::RESUME_BTN || i.render.rect.graphics.tex == TextureManager::QUIT_BTN) {
				i.isActive = isPaused;
				i.isClickable = isPaused;
			}
		}
	}

	/*!***********************************************************************
	\brief
		Toggle quit confirm UI.
	*************************************************************************/
	void ToggleQuitConfirm() {
		// Toggle between quit confirm state.
		showQuitConfirm = !showQuitConfirm;
		for (RenderSystem::Interactable& btn : buttons) {
			if (btn.render.rect.graphics.tex == TextureManager::YES_BTN) {
				btn.func = LoadMainMenu;
			}
			if (btn.render.rect.graphics.tex == TextureManager::NO_BTN) {
				btn.func = ToggleQuitConfirm;
			}
			// Toggle the visibility and clickability of yes and no buttons.
			if (btn.render.rect.graphics.tex == TextureManager::YES_BTN || btn.render.rect.graphics.tex == TextureManager::NO_BTN) {
				btn.isActive = showQuitConfirm;
				btn.isClickable = showQuitConfirm;
			}
			else {
				// Toggle the clickability of other buttons.
				btn.isClickable = !showQuitConfirm;
			}
		}
	}

	/*!***********************************************************************
	\brief
		Toggle restart confirm UI.
	*************************************************************************/
	void ToggleRestartConfirm() {
		// Toggle between quit confirm state.
		showRestartConfirm = !showRestartConfirm;
		CardManager::ToggleClickable(!showRestartConfirm);

		for (RenderSystem::Interactable& btn : buttons) {
			// Toggle the visibility and clickability of yes and no buttons.
			if (btn.render.rect.graphics.tex == TextureManager::YES_BTN || btn.render.rect.graphics.tex == TextureManager::NO_BTN) {
				btn.isActive = showRestartConfirm;
				btn.isClickable = showRestartConfirm;
				if (btn.render.rect.graphics.tex == TextureManager::YES_BTN) {
					btn.func = RestartLevel;
				}
				if (btn.render.rect.graphics.tex == TextureManager::NO_BTN) {
					btn.func = ToggleRestartConfirm;
				}
			}
			else {
				// Toggle the clickability of other buttons.
				btn.isClickable = !showRestartConfirm;
			}
		}

	}

	/*!***********************************************************************
	\brief
		Free PauseManager.
	*************************************************************************/
	void Free() {
		buttons.clear();
		// Unsubscribe from keys.
		InputManager::UnsubscribeKey(AEVK_ESCAPE, InputManager::TRIGGERED, TogglePause);
		InputManager::UnsubscribeKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandlePauseUIClick);
	}

	/*!***********************************************************************
	\brief
		Handle player click on buttons.
	*************************************************************************/
	void HandlePauseUIClick() {
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
}
