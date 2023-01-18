#pragma once

#include <list>

#include <EventSystem.h>
// #include <RenderSystem.h>
#include <GameObject.h>

class Scene {
	public:
		virtual void Load() = 0;
		virtual void Initialize() = 0;

		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
		virtual void Draw() = 0;

		virtual void Free() = 0;
		virtual void Unload() = 0;

		virtual void AddObject(GameObject &newObject) = 0;
		virtual void RemoveObject(GameObject &newObject) = 0;

	protected:
		// RenderSystem::Renderer renderer;
		std::list<GameObject*> objects;
};
