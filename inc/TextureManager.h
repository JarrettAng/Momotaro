#pragma once
#include <AEEngine.h>

namespace TextureManager {

	enum TEX_TYPE {
		TILE_TEX,
		NATURE_TREE,
		COMMERCIAL_S,
		COMMERCIAL_M,
		COMMERCIAL_L,
		RESIDENTIAL_S,
		RESIDENTIAL_M,
		RESIDENTIAL_L,
		CARD_BLUE,
		PAUSE_WINDOW,
	};
	void Initialize();
	AEGfxTexture* GetTexure(const TEX_TYPE& type);
}

