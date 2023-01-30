/*!************************************************************************
\file:          TextureManager.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

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
	AEGfxTexture* rock_Tex;
	AEGfxTexture* residential_S_Tex;
	AEGfxTexture* residential_M_Tex;
	AEGfxTexture* residential_L_Tex;
	AEGfxTexture* commercial_S_Tex;
	AEGfxTexture* commercial_M_Tex;
	AEGfxTexture* commercial_L_Tex;
	AEGfxTexture* industrial_S_Tex;
	AEGfxTexture* industrial_M_Tex;
	AEGfxTexture* industrial_L_Tex;
	AEGfxTexture* card_Tex;
	AEGfxTexture* pause_Tex;
	AEGfxTexture* pauseButton_Tex;

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
		industrial_S_Tex = AEGfxTextureLoad("Assets/industrial_s_test.png");
		industrial_M_Tex = AEGfxTextureLoad("Assets/industrial_m_test.png");
		industrial_L_Tex = AEGfxTextureLoad("Assets/industrial_l_test.png");
		nature_Tex = AEGfxTextureLoad("Assets/tree_test.png");
		rock_Tex = AEGfxTextureLoad("Assets/rock_test.png");
		pause_Tex = AEGfxTextureLoad("Assets/GameWindow.png");
		pauseButton_Tex = AEGfxTextureLoad("Assets/Pause_Button.png");
	}

	AEGfxTexture* GetTexture(const TEX_TYPE& type) {
		switch (type)
		{
		case TILE_TEX:
			return tile_Tex;
		case NATURE_TREE:
			return nature_Tex;
		case NATURE_ROCK:
			return rock_Tex;
		case COMMERCIAL_1X1_L1:
			return commercial_S_Tex;
		case COMMERCIAL_1X1_L2:
			return commercial_M_Tex;
		case COMMERCIAL_1X1_L3:
			return commercial_L_Tex;
		case INDUSTRIAL_1X1_L1:
			return industrial_S_Tex;
		case INDUSTRIAL_1X1_L2:
			return industrial_M_Tex;
		case INDUSTRIAL_1X1_L3:
			return industrial_L_Tex;
		case RESIDENTIAL_1X1_L1:
			return residential_S_Tex;
		case RESIDENTIAL_1X1_L2:
			return residential_M_Tex;
		case RESIDENTIAL_1X1_L3:
			return residential_L_Tex;
		case CARD_BLUE:
			return card_Tex;
		case PAUSE_WINDOW:
			return pause_Tex;
		case PAUSE_BUTTON:
			return pauseButton_Tex;
		default:
			break;
		}
		std::cout << "INVALID SPRITE TYPE ( " << type << " ) WHEN CALLING GetTexture()\n";
		return nullptr;
	}
}