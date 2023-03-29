/*!************************************************************************
\file:          SceneLevelSelect.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          29-03-2023
\brief


The functions include:
-
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <AEEngine.h>
#include <RenderSystem.h>

#include <AudioManager.h>
#include <SceneManager.h>
#include <InputManager.h>

#include <SceneLevelSelect.h>
#include <SceneGameLevel.h>
#include <TextureManager.h>
#include <ColorTable.h>
///////////////////////////////////////////////////////////////////////////
// Forward Declaration
void LvlSelectInitializeUI();
void DrawLvlSelectButtons();
void HandleLvlSelectBtnHover();
void HandleLvlSelectBtnClick();

void LvlSelectReturnToMenu(); // Button functions
void LvlSelectLoadMap1();
void LvlSelectLoadMap2();
void LvlSelectLoadMap3();
void LvlSelectLoadUserMap1();
void LvlSelectLoadUserMap2();

///////////////////////////////////////////////////////////////////////////
// UI variables
std::vector<RenderSystem::Interactable> lvlSelectButtons;
RenderSystem::Interactable lvlSelectClickedBtn{};	// Button player clicked on. To get position and callback func.

RenderSystem::Interactable lvlSelectBackBtn{};
RenderSystem::Interactable lvl1Btn{};
RenderSystem::Interactable lvl2Btn{};
RenderSystem::Interactable lvl3Btn{};
RenderSystem::Interactable lvlUser1Btn{};
RenderSystem::Interactable lvlUser2Btn{};

///////////////////////////////////////////////////////////////////////////
// Const variables
const Vec2<float> LVL_SELECT_POINTER_OFFSET = { 80.0f, 10.0f };
const float LVL_SELECT_TRANSITION_TIME = 1.0f;
const float LVL_SELECT_BLINK_INTERVAL = 0.07f;

///////////////////////////////////////////////////////////////////////////
// Button pointer variables
bool lvlSelectIsTransitioning = false;
bool lvlSelectIsBlinking = false;					// Use to toggle opacity of pointer to mimic blinking.

float lvlSelectCurrBlinkInterval = 0;
float lvlSelectCurrTransitionTime = 0;

///////////////////////////////////////////////////////////////////////////
// Scene Functions

void SceneLevelSelect::Load() {

}

void SceneLevelSelect::Initialize() {
	LvlSelectInitializeUI();
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleLvlSelectBtnClick);

	lvlSelectCurrBlinkInterval = LVL_SELECT_BLINK_INTERVAL;
	lvlSelectCurrTransitionTime = LVL_SELECT_TRANSITION_TIME;
	lvlSelectIsTransitioning = false;
	lvlSelectCurrBlinkInterval = false;

	AudioManager::PlayBGM(AudioManager::ClipName::BGM_MAIN);
}

void SceneLevelSelect::Update() {
	InputManager::HandleInput();
	AudioManager::Update();
}

void SceneLevelSelect::Draw() {
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, AEGfxGetWinMinX(), -AEGfxGetWinMinY(), (float)AEGetWindowWidth(), (float)AEGetWindowHeight(), TextureManager::CREDITS_BG, -1);
	
	DrawLvlSelectButtons();
	HandleLvlSelectBtnHover();

	RenderSystem::Render();
}

void SceneLevelSelect::Free() {
	InputManager::Free();
	RenderSystem::Free();
	lvlSelectButtons.clear();
}

void SceneLevelSelect::Unload() {

}

///////////////////////////////////////////////////////////////////////////
// UI Functions

void LvlSelectInitializeUI() {
	// BACK BUTTON
	lvlSelectBackBtn.render.rect.graphics.tex = TextureManager::BACK_BTN;
	lvlSelectBackBtn.func = LvlSelectReturnToMenu;

	lvlSelectBackBtn.render.rect.transform.pos.x = GetWorldXByPercentage(88.0f);
	lvlSelectBackBtn.render.rect.transform.pos.y = GetWorldYByPercentage(13.2f);

	lvlSelectBackBtn.render.rect.transform.size.x = 150.0f;
	lvlSelectBackBtn.render.rect.transform.size.y = 78.0f;
	lvlSelectBackBtn.render.rect.transform.cachedSize = lvlSelectBackBtn.render.rect.transform.size;
	lvlSelectButtons.push_back(lvlSelectBackBtn);

	// MAP 0 BUTTON

}

void DrawLvlSelectButtons() {
	// Loop through all buttons and draw them.
	for (RenderSystem::Interactable& btn : lvlSelectButtons) {
		// Skip if button is not visible.
		if (!btn.isActive) continue;

		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x, btn.render.rect.transform.pos.y, btn.render.rect.transform.size.x, btn.render.rect.transform.size.y, btn.render.rect.graphics.tex, btn.render.layer);
	}
}

void HandleLvlSelectBtnHover() {
	// Handle pointer blinking when transitioning to a different scene.
	if (lvlSelectIsTransitioning) {
		// Transition timer.
		if (lvlSelectCurrTransitionTime > 0) {
			// Tick timer to transition to next scene.
			lvlSelectCurrTransitionTime -= (float)AEFrameRateControllerGetFrameTime();
		}
		else {
			// Transition to next scene.
			lvlSelectClickedBtn.func();
		}

		// Blink timer.
		if (lvlSelectCurrBlinkInterval > 0) {
			// Tick timer to blink.
			lvlSelectCurrBlinkInterval -= (float)AEFrameRateControllerGetFrameTime();
		}
		else {
			// Toggle blink.
			lvlSelectIsBlinking = !lvlSelectIsBlinking;
			lvlSelectCurrBlinkInterval = LVL_SELECT_BLINK_INTERVAL;
		}

		// Change opacity of render setting to mimic blinking.
		RenderSystem::SetRenderSetting(Vec4<float>{1, 1, 1, static_cast<float>(lvlSelectIsBlinking)});
		// Draw pointer blinking.
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, lvlSelectClickedBtn.render.rect.transform.pos.x - LVL_SELECT_POINTER_OFFSET.x, lvlSelectClickedBtn.render.rect.transform.pos.y - LVL_SELECT_POINTER_OFFSET.y, 60, 90, TextureManager::POINTER);
		return;
	}

	// Cache mouse position.
	Vec2<int> mousePos = InputManager::GetMousePos();

	// Convert to world space position.
	mousePos.x -= (int)AEGfxGetWinMaxX();
	mousePos.y -= (int)AEGfxGetWinMaxY();

	// Loop through all buttons.
	for (RenderSystem::Interactable& btn : lvlSelectButtons) {
		// Skip if button is not visible or clickable.
		if (!btn.isActive || !btn.isClickable) continue;

		// Check if mouse is hovering button.
		if (MouseInsideButton(mousePos, btn.render.rect.transform.pos, btn.render.rect.transform.size)) {
			// Scale btn for visual feedback.
			btn.render.rect.transform.size = btn.render.rect.transform.cachedSize * 1.1f;

			// Draw pointer.
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x - LVL_SELECT_POINTER_OFFSET.x, btn.render.rect.transform.pos.y - LVL_SELECT_POINTER_OFFSET.y, 60, 90, TextureManager::POINTER, 3);
		}
		else {
			// Scale btn to original size.
			btn.render.rect.transform.size = btn.render.rect.transform.cachedSize;
		}
	}
}

void HandleLvlSelectBtnClick() {
	// Prevent any click if transitioning.
	if (lvlSelectIsTransitioning) return;

	// Cache mouse position.
	Vec2<int> mousePos = InputManager::GetMousePos();

	// Convert to world space position.
	mousePos.x -= (int)AEGfxGetWinMaxX();
	mousePos.y -= (int)AEGfxGetWinMaxY();

	// Loop through all buttons.
	for (RenderSystem::Interactable& btn : lvlSelectButtons) {
		// Skip if button is not visible or clickable.
		if (!btn.isActive || !btn.isClickable) continue;

		// Check if mouse is hovering button.
		if (MouseInsideButton(mousePos, btn.render.rect.transform.pos, btn.render.rect.transform.size)) {
			// Cache button.
			lvlSelectClickedBtn = btn;

			switch (lvlSelectClickedBtn.render.rect.graphics.tex)
			{
			case TextureManager::QUIT_BTN:
			case TextureManager::YES_BTN:
			case TextureManager::NO_BTN:
				// Immediate calling of button function when clicking. No transition needed. (For the above buttons)
				lvlSelectClickedBtn.func();
				break;
			default:
				// Play blinking pointer transition.
				lvlSelectIsTransitioning = true;
				break;
			}
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// Button functions

void LvlSelectReturnToMenu() {
	SceneManager::LoadScene(SceneManager::MAIN_MENU);
}

void LvlSelectLoadMap1() {
	MapToLoad("Assets/JSON_Data/Maps/map0.momomaps");
}

void LvlSelectLoadMap2() {
	MapToLoad("Assets/JSON_Data/Maps/map1.momomaps");
}

void LvlSelectLoadMap3() {
	MapToLoad("Assets/JSON_Data/Maps/map2.momomaps");
}

void LvlSelectLoadUserMap1() {
	MapToLoad("Assets/JSON_Data/Maps/userMap0.momomaps");
}

void LvlSelectLoadUserMap2() {
	MapToLoad("Assets/JSON_Data/Maps/userMap1.momomaps");
}
