/*!************************************************************************
\file:          IsometricGrid.cpp
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

void SceneSplashscreen::Load() {
}

void SceneSplashscreen::Initialize() {

}

void SceneSplashscreen::Update() {

}

void SceneSplashscreen::Draw() {
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	RenderSystem::Render();

	//Display the splash screen
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, 0, 0, 1500, 1500, TextureManager::SPLASH_SCREEN);

}

void SceneSplashscreen::Free() {

}

void SceneSplashscreen::Unload() {

}
