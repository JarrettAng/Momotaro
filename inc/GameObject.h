#pragma once

#include <MomoMaths.h>
#include <RenderSystem.h>

class GameObject {
	public:
		virtual void Start();
		
		virtual void Update();
		virtual void FixedUpdate();

		virtual void Render();

		virtual void Destroy();

		Vec3<int> position;
		RenderSystem::SpriteInfo spriteInfo;
};
