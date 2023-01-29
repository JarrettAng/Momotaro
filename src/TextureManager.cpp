#include <AEEngine.h>
#include <TextureManager.h>
#include <iostream>

namespace TextureManager {

	/*!***********************************************************************
	* FORWARD DECLARATION
	*************************************************************************/
	void LoadTextures();

	/*!***********************************************************************
	* TEXTURES
	*************************************************************************/
	AEGfxTexture* tile_Tex;
	AEGfxTexture* nature_Tex;
	AEGfxTexture* residential_S_Tex;
	AEGfxTexture* residential_M_Tex;
	AEGfxTexture* residential_L_Tex;
	AEGfxTexture* commercial_S_Tex;
	AEGfxTexture* commercial_M_Tex;
	AEGfxTexture* commercial_L_Tex;
	AEGfxTexture* card_Tex;
	AEGfxTexture* pause_Tex;

	void Initialize() {
		LoadTextures();
	}

	void LoadTextures() {
		tile_Tex = AEGfxTextureLoad("Assets/Tile.png");
		card_Tex = AEGfxTextureLoad("Assets/Card.png");
		residential_S_Tex = AEGfxTextureLoad("Assets/residential_s_test.png");
		residential_M_Tex = AEGfxTextureLoad("Assets/residential_m_test.png");
		residential_L_Tex = AEGfxTextureLoad("Assets/residential_l_test.png");
		commercial_S_Tex = AEGfxTextureLoad("Assets/commercial_s_test.png");
		commercial_M_Tex = AEGfxTextureLoad("Assets/commercial_m_test.png");
		commercial_L_Tex = AEGfxTextureLoad("Assets/commercial_l_test.png");
		nature_Tex = AEGfxTextureLoad("Assets/tree_test.png");
		pause_Tex = AEGfxTextureLoad("Assets/GameWindow.png");
	}

	AEGfxTexture* GetTexure(const TEX_TYPE& type) {
		switch (type)
		{
		case TILE_TEX:
			return tile_Tex;
		case NATURE_TREE:
			return nature_Tex;
		case COMMERCIAL_S:
			return commercial_S_Tex;
		case COMMERCIAL_M:
			return commercial_M_Tex;
		case COMMERCIAL_L:
			return commercial_L_Tex;
		case RESIDENTIAL_S:
			return residential_S_Tex;
		case RESIDENTIAL_M:
			return residential_M_Tex;
		case RESIDENTIAL_L:
			return residential_L_Tex;
		case CARD_BLUE:
			return card_Tex;
		case PAUSE_WINDOW:
			return pause_Tex;
		default:
			break;
		}
		std::cout << "INVALID SPRITE TYPE WHEN CALLING GetSpriteTex().";
		return nullptr;
	}
}