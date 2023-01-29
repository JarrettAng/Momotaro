#pragma once
#include <AEEngine.h>

namespace TextureManager {

	enum TEX_TYPE {
		TILE_TEX,
		NATURE_TREE,
		RESIDENTIAL_S,
		RESIDENTIAL_M,
		RESIDENTIAL_L,
		CARD_BLUE,
		PAUSE_WINDOW,
	};
	void Initialize();
	AEGfxTexture* GetTexure(const TEX_TYPE& type);
}

