/*!************************************************************************
\file			SceneMainMenu.cpp
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			31-03-2023
\brief
This source file handles Main Main scene of the game.

All content � 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <AEEngine.h>

#include <SceneManager.h>
#include <InputManager.h>
#include <AudioManager.h>
#include <TextureManager.h>

#include <MomoMaths.h>
#include <RenderSystem.h>
#include <SceneMainMenu.h>
#include <BuildingManager.h>
///////////////////////////////////////////////////////////////////////////

/*!***********************************************************************
* Const Variables
*************************************************************************/
const Vec2<float> POINTER_OFFSET = { 80.0f, 10.0f };
const float TRANSITION_TIME = 1.0f;
const float BLINK_INTERVAL = 0.07f;

/*!***********************************************************************
* Forward Declarations.
*************************************************************************/
void LoadStart();
void LoadEditor();
void LoadControls();
void LoadCredits();
void LoadQuit();

void HandleBtnClick();
void HandleBtnHover();

void DrawButtons();
void DrawQuitPrompt();

void InitializeMenuUI();
bool MouseInsideButton(Vec2<int> mousePos, Vec2<float> btnPos, Vec2<float> btnSize);

void ToggleQuitConfirm();

/*!***********************************************************************
* UI.
*************************************************************************/
RenderSystem::Interactable startBtn{};
RenderSystem::Interactable editorBtn{};
RenderSystem::Interactable controlsBtn{};
RenderSystem::Interactable creditsBtn{};
RenderSystem::Interactable quitBtn{};

RenderSystem::Interactable quitYesBtn{};
RenderSystem::Interactable quitNoBtn{};

std::vector<RenderSystem::Interactable> buttons;

RenderSystem::Renderable confirmQuitPrompt{};

/*!***********************************************************************
* Scene Transition Variables.
*************************************************************************/
RenderSystem::Interactable clickedBtn{};	// Button player clicked on. To get position and callback func.

bool isTransitioning = false;
bool isBlinking = false;					// Use to toggle opacity of pointer to mimic blinking.

float currBlinkInterval = 0;
float currTransitionTime = 0;

/*!***********************************************************************
* Variable.
*************************************************************************/
bool showQuitConfirm = false;

/*!***********************************************************************
\brief
	Load SceneMainMenu.
*************************************************************************/
void SceneMainMenu::Load() {
	BuildingManager::Initialize();
	return;
}

/*!***********************************************************************
\brief
	Initialize SceneMainMenu.
*************************************************************************/
void SceneMainMenu::Initialize() {
	// Initialize UI.
	InitializeMenuUI();

	// Subscribe to key events.
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleBtnClick);

	// Reset.
	currBlinkInterval = BLINK_INTERVAL;
	currTransitionTime = TRANSITION_TIME;

	isTransitioning = false;
	isBlinking = false;

	// Play main menu BGM.
	AudioManager::PlayBGM(AudioManager::ClipName::BGM_MAIN);
}

/*!***********************************************************************
\brief
	Update SceneMainMenu.
*************************************************************************/
void SceneMainMenu::Update() {
	// Check for button presses.
	InputManager::HandleInput();

	// Update managers.
	AudioManager::Update();
	TextureManager::Update();
}

/*!***********************************************************************
\brief
	Draw SceneMainMenu.
*************************************************************************/
void SceneMainMenu::Draw() {
	// Draw sky to fit to screen.
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, AEGfxGetWinMinX(), -AEGfxGetWinMinY(), (float)AEGetWindowWidth(), (float)AEGetWindowHeight(), TextureManager::SKY_BG, -3);

	// Draw clouds to fit to screen.
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, AEGfxGetWinMinX(), -AEGfxGetWinMinY(), (float)AEGetWindowWidth(), (float)AEGetWindowHeight(), TextureManager::CLOUD_BG, -2);

	// Draw background to fit to screen.
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, AEGfxGetWinMinX(), -AEGfxGetWinMinY(), (float)AEGetWindowWidth(), (float)AEGetWindowHeight(), TextureManager::MENU_BG, -1);

	// Draw UI.
	DrawButtons();
	DrawQuitPrompt();

	// Visual feedback.
	HandleBtnHover();

	RenderSystem::Render();
}

/*!***********************************************************************
\brief
	Free SceneMainMenu.
*************************************************************************/
void SceneMainMenu::Free() {
	buttons.clear();
	InputManager::Free();
	RenderSystem::Free();
}

/*!***********************************************************************
\brief
	Unload SceneMainMenu.
*************************************************************************/
void SceneMainMenu::Unload() {
	return;
}

/*!***********************************************************************
\brief
	Draw buttons that are visible.
*************************************************************************/
void DrawButtons() {
	// Loop through all buttons and draw them.
	for (RenderSystem::Interactable& btn : buttons) {
		// Skip if button is not visible.
		if (!btn.isActive) continue;

		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x, btn.render.rect.transform.pos.y, btn.render.rect.transform.size.x, btn.render.rect.transform.size.y, btn.render.rect.graphics.tex, btn.render.layer);
	}
}

/*!***********************************************************************
\brief
	Handle player click on buttons.
*************************************************************************/
void HandleBtnClick() {
	// Prevent any click if transitioning.
	if (isTransitioning) return;

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
			// Cache button.
			clickedBtn = btn;

			switch (clickedBtn.render.rect.graphics.tex)
			{
			case TextureManager::QUIT_BTN:
			case TextureManager::YES_BTN:
			case TextureManager::NO_BTN:
				// Immediate calling of button function when clicking. No transition needed. (For the above buttons)
				clickedBtn.func();
				break;
			default:
				// Play blinking pointer transition.
				isTransitioning = true;
				break;
			}
			break;
		}
	}
}

/*!***********************************************************************
\brief
	Handle visual feedback of buttons.
*************************************************************************/
void HandleBtnHover() {
	// Handle pointer blinking when transitioning to a different scene.
	if (isTransitioning) {
		// Transition timer.
		if (currTransitionTime > 0) {
			// Tick timer to transition to next scene.
			currTransitionTime -= (float)AEFrameRateControllerGetFrameTime();
		}
		else {
			// Transition to next scene.
			clickedBtn.func();
		}

		// Blink timer.
		if (currBlinkInterval > 0) {
			// Tick timer to blink.
			currBlinkInterval -= (float)AEFrameRateControllerGetFrameTime();
		}
		else {
			// Toggle blink.
			isBlinking = !isBlinking;
			currBlinkInterval = BLINK_INTERVAL;
		}

		// Change opacity of render setting to mimic blinking.
		RenderSystem::SetRenderSetting(Vec4<float>{1, 1, 1, static_cast<float>(isBlinking)});
		// Draw pointer blinking.
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, clickedBtn.render.rect.transform.pos.x - POINTER_OFFSET.x, clickedBtn.render.rect.transform.pos.y - POINTER_OFFSET.y, 60, 90, TextureManager::POINTER);
		return;
	}

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

			// Draw pointer.
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x - POINTER_OFFSET.x, btn.render.rect.transform.pos.y - POINTER_OFFSET.y, 60, 90, TextureManager::POINTER, 3);
		}
		else {
			// Scale btn to original size.
			btn.render.rect.transform.size = btn.render.rect.transform.cachedSize;
		}
	}
}

/*!***********************************************************************
\brief
	Draw quit prompt UI.
*************************************************************************/
void DrawQuitPrompt() {
	// If player wants to quit, draw quit prompt.
	if (showQuitConfirm) {
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, confirmQuitPrompt.rect.transform.pos.x, confirmQuitPrompt.rect.transform.pos.y, confirmQuitPrompt.rect.transform.size.x, confirmQuitPrompt.rect.transform.size.y, confirmQuitPrompt.rect.graphics.tex);
	}
}

/*!***********************************************************************
\brief
	Toggle quit confirm state.
*************************************************************************/
void ToggleQuitConfirm() {
	// Toggle between quit confirm state.
	showQuitConfirm = !showQuitConfirm;

	for (RenderSystem::Interactable& btn : buttons) {
		// Toggle the visibility and clickability of yes and no buttons.
		if (btn.render.rect.graphics.tex == TextureManager::YES_BTN || btn.render.rect.graphics.tex == TextureManager::NO_BTN) {
			btn.isActive = showQuitConfirm;
			btn.isClickable = showQuitConfirm;
		}
		else {
			// Toggle the clickability of other buttons in the menu.
			btn.isClickable = !showQuitConfirm;
		}
	}
}

/*!***********************************************************************
\brief
	Initialize UI.
*************************************************************************/
void InitializeMenuUI() {
	// Position button based on window width / height.
	// Width and height of button are based on png size.

	// START GAME BUTTON
	startBtn.render.rect.graphics.tex = TextureManager::STARTGAME_BTN;
	startBtn.func = LoadStart;

	startBtn.render.rect.transform.pos.x = GetWorldXByPercentage(21.9f);
	startBtn.render.rect.transform.pos.y = GetWorldYByPercentage(39.0f);

	startBtn.render.rect.transform.size.x = 460.0f;
	startBtn.render.rect.transform.size.y = 100.0f;
	startBtn.render.rect.transform.cachedSize = startBtn.render.rect.transform.size;

	buttons.push_back(startBtn);

	// EDITOR BUTTON
	editorBtn.render.rect.graphics.tex = TextureManager::EDITOR_BTN;
	editorBtn.func = LoadEditor;

	editorBtn.render.rect.transform.pos.x = GetWorldXByPercentage(61.9f);
	editorBtn.render.rect.transform.pos.y = GetWorldYByPercentage(39.0f);

	editorBtn.render.rect.transform.size.x = 260.0f;
	editorBtn.render.rect.transform.size.y = 100.0f;
	editorBtn.render.rect.transform.cachedSize = editorBtn.render.rect.transform.size;

	buttons.push_back(editorBtn);

	// CONTROLS BUTTON
	controlsBtn.render.rect.graphics.tex = TextureManager::CONTROLS_BTN;
	controlsBtn.func = LoadControls;

	controlsBtn.render.rect.transform.pos.x = GetWorldXByPercentage(55.7f);
	controlsBtn.render.rect.transform.pos.y = GetWorldYByPercentage(25.5f);

	controlsBtn.render.rect.transform.size.x = 360.0f;
	controlsBtn.render.rect.transform.size.y = 100.0f;
	controlsBtn.render.rect.transform.cachedSize = controlsBtn.render.rect.transform.size;

	buttons.push_back(controlsBtn);

	// CREDITS BUTTON
	creditsBtn.render.rect.graphics.tex = TextureManager::CREDITS_BTN;
	creditsBtn.func = LoadCredits;

	creditsBtn.render.rect.transform.pos.x = GetWorldXByPercentage(21.9f);
	creditsBtn.render.rect.transform.pos.y = GetWorldYByPercentage(25.5f);

	creditsBtn.render.rect.transform.size.x = 300.0f;
	creditsBtn.render.rect.transform.size.y = 100.0f;
	creditsBtn.render.rect.transform.cachedSize = creditsBtn.render.rect.transform.size;

	buttons.push_back(creditsBtn);

	// QUIT BUTTON
	quitBtn.render.rect.graphics.tex = TextureManager::QUIT_BTN;
	quitBtn.func = ToggleQuitConfirm;

	quitBtn.render.rect.transform.pos.x = GetWorldXByPercentage(88.0f);
	quitBtn.render.rect.transform.pos.y = GetWorldYByPercentage(13.2f);

	quitBtn.render.rect.transform.size.x = 170.0f;
	quitBtn.render.rect.transform.size.y = 110.0f;
	quitBtn.render.rect.transform.cachedSize = quitBtn.render.rect.transform.size;

	buttons.push_back(quitBtn);

	// QUIT YES BUTTON
	quitYesBtn.render.rect.graphics.tex = TextureManager::YES_BTN;
	quitYesBtn.func = LoadQuit;
	quitYesBtn.isActive = false;

	quitYesBtn.render.rect.transform.pos.x = GetWorldXByPercentage(35.0f);
	quitYesBtn.render.rect.transform.pos.y = GetWorldYByPercentage(50.0f);

	quitYesBtn.render.rect.transform.size.x = 130.0f;
	quitYesBtn.render.rect.transform.size.y = 100.0f;
	quitYesBtn.render.rect.transform.cachedSize = quitYesBtn.render.rect.transform.size;

	quitYesBtn.render.layer = 2;
	buttons.push_back(quitYesBtn);

	// QUIT NO BUTTON
	quitNoBtn.render.rect.graphics.tex = TextureManager::NO_BTN;
	quitNoBtn.func = ToggleQuitConfirm;
	quitNoBtn.isActive = false;

	quitNoBtn.render.rect.transform.pos.x = GetWorldXByPercentage(56.7f);
	quitNoBtn.render.rect.transform.pos.y = GetWorldYByPercentage(50.0f);

	quitNoBtn.render.rect.transform.size.x = 100.0f;
	quitNoBtn.render.rect.transform.size.y = 100.0f;
	quitNoBtn.render.rect.transform.cachedSize = quitNoBtn.render.rect.transform.size;

	quitNoBtn.render.layer = 2;
	buttons.push_back(quitNoBtn);

	// CONFIRMATION PROMPT
	confirmQuitPrompt.rect.graphics.tex = TextureManager::CONFIRM_PROMPT;
	confirmQuitPrompt.rect.transform.pos.x = GetWorldXByPercentage(23.1f);
	confirmQuitPrompt.rect.transform.pos.y = GetWorldYByPercentage(72.3f);

	confirmQuitPrompt.rect.transform.size.x = 850.0f;
	confirmQuitPrompt.rect.transform.size.y = 390.0f;

	confirmQuitPrompt.layer = 1;
}

/*!***********************************************************************
\brief
	Load level select scene.
*************************************************************************/
void LoadStart() {
	SceneManager::LoadScene(SceneManager::LVL_SELECT);
}

/*!***********************************************************************
\brief
	Load editor scene.
*************************************************************************/
void LoadEditor() {
	SceneManager::LoadScene(SceneManager::EDITOR);
}

/*!***********************************************************************
\brief
	Load controls scene.
*************************************************************************/
void LoadControls() {
	SceneManager::LoadScene(SceneManager::CONTROLS);
}

/*!***********************************************************************
\brief
	Load credits scene.
*************************************************************************/
void LoadCredits() {
	SceneManager::LoadScene(SceneManager::CREDITS);
}

/*!***********************************************************************
\brief
	Quit game.
*************************************************************************/
void LoadQuit() {
	SceneManager::LoadScene(SceneManager::QUIT);
}
