/*!************************************************************************
\file:          CardManager.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This header file holds the data of each card in the player's hand and handles
the creation/deletion of the cards.

The functions include:
- Initialize
Draws the cards for the player to play with at the same of the game scene
- PrepareUIRenderBatch
Sends data to UIManager to render the cards
- DrawCard
Draws a draw into the player's hand of a given type, level, size
- DrawRandomCard
Draws a random draw into the player's hand
**************************************************************************/

#pragma once

#include <string>
#include <BuildingManager.h>

namespace CardManager {
	void Initialize();

	void PrepareUIRenderBatch();

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level);

	void DrawRandomCard(BuildingEnum::LEVEL level);
	void DrawRandomCard(BuildingEnum::TYPE type);

	void Free();
}
