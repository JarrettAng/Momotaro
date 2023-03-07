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

float splashScreenTime = 3.0f;
RenderSystem::RenderSetting fadeSetting;

void SceneSplashscreen::Load() {
}

void SceneSplashscreen::Initialize() {
	RenderSystem::Initialize();
}

void SceneSplashscreen::Update() {
	splashScreenTime -= AEFrameRateControllerGetFrameTime();

	if (splashScreenTime <= 0) {
		SceneManager::LoadScene(SceneManager::GAME_LEVEL);
	}
}

void SceneSplashscreen::Draw() {
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	//Display the Digipen logo
	fadeSetting.tint.z -= 1 / (3 / AEFrameRateControllerGetFrameTime());
	*RenderSystem::GetRenderSetting() = fadeSetting;
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, -300, 85, 600, 171, TextureManager::DIGIPEN_LOGO);

	RenderSystem::Render();
}

void SceneSplashscreen::Free() {
}

void SceneSplashscreen::Unload() {
}
