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
	BuildingManager::Initialize();
}

void SceneGameLevel::Initialize() {
	PauseManager::Initialize();
	CardManager::Initialize();
	//UIManager::Initialize();
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

	//Debug
	//printf("MinX %f\nMaxX %f\nMinY %f\nMaxY %f\n", AEGfxGetWinMinX(), AEGfxGetWinMaxX(), AEGfxGetWinMinY(), AEGfxGetWinMaxY);
	//Add the pause button
	//UIManager::AddRectToBatch(800.0f, -400.0f, 10.0f, 10.0f, 6 , AEGfxTexture * tex);
	//-causes error
	//UIManager::Draw();

	//for (auto object : objects) {
	//	// renderer.AddBatch(object->Render());
	//}

	RenderSystem::Render();
}

void SceneGameLevel::Free() {
	PauseManager::Free();
	InputManager::Free();
	CardManager::Free();
}

void SceneGameLevel::Unload() {
	BuildingManager::Clear();
}
