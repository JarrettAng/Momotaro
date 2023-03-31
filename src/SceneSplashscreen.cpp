/*!************************************************************************
\file:          SceneSplashscreen.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <RenderSystem.h>

#include <UIManager.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <TextureManager.h>

#include <SceneSplashscreen.h>
#include <AudioManager.h>
#include <ColorTable.h>

///////////////////////////////////////////////////////////////////////////
// Forward Declarations
void BypassIntro();

///////////////////////////////////////////////////////////////////////////
// Timing variables
const float SPLASH_SCREEN_TIME = 2.0f;
float fade = 0;

UI::TextBox skipText;

void SceneSplashscreen::Load() {
	AudioManager::Load();
	return;
}

void SceneSplashscreen::Initialize() {
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, BypassIntro);
	InputManager::SubscribeToKey(AEVK_ESCAPE, InputManager::TRIGGERED, BypassIntro);

	skipText = UI::TextBox({ -AEGfxGetWinMaxX() * 0.5f, -AEGfxGetWinMaxY() + 10.0f }, 
							 "Press Esc or Click to skip.", UI::CENTER_JUSTIFY, AEGfxGetWinMaxX(), 20.0f, COLOR_WHITE);
}

void SceneSplashscreen::Update() {
	if (fade >= SPLASH_SCREEN_TIME) {
		SceneManager::LoadScene(SceneManager::MAIN_MENU);
	}
}

void SceneSplashscreen::Draw() {
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	//Display the Digipen logo
	RenderSystem::SetRenderSetting(Vec4<float>{1.0f, 1.0f, 1.0f, fade / SPLASH_SCREEN_TIME});
	fade += 1.0f / (SPLASH_SCREEN_TIME / (float)AEFrameRateControllerGetFrameTime());

	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, -300, 85, 600, 171, TextureManager::DIGIPEN_LOGO);
	skipText.Render();

	RenderSystem::Render();
}

void SceneSplashscreen::Free() {
	InputManager::UnsubscribeKey(AEVK_LBUTTON, InputManager::TRIGGERED, BypassIntro);
	InputManager::UnsubscribeKey(AEVK_ESCAPE, InputManager::TRIGGERED, BypassIntro);
}

void SceneSplashscreen::Unload() {
	return;
}

void BypassIntro() {
	fade = SPLASH_SCREEN_TIME;
}
