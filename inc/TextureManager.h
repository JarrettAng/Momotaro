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
		NONE = 0,
		RESIDENTIAL_1X1_L1 = 1,
		RESIDENTIAL_1X1_L2,
		RESIDENTIAL_1X1_L3,
		RESIDENTIAL_1X2_L1 = 4,
		RESIDENTIAL_1X2_L2,
		RESIDENTIAL_1X2_L3,
		COMMERCIAL_1X1_L1 = 7,
		COMMERCIAL_1X1_L2,
		COMMERCIAL_1X1_L3,
		INDUSTRIAL_1X1_L1 = 10,
		INDUSTRIAL_1X1_L2,
		INDUSTRIAL_1X1_L3,
		TILE_TEX,
		NATURE_TREE,
		NATURE_ROCK,
		CARD_BLUE,
		PAUSE_WINDOW,
		PAUSE_BUTTON,
		SPLASH_SCREEN,
		SYNERGY,
	};

	struct TextureSheet {
		TEX_TYPE tex;
		int	rows;
		int cols;

		// Animation.
		float frameInterval;		// Time between each frame.
		float currInterval = 0;

		float texWidth;				// Texture frame UV width offset:	1/cols
		float texHeight;			// Texture frame UV height offset:	1/rows
		float currTexWidth = 0;		// Current width offset
		float currTexHeight = 0;	// Current height offset
	};

	struct Mesh {
		TEX_TYPE tex;
		AEGfxVertexList* mesh;
	};

	void Initialize();
	void Update();
	float GetTexWidth(const TEX_TYPE& type);
	float GetTexHeight(const TEX_TYPE& type);
	AEGfxVertexList* GetMesh(const TEX_TYPE& type);
	AEGfxTexture* GetTexture(const TEX_TYPE& type);
}

