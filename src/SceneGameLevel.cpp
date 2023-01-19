
#include <SceneManager.h>
#include <SceneGameLevel.h>

int counter = 0;

void SceneGameLevel::Load() {

}

void SceneGameLevel::Initialize() {

}

void SceneGameLevel::Update() {
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
	for (auto object : objects) {
		// renderer.AddBatch(object->Render());
	}

	// renderer.Render();
}

void SceneGameLevel::Free() {
	for (auto object : objects) {
		object->Destroy();
	}

	objects.clear();
}

void SceneGameLevel::Unload() {

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
