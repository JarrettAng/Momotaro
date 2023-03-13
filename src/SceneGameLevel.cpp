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



void SceneGameLevel::Load() {
	GridManager::Initialize();
	BuildingManager::Initialize();
	ScoreManger::Initialize();
}

void SceneGameLevel::Initialize() {
	GridManager::onBoardFull.Subscribe(GameOver);
	CardManager::onHandEmpty.Subscribe(GameOver);
	PauseManager::Initialize();
	CardManager::Initialize();
}

void SceneGameLevel::Update() {
	GridManager::UpdateMouseToGrid();
	PauseManager::Update();
	TextureManager::Update();
	CardManager::Update();
}

void SceneGameLevel::Draw() {
	AEGfxSetBackgroundColor(WATER);

	//Make the grid
	GridManager::PrepareTileRenderBatch();
	CardManager::PrepareUIRenderBatch();
	PauseManager::Draw();
	ScoreManger::Draw();

	RenderSystem::Render();
}

void SceneGameLevel::Free() {
	InputManager::Free();
	CardManager::Free();
	RenderSystem::Free();
}

void SceneGameLevel::Unload() {
	GridManager::Free();
	BuildingManager::Clear();
}

void GameOver(){
	std::cout << "Spawn Game Over UI!\n";
}

