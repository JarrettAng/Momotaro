#pragma once

#include<Scene.h>

class SceneGameLevel : public Scene {
	public:
		void Load();
		void Initialize();

		void Update();
		void FixedUpdate();
		void Draw();

		void Free();
		void Unload();

		void AddObject(GameObject *newObject);
		void RemoveObject(GameObject *newObject);
};
