#pragma once
#include <AEEngine.h>

namespace TextureManager {

	enum TEX_TYPE {
		RESIDENTIAL_1X1_L1 = 0,
		RESIDENTIAL_1X1_L2,
		RESIDENTIAL_1X1_L3,
		COMMERCIAL_1X1_L1 = 3,
		COMMERCIAL_1X1_L2,
		COMMERCIAL_1X1_L3,
		INDUSTRIAL_1X1_L1 = 6,
		INDUSTRIAL_1X1_L2,
		INDUSTRIAL_1X1_L3,
		TILE_TEX,
		NATURE_TREE,
		CARD_BLUE,
		PAUSE_WINDOW,
	};
	void Initialize();
	AEGfxTexture* GetTexture(const TEX_TYPE& type);
}

