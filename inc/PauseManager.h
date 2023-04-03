/*!************************************************************************
\file			PauseManager.h
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			29-02-2023
\brief
This header file contians prototype functions and declarations for PauseManager.cpp.

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <EventSystem.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <TextureManager.h>
#include <RenderSystem.h>
#include <MomoMaths.h>
#include <GridManager.h>
#include <CardManager.h>
///////////////////////////////////////////////////////////////////////////

namespace PauseManager {
	/*!***********************************************************************
	\brief
		Initialize PauseManager.
	*************************************************************************/
	void Initialize();

	/*!***********************************************************************
	\brief
		Update PauseManager.
	*************************************************************************/
	void Update();

	/*!***********************************************************************
	\brief
		Draw the PauseManager.
	*************************************************************************/
	void Draw();

	/*!***********************************************************************
	\brief
		Free PauseManager.
	*************************************************************************/
	void Free();

	/*!***********************************************************************
	\brief
		Is the game paused?
	*************************************************************************/
	bool IsPaused();

	/*!***********************************************************************
	\brief
		Toggle pause UI.
	*************************************************************************/
	void TogglePause();

	/*!***********************************************************************
	\brief
		Toggle restart button based on given bool.
	*************************************************************************/
	void ToggleShowRestart(bool state);

	/*!***********************************************************************
	\brief
		Toggle pause button based on given bool.
	*************************************************************************/
    void ToggleShowPause(bool state);

}
