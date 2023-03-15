/*!************************************************************************
\file:          SceneTutorial.cpp
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
#include <AEEngine.h>
#include <RenderSystem.h>

#include <SceneManager.h>
#include <GridManager.h>
#include <BuildingManager.h>
#include <InputManager.h>
#include <CardManager.h>
#include <UIManager.h>

#include <SceneTutorial.h>
#include <ColorTable.h>
#include <PauseManager.h>
#include <ScoreManager.h>
#include <TextureManager.h>
///////////////////////////////////////////////////////////////////////////

void SceneTutorial::Load() {
	GridManager::Initialize();
}

void SceneTutorial::Initialize() {
	PauseManager::Initialize();
}

void SceneTutorial::Update() {
	GridManager::UpdateMouseToGrid();
	PauseManager::Update();
}

void SceneTutorial::Draw() {
	AEGfxSetBackgroundColor(WATER);

	GridManager::PrepareTileRenderBatch();
	PauseManager::Draw();

	RenderSystem::Render();
}

void SceneTutorial::Free() {
	GridManager::Free();
}

void SceneTutorial::Unload() {
	return;
}
