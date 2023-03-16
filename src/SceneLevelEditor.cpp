/*!************************************************************************
\file:          SceneLevelEditor.cpp
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
#include <UIManager.h>

#include <SceneLevelEditor.h>
#include <ColorTable.h>
#include <PauseManager.h>
#include <TextureManager.h>
///////////////////////////////////////////////////////////////////////////

void SceneLevelEditor::Load() {
	GridManager::Initialize("Assets/JSON_Data/Maps/tutorial.momomaps");
}

void SceneLevelEditor::Initialize() {
	PauseManager::Initialize();
}

void SceneLevelEditor::Update() {
	GridManager::UpdateMouseToGrid();
	PauseManager::Update();
}

void SceneLevelEditor::Draw() {
	AEGfxSetBackgroundColor(WATER);

	GridManager::PrepareTileRenderBatch();
	PauseManager::Draw();

	RenderSystem::Render();
}

void SceneLevelEditor::Free() {
	GridManager::Free();
}

void SceneLevelEditor::Unload() {
	return;
}
