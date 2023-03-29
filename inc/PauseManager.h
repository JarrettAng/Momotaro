/*!************************************************************************
\file:          PauseManager.h
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

#include <EventSystem.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <TextureManager.h>
#include <RenderSystem.h>
#include <MomoMaths.h>
#include <GridManager.h>
#include <CardManager.h>

namespace PauseManager {
	bool IsPaused();
	void Initialize();
	void Update();
	void Draw();
	void Free();
	void TogglePause();
}
