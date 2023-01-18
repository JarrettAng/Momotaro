#pragma once

#include <list>

#include <EventSystem.h>
// #include <RenderSystem.h>
#include <GameObject.h>

class Scene {
	public:
		virtual void Load();
		virtual void Initialize();

		virtual void Update();
		virtual void FixedUpdate();
		virtual void Draw();

		virtual void Free();
		virtual void Unload();

		virtual void AddObject(GameObject *newObject);
		virtual void RemoveObject(GameObject *newObject);

	protected:
		// RenderSystem::Renderer renderer;
		std::list<GameObject*> objects;
};
