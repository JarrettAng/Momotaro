/*!************************************************************************
\file:          SceneCredits.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          09-03-2023
\brief


The functions include:
-
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <SceneManager.h>

#include <SceneCredits.h>
#include <AEEngine.h>
#include <InputManager.h>
#include <RenderSystem.h>

#include <TextureManager.h>
///////////////////////////////////////////////////////////////////////////

/*!***********************************************************************
* FORWARD DECLARATIONS
*************************************************************************/
void ReturnToMenu();

RenderSystem::Interactable backBtn{};

void SceneCredits::Load() {
	return;
}

void SceneCredits::Initialize() {
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, ReturnToMenu);
}

void SceneCredits::Update() {
	// Check for button presses.
	InputManager::HandleInput();
}

void SceneCredits::Draw() {
	// Draw background to fit to screen.
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, AEGfxGetWinMinX(), -AEGfxGetWinMinY(), (float)AEGetWindowWidth(), (float)AEGetWindowHeight(), TextureManager::MENU_BG, -1);

	RenderSystem::Render();
}

void SceneCredits::Free() {
	InputManager::Free();
}

void SceneCredits::Unload() {
	return;
}

void ReturnToMenu() {
	return;
}
