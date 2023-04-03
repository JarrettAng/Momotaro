/*!************************************************************************
\file			GameOverManager.h
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			29-02-2023
\brief
This header file contains prototype functions and declarations for GameOverManager.cpp.

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <InputManager.h>
#include <SceneManager.h>
#include <TextureManager.h>
#include <RenderSystem.h>
#include <MomoMaths.h>
#include <GridManager.h>
#include <CardManager.h>
#include <AudioManager.h>
///////////////////////////////////////////////////////////////////////////

namespace GameOverManager {
	/*!***********************************************************************
	\brief
		Initialize the GameOverManager.
	*************************************************************************/
	void Initialize();

	/*!***********************************************************************
	\brief
		Update the GameOverManager.
	*************************************************************************/
	void Update();

	/*!***********************************************************************
	\brief
		Draw the GameOverManager.
	*************************************************************************/
	void Draw();

	/*!***********************************************************************
	\brief
		Free the GameOverManager.
	*************************************************************************/
	void Free();

	/*!***********************************************************************
	\brief
		Check if it is game over.
	*************************************************************************/
	bool IsGameOver();
}
