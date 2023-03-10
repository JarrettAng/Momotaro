/*!************************************************************************
\file:          Scene.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
- Load

- Initialize

- Update

- FixedUpdate

- Draw

- Free

- Unload

- AddObject

- RemoveObject

**************************************************************************/

#pragma once

#include <list>
#include <MomoMaths.h>
#include <RenderSystem.h>
#include <EventSystem.h>

class Scene {
public:
	virtual void Load();
	virtual void Initialize();

	virtual void Update();
	virtual void Draw();

	virtual void Free();
	virtual void Unload();
};
