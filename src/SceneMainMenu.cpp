/*!************************************************************************
\file:          SceneMainMenu.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <AEEngine.h>
#include <InputManager.h>
#include <RenderSystem.h>
#include <MomoMaths.h>
#include <SceneManager.h>
#include <SceneMainMenu.h>

#include <TextureManager.h>

/*!***********************************************************************
* CONST VARIABLES
*************************************************************************/
const float POINTER_OFFSET = 80.0f;
const float TRANSITION_TIME = 1.0f;
const float BLINK_INTERVAL = 0.07f;

// Title animation.
float ANIMATION_FRAMES;
const float ANIMATION_INTERVAL = 4.0f;

/*!***********************************************************************
* FORWARD DECLARATIONS
*************************************************************************/
void LoadStart();
void LoadEditor();
void LoadControls();
void LoadCredits();
void LoadQuit();

void HandleBtnClick();

void DrawButtons();
void DrawPointer();
void DrawQuitPrompt();

void InitializeButtons();
bool MouseInsideButton(Vec2<int> mousePos, Vec2<float> btnPos, Vec2<float> btnSize);

void ToggleQuitConfirm();

float GetWorldXByPercentage(float percent);
float GetWorldYByPercentage(float percent);

/*!***********************************************************************
* MENU BUTTONS
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
* SCENE TRANSITION
*************************************************************************/
RenderSystem::Interactable clickedBtn{};	// Button player clicked on. To get position and callback func.

bool isTransitioning = false;
bool isBlinking = false;					// Use to toggle opacity of pointer to mimic blinking.

float currBlinkInterval = 0;
float currTransitionTime = 0;

/*!***********************************************************************
* TITLE ANIMATION
*************************************************************************/
float currAnimFrame = 0;
float currAnimInterval = 0.0f;

/*!***********************************************************************
* QUIT CONFIRMATION PROMPT
*************************************************************************/
bool showQuitConfirm = false;

void SceneMainMenu::Load() {
	return;
}

void SceneMainMenu::Initialize() {
	InitializeButtons();
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleBtnClick);

	// Hardcoding to animate menu animation.
	// Num of frames * dt + total delay between frames.
	ANIMATION_FRAMES = 13.0f * (float)AEFrameRateControllerGetFrameTime() + 1.3f;

	currBlinkInterval = BLINK_INTERVAL;
	currTransitionTime = TRANSITION_TIME;
	isTransitioning = false;
	isBlinking = false;
}

void SceneMainMenu::Update() {
	// Check for button presses.
	InputManager::HandleInput();

	// Play menu animation.
	if (currAnimFrame < ANIMATION_FRAMES) {
		// Tick timer and update texture.
		currAnimFrame += (float)AEFrameRateControllerGetFrameTime();
		TextureManager::Update();
	}
	else {
		// Handle delay between animation loop.
		if (currAnimInterval < ANIMATION_INTERVAL) {
			// Tick timer.
			currAnimInterval += (float)AEFrameRateControllerGetFrameTime();
		}
		else {
			// Reset animation loop.
			currAnimFrame = 0;
			currAnimInterval = 0;
		}
	}
}

void SceneMainMenu::Draw() {
	// Draw background to fit to screen.
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, AEGfxGetWinMinX(), -AEGfxGetWinMinY(), (float)AEGetWindowWidth(), (float)AEGetWindowHeight(), TextureManager::MENU_BG, -1);

	DrawButtons();
	DrawPointer();
	DrawQuitPrompt();

	RenderSystem::Render();
}

void SceneMainMenu::Free() {
	buttons.clear();
	InputManager::Free();
	RenderSystem::Free();
}

void SceneMainMenu::Unload() {
	return;
}

void DrawButtons() {
	// Loop through all buttons and draw them.
	for (RenderSystem::Interactable& btn : buttons) {
		// Skip if button is not visible.
		if (!btn.isActive) continue;

		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x, btn.render.rect.transform.pos.y, btn.render.rect.transform.size.x, btn.render.rect.transform.size.y, btn.render.rect.graphics.tex, btn.render.layer);
	}
}

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

void DrawPointer() {
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
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, clickedBtn.render.rect.transform.pos.x - POINTER_OFFSET, clickedBtn.render.rect.transform.pos.y, 60, 90, TextureManager::POINTER);
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

		// Dont draw pointer for quit button.
		if (btn.render.rect.graphics.tex == TextureManager::QUIT_BTN) continue;

		// Check if mouse is hovering button.
		if (MouseInsideButton(mousePos, btn.render.rect.transform.pos, btn.render.rect.transform.size)) {
			// Draw pointer.
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x - POINTER_OFFSET, btn.render.rect.transform.pos.y, 60, 90, TextureManager::POINTER, 3);
			break;
		}
	}
}

void DrawQuitPrompt() {
	// If player wants to quit, draw quit prompt.
	if (showQuitConfirm) {
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, confirmQuitPrompt.rect.transform.pos.x, confirmQuitPrompt.rect.transform.pos.y, confirmQuitPrompt.rect.transform.size.x, confirmQuitPrompt.rect.transform.size.y, TextureManager::CONFIRM_PROMPT);
	}
}

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

void InitializeButtons() {
	// Position button based on window width / height.
	// Width and height of button are based on png size.

	// START GAME BUTTON
	startBtn.render.rect.graphics.tex = TextureManager::STARTGAME_BTN;
	startBtn.func = LoadStart;

	startBtn.render.rect.transform.pos.x = GetWorldXByPercentage(21.9f);
	startBtn.render.rect.transform.pos.y = GetWorldYByPercentage(39.0f);

	startBtn.render.rect.transform.size.x = 460.0f;
	startBtn.render.rect.transform.size.y = 100.0f;

	buttons.push_back(startBtn);

	// EDITOR BUTTON
	editorBtn.render.rect.graphics.tex = TextureManager::EDITOR_BTN;
	editorBtn.func = LoadEditor;

	editorBtn.render.rect.transform.pos.x = GetWorldXByPercentage(61.9f);
	editorBtn.render.rect.transform.pos.y = GetWorldYByPercentage(39.0f);

	editorBtn.render.rect.transform.size.x = 260.0f;
	editorBtn.render.rect.transform.size.y = 100.0f;
	buttons.push_back(editorBtn);

	// CONTROLS BUTTON
	controlsBtn.render.rect.graphics.tex = TextureManager::CONTROLS_BTN;
	controlsBtn.func = LoadControls;

	controlsBtn.render.rect.transform.pos.x = GetWorldXByPercentage(55.7f);
	controlsBtn.render.rect.transform.pos.y = GetWorldYByPercentage(25.5f);

	controlsBtn.render.rect.transform.size.x = 360.0f;
	controlsBtn.render.rect.transform.size.y = 100.0f;
	buttons.push_back(controlsBtn);

	// CREDITS BUTTON
	creditsBtn.render.rect.graphics.tex = TextureManager::CREDITS_BTN;
	creditsBtn.func = LoadCredits;

	creditsBtn.render.rect.transform.pos.x = GetWorldXByPercentage(21.9f);
	creditsBtn.render.rect.transform.pos.y = GetWorldYByPercentage(25.5f);

	creditsBtn.render.rect.transform.size.x = 300.0f;
	creditsBtn.render.rect.transform.size.y = 100.0f;
	buttons.push_back(creditsBtn);

	// QUIT BUTTON
	quitBtn.render.rect.graphics.tex = TextureManager::QUIT_BTN;
	quitBtn.func = ToggleQuitConfirm;

	quitBtn.render.rect.transform.pos.x = GetWorldXByPercentage(88.0f);
	quitBtn.render.rect.transform.pos.y = GetWorldYByPercentage(13.2f);

	quitBtn.render.rect.transform.size.x = 170.0f;
	quitBtn.render.rect.transform.size.y = 110.0f;
	buttons.push_back(quitBtn);

	// QUIT YES BUTTON
	quitYesBtn.render.rect.graphics.tex = TextureManager::YES_BTN;
	quitYesBtn.func = LoadQuit;
	quitYesBtn.isActive = false;

	quitYesBtn.render.rect.transform.pos.x = GetWorldXByPercentage(35.0f);
	quitYesBtn.render.rect.transform.pos.y = GetWorldYByPercentage(50.0f);

	quitYesBtn.render.rect.transform.size.x = 130.0f;
	quitYesBtn.render.rect.transform.size.y = 100.0f;
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
	quitNoBtn.render.layer = 2;
	buttons.push_back(quitNoBtn);

	// CONFIRMATION PROMPT
	confirmQuitPrompt.rect.transform.pos.x = GetWorldXByPercentage(23.1f);
	confirmQuitPrompt.rect.transform.pos.y = GetWorldYByPercentage(72.3f);

	confirmQuitPrompt.rect.transform.size.x = 850.0f;
	confirmQuitPrompt.rect.transform.size.y = 390.0f;
	quitYesBtn.render.layer = 1;
}

void LoadStart() {
	SceneManager::LoadScene(SceneManager::GAME_LEVEL);
}

void LoadEditor() {
	SceneManager::LoadScene(SceneManager::EDITOR);
}

void LoadControls() {
	SceneManager::LoadScene(SceneManager::CONTROLS);
}

void LoadCredits() {
	SceneManager::LoadScene(SceneManager::CREDITS);
}

void LoadQuit() {
	SceneManager::LoadScene(SceneManager::QUIT);
}
