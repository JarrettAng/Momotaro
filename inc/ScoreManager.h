/*!************************************************************************
\file			ScoreManager.h
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			18-01-2023
\brief
This header file contains prototype functions and declarations for ScoreManager.cpp.

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/
#pragma once
///////////////////////////////////////////////////////////////////////////
#include <EventSystem.h>
#include <AEEngine.h>
#include <string>
#include <vector>
#include <iostream>
///////////////////////////////////////////////////////////////////////////

namespace ScoreManager {
	/*!***********************************************************************
	* Events.
	*************************************************************************/
	extern EventSystem::Event<void> onScoreUpdate;
	extern EventSystem::Event<void> onLevelChange;

	/*!***********************************************************************
	\brief
		Initialize ScoreManager.
	*************************************************************************/
	void Initialize();

	/*!***********************************************************************
	\brief
		Draw ScoreManager.
	*************************************************************************/
	void Draw();

	/*!***********************************************************************
	\brief
		Free ScoreManager.
	*************************************************************************/
	void Free();

	/*!***********************************************************************
	\brief
		Get player's highscore.	
	*************************************************************************/
	int GetHighScore();
	int GetLevel();
	int GetCurrScore();

	/*!***********************************************************************
	\brief
		Set player's highscore.
	*************************************************************************/
	void SetHighScore(int _score);
	void SetLevel(int _level);
	void SetScore(int _currScore);

	/*!***********************************************************************
	\brief
		Add score.
	*************************************************************************/
	void AddScore(int modifier);

	/*!***********************************************************************
	\brief
		Toggle ShowHighScore state.
	*************************************************************************/
	void ToggleShowHighScore(bool state);

	/*!***********************************************************************
	\brief
		Update potential score text.
	*************************************************************************/
	void UpdatePotentialScoreGain(int);
}
