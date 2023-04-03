/*!************************************************************************
\file:          Scene.cpp
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This source file defines stub functions for the base scene class that 
all scenes in the game will inherit from

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

///////////////////////////////////////////////////////////////////////////
#include <Scene.h>
///////////////////////////////////////////////////////////////////////////

// Empty by design \/

void Scene::Load() { }

void Scene::Initialize() { }

void Scene::Update() { }

void Scene::Draw() { }

void Scene::Free() { }

void Scene::Unload() { }
