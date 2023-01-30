/*!************************************************************************
\file:          Scene.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <Scene.h>

void Scene::Load() {

}

void Scene::Initialize() {

}

void Scene::Update() {
	for (auto object : objects) {
		object->Update();
	}
}

void Scene::FixedUpdate() {
	for (auto object : objects) {
		object->FixedUpdate();
	}
}

void Scene::Draw() {
	for (auto object : objects) {
		// renderer.AddBatch(object->Render());
	}

	// renderer.Render();
}

void Scene::Free() {
	for (auto object : objects) {
		object->Destroy();
	}

	objects.clear();
}

void Scene::Unload() {

}

void Scene::AddObject(GameObject* newObject) {
	objects.push_back(newObject);

	newObject->Start();
}

void Scene::RemoveObject(GameObject* newObject) {
	for (auto i = objects.begin(); i != objects.end(); ++i) {
		if ((*i) == newObject) {
			objects.erase(i);
			break;
		}
	}

	newObject->Destroy();
}
