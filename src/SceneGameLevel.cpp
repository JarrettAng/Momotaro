/*!************************************************************************
\file:          SceneGameLevel.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <AEEngine.h>
#include <RenderSystem.h>

#include <SceneManager.h>
#include <GridManager.h>
#include <BuildingManager.h>
#include <InputManager.h>
#include <CardManager.h>
#include <UIManager.h>

#include <SceneGameLevel.h>
#include <ColorTable.h>
#include <PauseManager.h>
#include <ScoreManager.h>
#include <TextureManager.h>
#include <AudioManager.h>
#include <GameOverManager.h>

std::string mapToLoad = "Assets/JSON_Data/Maps/map0.momomaps";

void SceneGameLevel::Load() {
}

void SceneGameLevel::Initialize() {
	ScoreManager::Initialize();
	GridManager::Initialize(mapToLoad);
	PauseManager::Initialize();
	CardManager::Initialize();
	GameOverManager::Initialize();
}

void SceneGameLevel::Update() {
	GridManager::UpdateMouseToGrid();
	PauseManager::Update();
	TextureManager::Update();
	CardManager::Update();
	GameOverManager::Update();
}

void SceneGameLevel::Draw() {
	AEGfxSetBackgroundColor(WATER);

	//Make the grid
	GridManager::PrepareTileRenderBatch();
	CardManager::PrepareUIRenderBatch();
	PauseManager::Draw();
	ScoreManager::Draw();
	GameOverManager::Draw();

	RenderSystem::Render();
}

void SceneGameLevel::Free() {
	GridManager::Free();
	InputManager::Free();
	CardManager::Free();
	RenderSystem::Free();
	PauseManager::Free();
	ScoreManager::Free();
	GameOverManager::Free();
}

void SceneGameLevel::Unload() {
	BuildingManager::Clear();
}

void MapToLoad(std::string const& mapFilePath) {
	mapToLoad = mapFilePath;
}
