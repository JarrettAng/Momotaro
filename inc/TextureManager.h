/*!************************************************************************
\file:          TextureManager.h
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

#include <AEEngine.h>

namespace TextureManager {

	enum TEX_TYPE {
		RESIDENTIAL_1X1_L1 = 1,
		RESIDENTIAL_1X1_L2,
		RESIDENTIAL_1X1_L3,
		COMMERCIAL_1X1_L1 = 4,
		COMMERCIAL_1X1_L2,
		COMMERCIAL_1X1_L3,
		INDUSTRIAL_1X1_L1 = 7,
		INDUSTRIAL_1X1_L2,
		INDUSTRIAL_1X1_L3,
		TILE_TEX,
		NATURE_TREE,
		NATURE_ROCK,
		CARD_BLUE,
		PAUSE_WINDOW,
		PAUSE_BUTTON,
		SPLASH_SCREEN,
	};
	void Initialize();
	AEGfxTexture* GetTexture(const TEX_TYPE& type);
}

