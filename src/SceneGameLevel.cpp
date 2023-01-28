
#include <AEEngine.h>
#include <RenderSystem.h>

#include <SceneManager.h>
#include <GridManager.h>
#include <BuildingManager.h>
#include <InputManager.h>

#include <SceneGameLevel.h>
#include <ColorTable.h>
#include <PauseManager.h>


void SceneGameLevel::Load() {
	BuildingManager::Initialize();
}

void SceneGameLevel::Initialize() {
	PauseManager::Initialize();
}

void SceneGameLevel::Update() {
	GridManager::ScreenToTilePos();

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

	GridManager::PrepareTileRenderBatch();

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
