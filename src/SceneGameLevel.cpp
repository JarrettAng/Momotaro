
#include <Scene.h>

class SceneGameLevel : public Scene {
	void Load() {

	}

	void Initialize() {

	}

	void Update() {
		for (auto object : objects) {
			object->Update();
		}
	}

	void FixedUpdate() {
		for (auto object : objects) {
			object->FixedUpdate();
		}
	}

	void Draw() {
		for (auto object : objects) {
			// renderer.AddBatch(object->Render());
		}

		// renderer.Render();
	}

	void Free() {
		for (auto object : objects) {
			object->Destroy();
		}

		objects.clear();
	}

	void Unload() {

	}

	void AddObject(GameObject *newObject) {
		objects.push_back(newObject);

		newObject->Start();
	}

	void RemoveObject(GameObject *newObject) {
		for (auto i = objects.begin(); i != objects.end(); ++i) {
			if ((*i) == newObject) {
				objects.erase(i);
				break;
			}
		}

		newObject->Destroy();
	}
};