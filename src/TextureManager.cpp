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

#include <TextureManager.h>
#include <iostream>
#include <vector>

namespace TextureManager {

	/*!***********************************************************************
	* FORWARD DECLARATION
	*************************************************************************/
	void LoadTextures();
	void UpdateTextureSheetAnimation();
	void GenerateMesh();

	std::vector<TextureSheet> textures;
	std::vector<AEGfxVertexList*> meshes;

	/*!***********************************************************************
	* TEXTURES
	*************************************************************************/
	AEGfxTexture* tile_Tex;
	AEGfxTexture* nature_Tex;
	AEGfxTexture* rock_Tex;
	AEGfxTexture* residential_S_Tex;
	AEGfxTexture* residential_1x2_S_Tex;
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
	AEGfxTexture* splash_Tex;

	void Initialize() {
		LoadTextures();
		GenerateMesh();
	}

	void LoadTextures() {
		tile_Tex = AEGfxTextureLoad("Assets/Tile.png");
		card_Tex = AEGfxTextureLoad("Assets/Card.png");
		splash_Tex = AEGfxTextureLoad("Assets/SplashScreen.png");
		residential_S_Tex = AEGfxTextureLoad("Assets/residential_s_test.png");
		// residential_1x2_S_Tex = AEGfxTextureLoad("Assets/residential_s_test.png");
		residential_1x2_S_Tex = AEGfxTextureLoad("Assets/residential_1x2_test.png");
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

		// TODO: READ FROM JSON
		// TO GET: ROWS, COLS AND FRAME INTERVAL, 
		textures.push_back(TextureSheet{ NONE, 1, 1,-1 });


		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L1, 1, 1, -1 });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L2, 1, 1, -1 });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L3, 1, 1, -1 });

		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L1, 1, 1, -1 });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L2, 1, 1, -1 });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L3, 1, 1, -1 });

		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L1, 1, 1, -1 });
		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L2, 1, 1, -1 });
		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L3, 1, 1, -1 });
		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L1, 1, 1, -1 });
		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L2, 1, 1, -1 });
		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L3, 1, 1, -1 });

		textures.push_back(TextureSheet{ TILE_TEX, 1, 2,.5 });
		textures.push_back(TextureSheet{ NATURE_TREE, 1, 1, -1 });
		textures.push_back(TextureSheet{ NATURE_ROCK, 1, 1, -1 });

		textures.push_back(TextureSheet{ CARD_BLUE, 1, 1, -1 });
		textures.push_back(TextureSheet{ PAUSE_WINDOW, 1, 1, -1 });
		textures.push_back(TextureSheet{ PAUSE_BUTTON, 1, 1, -1 });
		textures.push_back(TextureSheet{ SPLASH_SCREEN, 1, 1, -1 });


		for (TextureSheet& t : textures) {
			t.tw = 1.0 / t.cols;
			t.th = 1.0 / t.rows;
			t.currInterval = t.frameInterval;
		}
	}

	void TextureManager::Update() {
		UpdateTextureSheetAnimation();
	}

	// TEMP LOCATION
	void GenerateMesh() {
		for (TextureSheet& t : textures) {
			AEGfxMeshStart();
			// UVS ARE BASED ON NUMBER OF ROWS AND COLS FOR ANIMATIONS.
			AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
				0.0f, -1.0f, 0xFFFFFFFF, 0.0f, t.th,
				1.0f, -1.0f, 0xFFFFFFFF, t.tw, t.th);
			AEGfxTriAdd(1.0f, -1.0f, 0xFFFFFFFF, t.tw, t.th,
				1.0f, 0.0f, 0xFFFFFFFF, t.tw, 0.0f,
				0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
			meshes.push_back(AEGfxMeshEnd());
		}
	}

	void UpdateTextureSheetAnimation() {
		// Update animation.
		for (TextureSheet& t : textures) {
			// No animation.
			if (t.frameInterval == -1) continue;

			// Tick timer.
			if (t.currInterval > 0) {
				t.currInterval -= AEFrameRateControllerGetFrameTime();
				continue;
			}

			// Goto next anim frame.
			if (t.currInterval <= 0) {
				// Try to go to next frame.
				t.ctw += t.tw;
				// End of row
				if (t.ctw >= 1) {
					// Reset X
					t.ctw = 0;
					// Go down Y
					t.cth += t.th;
				}
				// End of Y
				if (t.cth >= 1) {
					// Back to the top of spritesheet.
					t.cth = 0;
				}
				// Reset timer.
				t.currInterval = t.frameInterval;
			}
		}
	}

	float GetTW(const TEX_TYPE& type) {
		for (TextureSheet& t : textures) {
			if (t.tex == type) {
				return t.ctw;
			}
		}
	}

	float GetTH(const TEX_TYPE& type) {
		for (TextureSheet& t : textures) {
			if (t.tex == type) {
				return t.cth;
			}
		}
	}

	AEGfxVertexList* GetMesh(const TEX_TYPE& type) {
		return meshes[type];
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
		case RESIDENTIAL_1X2_L1:
			return residential_1x2_S_Tex;
		case RESIDENTIAL_1X2_L2:
			return residential_1x2_S_Tex;
		case RESIDENTIAL_1X2_L3:
			return residential_1x2_S_Tex;
		case CARD_BLUE:
			return card_Tex;
		case PAUSE_WINDOW:
			return pause_Tex;
		case PAUSE_BUTTON:
			return pauseButton_Tex;
		case SPLASH_SCREEN:
			return splash_Tex;
		default:
			break;
		}
		std::cout << "INVALID SPRITE TYPE ( " << type << " ) WHEN CALLING GetTexture()\n";
		return nullptr;
	}
}