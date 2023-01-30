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
	for (auto object : objects) {
		object->Update();
	}
}

void SceneSplashscreen::FixedUpdate() {
	for (auto object : objects) {
		object->FixedUpdate();
	}
}

void SceneSplashscreen::Draw() {
	AEGfxSetBackgroundColor(0.52f, 0.89f, 1.0f);


	for (auto object : objects) {
		// renderer.AddBatch(object->Render());
	}

	RenderSystem::Render();
}

void SceneSplashscreen::Free() {
	for (auto object : objects) {
		object->Destroy();
	}

	objects.clear();
}

void SceneSplashscreen::Unload() {

}
