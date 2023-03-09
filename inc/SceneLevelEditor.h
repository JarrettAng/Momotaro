/*!************************************************************************
\file:          SceneLevelEditor.h
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          09-03-2023
\brief


The functions include:
-
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
