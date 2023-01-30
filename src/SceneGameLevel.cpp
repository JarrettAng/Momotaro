
#include <AEEngine.h>
#include <RenderSystem.h>

#include <SceneManager.h>
#include <GridManager.h>
#include <BuildingManager.h>
#include <InputManager.h>
#include <CardManager.h>

#include <SceneGameLevel.h>
#include <ColorTable.h>
#include <PauseManager.h>
#include <UIManager.h>


void SceneGameLevel::Load() {
	BuildingManager::Initialize();
}

void SceneGameLevel::Initialize() {
	PauseManager::Initialize();
	UIManager::Initialize();
	CardManager::Initialize();
}

void SceneGameLevel::Update() {
	GridManager::UpdateMouseToGrid();

	//UIManager::UIPause();

	for (auto object : objects) {
		object->Update();
	}
}

void SceneGameLevel::FixedUpdate() {
	for (auto object : objects) {
		object->FixedUpdate();
	}
}

void SceneGameLevel::Draw() {
	AEGfxSetBackgroundColor(WATER);

	//Make the grid
	GridManager::PrepareTileRenderBatch();
	//UIManager::PrepareUIRenderBatch();
	CardManager::PrepareUIRenderBatch();
	PauseManager::Update();
	UIManager::LevelUI();
	//Debug
	//printf("MinX %f\nMaxX %f\nMinY %f\nMaxY %f\n", AEGfxGetWinMinX(), AEGfxGetWinMaxX(), AEGfxGetWinMinY(), AEGfxGetWinMaxY);

	//Add the pause button
	//UIManager::AddRectToBatch(800.0f, -400.0f, 10.0f, 10.0f, 6 , AEGfxTexture * tex);


	//TEMP--------------------------------------------------------------------------
	//Change when Scoring system has been made
	//Change to UI element that changes accordingly
	UIManager::MakeText();

	for (auto object : objects) {
		// renderer.AddBatch(object->Render());
	}

	RenderSystem::Render();
}

void SceneGameLevel::Free() {
	PauseManager::Free();

	for (auto object : objects) {
		object->Destroy();
	}

	objects.clear();
}

void SceneGameLevel::Unload() {
	BuildingManager::Clear();
}

void SceneGameLevel::AddObject(GameObject* newObject) {
	objects.push_back(newObject);

	newObject->Start();
}

void SceneGameLevel::RemoveObject(GameObject* newObject) {
	for (auto i = objects.begin(); i != objects.end(); ++i) {
		if ((*i) == newObject) {
			objects.erase(i);
			break;
		}
	}

	newObject->Destroy();
}
