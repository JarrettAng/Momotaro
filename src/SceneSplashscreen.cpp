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

#include <SceneManager.h>

#include <TextureManager.h>
#include <SceneSplashscreen.h>

const float SPLASH_SCREEN_TIME = 1.0f;
float fade = 0;

void SceneSplashscreen::Load() {
	return;
}

void SceneSplashscreen::Initialize() {
	return;
}

void SceneSplashscreen::Update() {
	if (fade >= SPLASH_SCREEN_TIME) {
		SceneManager::LoadScene(SceneManager::MAIN_MENU);
	}
}

void SceneSplashscreen::Draw() {
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	//Display the Digipen logo
	RenderSystem::SetRenderSetting(Vec4<float>{1.0f, 1.0f, 1.0f, fade});
	fade += 1.0f / (SPLASH_SCREEN_TIME / (float)AEFrameRateControllerGetFrameTime());

	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, -300, 85, 600, 171, TextureManager::DIGIPEN_LOGO);

	RenderSystem::Render();
}

void SceneSplashscreen::Free() {
	return;
}

void SceneSplashscreen::Unload() {
	return;
}
