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
#include <UIManager.h>

#include <SceneLevelSelect.h>
#include <TextureManager.h>
#include <ColorTable.h>
///////////////////////////////////////////////////////////////////////////
// Forward Declaration
void LvlSelectInitializeUI();
void HandleLvlSelectBackBtnClick();
void HandleLvlSelectBackBtnHover();
void LvlSelectReturnToMenu();

RenderSystem::Interactable lvlSelectBackBtn{};

void SceneLevelSelect::Load() {

}

void SceneLevelSelect::Initialize() {
	LvlSelectInitializeUI();
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleLvlSelectBackBtnClick);
	AudioManager::PlayBGM(AudioManager::ClipName::BGM_CREDITS);
}

void SceneLevelSelect::Update() {
	InputManager::HandleInput();
	AudioManager::Update();
}

void SceneLevelSelect::Draw() {
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, AEGfxGetWinMinX(), -AEGfxGetWinMinY(), (float)AEGetWindowWidth(), (float)AEGetWindowHeight(), TextureManager::CREDITS_BG, -1);
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, lvlSelectBackBtn.render.rect.transform.pos.x, lvlSelectBackBtn.render.rect.transform.pos.y, lvlSelectBackBtn.render.rect.transform.size.x, lvlSelectBackBtn.render.rect.transform.size.y, TextureManager::BACK_BTN);

	HandleLvlSelectBackBtnHover();

	RenderSystem::Render();
}

void SceneLevelSelect::Free() {
	InputManager::Free();
	RenderSystem::Free();
}

void SceneLevelSelect::Unload() {

}

///////////////////////////////////////////////////////////////////////////

void LvlSelectInitializeUI() {
	// Back button
	lvlSelectBackBtn.render.rect.graphics.tex = TextureManager::BACK_BTN;
	lvlSelectBackBtn.func = LvlSelectReturnToMenu;

	lvlSelectBackBtn.render.rect.transform.pos.x = GetWorldXByPercentage(88.0f);
	lvlSelectBackBtn.render.rect.transform.pos.y = GetWorldYByPercentage(13.2f);

	lvlSelectBackBtn.render.rect.transform.size.x = 150.0f;
	lvlSelectBackBtn.render.rect.transform.size.y = 78.0f;
	lvlSelectBackBtn.render.rect.transform.cachedSize = lvlSelectBackBtn.render.rect.transform.size;
}

void HandleLvlSelectBackBtnHover() {
	// Cache mouse position.
	Vec2<int> mousePos = InputManager::GetMousePos();

	// Convert to world space position.
	mousePos.x -= (int)AEGfxGetWinMaxX();
	mousePos.y -= (int)AEGfxGetWinMaxY();

	// Check if mouse is clicking back btn.
	if (MouseInsideButton(mousePos, lvlSelectBackBtn.render.rect.transform.pos, lvlSelectBackBtn.render.rect.transform.size)) {
		// Scale btn for visual feedback.
		lvlSelectBackBtn.render.rect.transform.size = lvlSelectBackBtn.render.rect.transform.cachedSize * 1.1f;
	}
	else {
		// Scale btn to original size.
		lvlSelectBackBtn.render.rect.transform.size = lvlSelectBackBtn.render.rect.transform.cachedSize;
	}
}

void HandleLvlSelectBackBtnClick() {
	// Cache mouse position.
	Vec2<int> mousePos = InputManager::GetMousePos();

	// Convert to world space position.
	mousePos.x -= (int)AEGfxGetWinMaxX();
	mousePos.y -= (int)AEGfxGetWinMaxY();

	// Check if mouse is clicking back btn.
	if (MouseInsideButton(mousePos, lvlSelectBackBtn.render.rect.transform.pos, lvlSelectBackBtn.render.rect.transform.size)) {
		lvlSelectBackBtn.func();
	}
}

void LvlSelectReturnToMenu() {
	SceneManager::LoadScene(SceneManager::MAIN_MENU);
}
