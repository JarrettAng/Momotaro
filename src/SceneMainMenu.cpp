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

#include <TextureManager.h>
#include <SceneMainMenu.h>

void startGame();

void SceneMainMenu::Load() {
}

void SceneMainMenu::Initialize() {
	RenderSystem::Initialize();
	InputManager::SubscribeToKey(AEVK_SPACE, InputManager::TRIGGERED, startGame);
}

void SceneMainMenu::Update() {
	InputManager::HandleInput();
}

void SceneMainMenu::Draw() {
	// Draw menu
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, AEGfxGetWinMinX(), -AEGfxGetWinMinY(), AEGetWindowWidth(), AEGetWindowHeight(), TextureManager::MENU_BG, -1);

	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, -420, -100, 460, 100, TextureManager::STARTGAME_BTN);

	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, -420, -220, 300, 100, TextureManager::OPTIONS_BTN);

	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, 190, -100, 260, 100, TextureManager::EDITOR_BTN);

	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, 90, -220, 360, 100, TextureManager::CONTROLS_BTN);

	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, -780, -330, 300, 100, TextureManager::CREDITS_BTN);

	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, 610, -330, 170, 110, TextureManager::QUIT_BTN);

	RenderSystem::Render();
}

void SceneMainMenu::Free() {
	InputManager::Free();
}

void SceneMainMenu::Unload() {
}

void startGame() {
	SceneManager::LoadScene(SceneManager::GAME_LEVEL);
}
