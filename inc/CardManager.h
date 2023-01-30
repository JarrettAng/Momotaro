/*!************************************************************************
\file:          CardManager.h
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

#include <BuildingManager.h>

namespace CardManager {

	struct DeckData {
		int count;
		BuildingData card;
	};

	void Initialize();

	void PrepareUIRenderBatch();

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level);

	void DrawRandomCard(BuildingEnum::LEVEL level);
	void DrawRandomCard(BuildingEnum::TYPE type);
}
