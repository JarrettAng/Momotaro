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

void SceneSplashscreen::Load() {
}

void SceneSplashscreen::Initialize() {

}

void SceneSplashscreen::Update() {

}

void SceneSplashscreen::Draw() {
	AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f);

	//Display the splash screen
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, 0, 0, 1525, 445, TextureManager::DIGIPEN_LOGO);

	RenderSystem::Render();

}

void SceneSplashscreen::Free() {

}

void SceneSplashscreen::Unload() {

}
