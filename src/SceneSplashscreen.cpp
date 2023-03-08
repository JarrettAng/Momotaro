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

#include <SceneSplashscreen.h>
#include <RenderSystem.h>

#include <SceneManager.h>

#include <TextureManager.h>

const float SPLASH_SCREEN_TIME = 3.0f;
float fade = 0;

void SceneSplashscreen::Load() {
}

void SceneSplashscreen::Initialize() {
	RenderSystem::Initialize();
}

void SceneSplashscreen::Update() {
	if (fade >= SPLASH_SCREEN_TIME) {
		SceneManager::LoadScene(SceneManager::GAME_LEVEL);
	}
}

void SceneSplashscreen::Draw() {
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	//Display the Digipen logo
	RenderSystem::SetRenderSetting(Vec4<float>{1.0f, 1.0f, 1.0f, fade});
	fade += 1.0f / (3.0f / AEFrameRateControllerGetFrameTime());

	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, -300, 85, 600, 171, TextureManager::DIGIPEN_LOGO);

	RenderSystem::Render();
}

void SceneSplashscreen::Free() {
}

void SceneSplashscreen::Unload() {
}
