/*!************************************************************************
\file GameOverManager.h
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 29-02-2023
\brief
This source file declares

**************************************************************************/
#pragma once

#include <InputManager.h>
#include <SceneManager.h>
#include <TextureManager.h>
#include <RenderSystem.h>
#include <MomoMaths.h>
#include <GridManager.h>
#include <CardManager.h>
#include <AudioManager.h>

namespace GameOverManager {
	void Initialize();
	void Update();
	void Draw();
	void Free();
	bool IsGameOver();
}
