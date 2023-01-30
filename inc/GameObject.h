/*!************************************************************************
\file:          GameObject.h
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

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
