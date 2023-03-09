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

#include <SceneManager.h>
#include <SceneMainMenu.h>

#include <TextureManager.h>

/*!***********************************************************************
* CONST VARIABLES
*************************************************************************/
const float POINTER_OFFSET = 80.0f;
const float TRANSITION_TIME = 1.0f;
const float BLINK_INTERVAL = 0.07f;

// Menu animation.
float ANIMATION_FRAMES;
const float ANIMATION_INTERVAL = 4.0f;

/*!***********************************************************************
* FORWARD DECLARATIONS
*************************************************************************/
void LoadStart();
void LoadEditor();
void LoadOptions();
void LoadControls();
void LoadCredits();
void LoadQuit();

void HandleBtnClick();

void DrawButtons();
void DrawPointer();

void InitializeButtons();
bool MouseInsideButton(Vec2<int> mousePos, Vec2<float> btnPos, Vec2<float> btnSize);

/*!***********************************************************************
* MENU BUTTONS
*************************************************************************/
RenderSystem::Interactable startBtn{};
RenderSystem::Interactable editorBtn{};
RenderSystem::Interactable optionsBtn{};
RenderSystem::Interactable controlsBtn{};
RenderSystem::Interactable creditsBtn{};
RenderSystem::Interactable quitBtn{};

std::vector<RenderSystem::Interactable> buttons;

/*!***********************************************************************
* SCENE TRANSITION
*************************************************************************/
RenderSystem::Interactable clickedBtn{};	// Button player clicked on. To get position and callback func.

bool isTransitioning = false;
bool isBlinking = false;					// Use to toggle opacity of pointer to mimic blinking.

float currBlinkInterval = BLINK_INTERVAL;
float currTransitionTime = TRANSITION_TIME;

// Menu animation.
float currAnimFrame = 0;
float currAnimInterval = 0.0f;

void SceneMainMenu::Load() {
	return;
}

void SceneMainMenu::Initialize() {
	InitializeButtons();
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleBtnClick);

	// Hardcoding to animate menu animation.
	// Num of frames * dt + total delay between frames.
	ANIMATION_FRAMES = 13.0f * (float)AEFrameRateControllerGetFrameTime() + 1.3f;
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

	RenderSystem::Render();
}

void SceneMainMenu::Free() {
	InputManager::Free();
}

void SceneMainMenu::Unload() {
	return;
}

void DrawButtons() {
	// Loop through all buttons and draw them.
	for (RenderSystem::Interactable& btn : buttons) {
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x, btn.render.rect.transform.pos.y, btn.render.rect.transform.size.x, btn.render.rect.transform.size.y, btn.render.rect.graphics.tex);
	}
}

void HandleBtnClick() {
	if (isTransitioning) return;

	// Cache mouse position.
	Vec2<int> mousePos = InputManager::GetMousePos();

	// Convert to world space position.
	mousePos.x -= (int)AEGfxGetWinMaxX();
	mousePos.y -= (int)AEGfxGetWinMaxY();

	// Loop through all buttons.
	for (RenderSystem::Interactable& btn : buttons) {
		// Check if mouse is hovering button.
		if (MouseInsideButton(mousePos, btn.render.rect.transform.pos, btn.render.rect.transform.size)) {
			// Cache button.
			clickedBtn = btn;
			// Start transitioning.
			isTransitioning = true;
			break;
		}
	}
}

void DrawPointer() {
	// Handle pointer blinking when transitioning to a different scene.
	if (isTransitioning) {
		// Immediate transition for credits and quit button.
		if (clickedBtn.render.rect.graphics.tex == TextureManager::CREDITS_BTN || clickedBtn.render.rect.graphics.tex == TextureManager::QUIT_BTN) clickedBtn.func();

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
		// Dont draw pointer for credits and quit buttons.
		if (btn.render.rect.graphics.tex == TextureManager::CREDITS_BTN || btn.render.rect.graphics.tex == TextureManager::QUIT_BTN) continue;

		// Check if mouse is hovering button.
		if (MouseInsideButton(mousePos, btn.render.rect.transform.pos, btn.render.rect.transform.size)) {
			// Draw pointer.
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x - POINTER_OFFSET, btn.render.rect.transform.pos.y, 60, 90, TextureManager::POINTER);
			break;
		}
	}
}

void InitializeButtons() {
	// Position button based on window width / height.
	// Width and height of button are based on png size.

	// START GAME BUTTON
	startBtn.render.rect.graphics.tex = TextureManager::STARTGAME_BTN;
	startBtn.func = LoadStart;

	startBtn.render.rect.transform.pos.x = AEGfxGetWinMinX() * 0.525f;
	startBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.222f;

	startBtn.render.rect.transform.size.x = 460.0f;
	startBtn.render.rect.transform.size.y = 100.0f;

	buttons.push_back(startBtn);

	// EDITOR BUTTON
	editorBtn.render.rect.graphics.tex = TextureManager::EDITOR_BTN;
	editorBtn.func = LoadEditor;

	editorBtn.render.rect.transform.pos.x = AEGfxGetWinMaxX() * 0.237f;
	editorBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.222f;

	editorBtn.render.rect.transform.size.x = 260.0f;
	editorBtn.render.rect.transform.size.y = 100.0f;
	buttons.push_back(editorBtn);

	// OPTION BUTTON
	optionsBtn.render.rect.graphics.tex = TextureManager::OPTIONS_BTN;
	optionsBtn.func = LoadOptions;

	optionsBtn.render.rect.transform.pos.x = AEGfxGetWinMinX() * 0.525f;
	optionsBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.488f;

	optionsBtn.render.rect.transform.size.x = 300.0f;
	optionsBtn.render.rect.transform.size.y = 100.0f;
	buttons.push_back(optionsBtn);

	// CONTROLS BUTTON
	controlsBtn.render.rect.graphics.tex = TextureManager::CONTROLS_BTN;
	controlsBtn.func = LoadControls;

	controlsBtn.render.rect.transform.pos.x = AEGfxGetWinMaxX() * 0.112f;
	controlsBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.488f;

	controlsBtn.render.rect.transform.size.x = 360.0f;
	controlsBtn.render.rect.transform.size.y = 100.0f;
	buttons.push_back(controlsBtn);

	// CREDITS BUTTON
	creditsBtn.render.rect.graphics.tex = TextureManager::CREDITS_BTN;
	creditsBtn.func = LoadCredits;

	creditsBtn.render.rect.transform.pos.x = AEGfxGetWinMinX() * 0.975f;
	creditsBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.733f;

	creditsBtn.render.rect.transform.size.x = 300.0f;
	creditsBtn.render.rect.transform.size.y = 100.0f;
	buttons.push_back(creditsBtn);

	// QUIT BUTTON
	quitBtn.render.rect.graphics.tex = TextureManager::QUIT_BTN;
	quitBtn.func = LoadQuit;

	quitBtn.render.rect.transform.pos.x = AEGfxGetWinMaxX() * 0.762f;
	quitBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.733f;

	quitBtn.render.rect.transform.size.x = 170.0f;
	quitBtn.render.rect.transform.size.y = 110.0f;
	buttons.push_back(quitBtn);
}

void LoadStart() {
	SceneManager::LoadScene(SceneManager::GAME_LEVEL);
}

void LoadEditor() {
	SceneManager::LoadScene(SceneManager::EDITOR);
}

void LoadOptions() {
	SceneManager::LoadScene(SceneManager::OPTIONS);
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

bool MouseInsideButton(Vec2<int> mousePos, Vec2<float> btnPos, Vec2<float> btnSize) {
	// LEFT / RIGHT BOUNDS
	if ((mousePos.x > btnPos.x &&
		mousePos.x < btnPos.x + btnSize.x) &&

		// TOP / BOTTOM BOUNDS
		(mousePos.y > -btnPos.y &&
			mousePos.y < -btnPos.y + btnSize.y)) {

		return true;
	}
	return false;
}
