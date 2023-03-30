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
#include <AudioManager.h>

#include <SceneLevelEditor.h>
#include <ColorTable.h>
#include <PauseManager.h>
#include <TextureManager.h>
///////////////////////////////////////////////////////////////////////////
// Forward Declarations
void LoadMap1();
void LoadMap2();
void LoadMap3();
void LoadUserMap1();
void LoadUserMap2();
void LoadUserMap3();

///////////////////////////////////////////////////////////////////////////
// Variables
std::string selectedMapFilePath;


void SceneLevelEditor::Load() {
	BuildingManager::Initialize();
	GridManager::Initialize("Assets/JSON_Data/Maps/map0.momomaps");
}

void SceneLevelEditor::Initialize() {
	PauseManager::Initialize();

	// Bind keys 1-6 to the different maps
	InputManager::SubscribeToKey(AEVK_1, InputManager::TRIGGERED, LoadMap1);
	InputManager::SubscribeToKey(AEVK_2, InputManager::TRIGGERED, LoadMap2);
	InputManager::SubscribeToKey(AEVK_3, InputManager::TRIGGERED, LoadMap3);
	InputManager::SubscribeToKey(AEVK_4, InputManager::TRIGGERED, LoadUserMap1);
	InputManager::SubscribeToKey(AEVK_5, InputManager::TRIGGERED, LoadUserMap2);
}

void SceneLevelEditor::Update() {
	GridManager::UpdateMouseToGrid();
	PauseManager::Update();
	AudioManager::Update();
}

void SceneLevelEditor::Draw() {
	AEGfxSetBackgroundColor(WATER);

	GridManager::PrepareTileRenderBatch();
	PauseManager::Draw();

	RenderSystem::Render();
}

void SceneLevelEditor::Free() {
	GridManager::Free();

	InputManager::UnsubscribeKey(AEVK_1, InputManager::TRIGGERED, LoadMap1);
	InputManager::UnsubscribeKey(AEVK_2, InputManager::TRIGGERED, LoadMap2);
	InputManager::UnsubscribeKey(AEVK_3, InputManager::TRIGGERED, LoadMap3);
	InputManager::UnsubscribeKey(AEVK_4, InputManager::TRIGGERED, LoadUserMap1);
	InputManager::UnsubscribeKey(AEVK_5, InputManager::TRIGGERED, LoadUserMap2);
}

void SceneLevelEditor::Unload() {
	BuildingManager::Clear();
}

///////////////////////////////////////////////////////////////////////////
// Map loading

void LoadSelectedMap() {
	GridManager::Free();
	GridManager::Initialize(selectedMapFilePath);
}

void LoadMap1() {
	selectedMapFilePath = "Assets/JSON_Data/Maps/map0.momomaps";
	LoadSelectedMap();
}

void LoadMap2() {
	selectedMapFilePath = "Assets/JSON_Data/Maps/map1.momomaps";
	LoadSelectedMap();
}

void LoadMap3() {
	selectedMapFilePath = "Assets/JSON_Data/Maps/map2.momomaps";
	LoadSelectedMap();
}

void LoadUserMap1() {
	selectedMapFilePath = "Assets/JSON_Data/Maps/userMap0.momomaps";
	LoadSelectedMap();
}

void LoadUserMap2() {
	selectedMapFilePath = "Assets/JSON_Data/Maps/userMap1.momomaps";
	LoadSelectedMap();
}
