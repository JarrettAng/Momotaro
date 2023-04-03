/*!************************************************************************
\file:          Scene.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This header file declares the base scene class that all scenes in the game will
inherit from

The functions include:
- Load
Called once at after scene swaps
- Initialize
Called after scene swaps or scene restarts
- Update
Called at the start of every frame
- Draw
Called at the end of every frame
- Free
Called before scene swaps or scene restarts
- Unload
Called before scene swaps

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <list>
#include <RenderSystem.h>
#include <EventSystem.h>
#include <MomoMaths.h>
///////////////////////////////////////////////////////////////////////////

class Scene {
public:
	virtual void Load();
	virtual void Initialize();

	virtual void Update();
	virtual void Draw();

	virtual void Free();
	virtual void Unload();
};
