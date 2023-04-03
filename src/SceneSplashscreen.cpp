/*!************************************************************************
\file			SceneSplashscreen.cpp
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			31-03-2023
\brief
This source file handles the splash screen of the game.

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#include <RenderSystem.h>

#include <UIManager.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <TextureManager.h>

#include <SceneSplashscreen.h>
#include <AudioManager.h>
#include <ColorTable.h>

/*!***********************************************************************
* Const Variable.
*************************************************************************/
const float SPLASH_SCREEN_TIME = 2.0f;

/*!***********************************************************************
* Forward Declarations.
*************************************************************************/
void BypassIntro();

/*!***********************************************************************
* UI.
*************************************************************************/
UI::TextBox skipText;

/*!***********************************************************************
* Variables.
*************************************************************************/
float fade = 0;

/*!***********************************************************************
\brief
	Load SceneSplashScreen.
*************************************************************************/
void SceneSplashscreen::Load() {
	AudioManager::Load();
	return;
}

/*!***********************************************************************
\brief
	Initialize SceneSplashScreen.
*************************************************************************/
void SceneSplashscreen::Initialize() {
	// Subscribe to key events.
	InputManager::SubscribeToKey(AEVK_ESCAPE, InputManager::TRIGGERED, BypassIntro);
	InputManager::SubscribeToKey(AEVK_SPACE, InputManager::TRIGGERED, BypassIntro);

	// Initialize skip text.
	skipText = UI::TextBox({ -AEGfxGetWinMaxX() * 0.5f, -AEGfxGetWinMaxY() + 10.0f }, 
							 "Press Esc or Space to skip.", UI::CENTER_JUSTIFY, AEGfxGetWinMaxX(), 20.0f, COLOR_WHITE);
}

/*!***********************************************************************
\brief
	Update SceneSplashScreen.
*************************************************************************/
void SceneSplashscreen::Update() {
	if (fade >= SPLASH_SCREEN_TIME) {
		// Load main menu.
		SceneManager::LoadScene(SceneManager::MAIN_MENU);
	}
}

/*!***********************************************************************
\brief
	Draw SceneSplashScreen.
*************************************************************************/
void SceneSplashscreen::Draw() {
	// Black background.
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	//Display the Digipen logo
	RenderSystem::SetRenderSetting(Vec4<float>{1.0f, 1.0f, 1.0f, fade / SPLASH_SCREEN_TIME});
	// Fade in.
	fade += 1.0f / (SPLASH_SCREEN_TIME / (float)AEFrameRateControllerGetFrameTime());

	// Add to render batch.
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, -300, 85, 600, 171, TextureManager::DIGIPEN_LOGO);
	skipText.Render();

	RenderSystem::Render();
}

/*!***********************************************************************
\brief
	Free SceneSplashScreen.
*************************************************************************/
void SceneSplashscreen::Free() {
	// Unsubscribe to key events.
	InputManager::UnsubscribeKey(AEVK_ESCAPE, InputManager::TRIGGERED, BypassIntro);
	InputManager::UnsubscribeKey(AEVK_SPACE, InputManager::TRIGGERED, BypassIntro);
}

/*!***********************************************************************
\brief
	Unload SceneSplashScreen.
*************************************************************************/
void SceneSplashscreen::Unload() {
	return;
}

/*!***********************************************************************
\brief
	Skip splash screen.
*************************************************************************/
void BypassIntro() {
	fade = SPLASH_SCREEN_TIME;
}
