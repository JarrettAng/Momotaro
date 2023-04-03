/*!************************************************************************
\file:          SceneLevelEditor.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          09-03-2023
\brief
This header file handles the level editor of the game.

The functions include:
- Standard base functions from scene

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <Scene.h>
///////////////////////////////////////////////////////////////////////////

class SceneLevelEditor : public Scene {
public:
	void Load();
	void Initialize();

	void Update();
	void Draw();

	void Free();
	void Unload();
};
