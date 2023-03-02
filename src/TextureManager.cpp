/*!************************************************************************
\file TextureManager.cpp
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 18-02-2023
\brief
This source file declares

**************************************************************************/

#include <TextureManager.h>


namespace TextureManager {
	/*!***********************************************************************
	* FORWARD DECLARATION
	*************************************************************************/
	void LoadTextures();
	void InitializeTextures();
	void GenerateMesh();

	void UpdateTextureSheetAnimation();

	/*!***********************************************************************
	* VARIABLES
	*************************************************************************/
	std::vector<TextureSheet> textures;
	std::vector<Mesh> meshes;

	/*!***********************************************************************
	* TEXTURES
	*************************************************************************/
	AEGfxTexture* residential_S_Tex;
	AEGfxTexture* residential_M_Tex;
	AEGfxTexture* residential_L_Tex;

	AEGfxTexture* residential_1x2_S_Tex;
	AEGfxTexture* residential_1x2_M_Tex;
	AEGfxTexture* residential_1x2_L_Tex;

	AEGfxTexture* commercial_S_Tex;
	AEGfxTexture* commercial_M_Tex;
	AEGfxTexture* commercial_L_Tex;

	AEGfxTexture* industrial_S_Tex;
	AEGfxTexture* industrial_M_Tex;
	AEGfxTexture* industrial_L_Tex;

	AEGfxTexture* tile_Tex;
	AEGfxTexture* nature_Tex;
	AEGfxTexture* rock_Tex;

	AEGfxTexture* card_Tex;
	AEGfxTexture* pause_Tex;
	AEGfxTexture* pauseButton_Tex;

	//IN PROGRESS
	AEGfxTexture* splash_Tex;
	AEGfxTexture* synergy_Tex;
	AEGfxTexture* positiveSynergy_Tex;
	AEGfxTexture* negativeSynergy_Tex;

	TEX_TYPE operator++(TEX_TYPE& _texture, int)
	{
		if (_texture == BUILDING_TEXTURE_COUNT) {
			std::cerr << "WARNING " << __FILE__ << "ln" << __LINE__ << ": Texture values wrapped!! Reached max building texture count!\n";
		}
		_texture = (_texture == BUILDING_TEXTURE_COUNT) ?
			RESIDENTIAL_1X1_L1 :
			TEX_TYPE(static_cast<int>(_texture + 1));
		return _texture;
	}

	void Initialize()
	{
		LoadTextures();
		InitializeTextures();
		GenerateMesh();
	}

	/*!***********************************************************************
	\brief
		Load textures from file.
	*************************************************************************/
	void LoadTextures() {
		residential_S_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L1.png");
		residential_M_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L2.png");
		residential_L_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L3.png");

		residential_1x2_S_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/residential_1x2_test.png");
		residential_1x2_M_Tex = AEGfxTextureLoad("Assets/Textures/UI/BlueRect.png");
		residential_1x2_L_Tex = AEGfxTextureLoad("Assets/Textures/UI/BlueRect.png");

		commercial_S_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Commercial_1x1_L1.png");
		commercial_M_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Commercial_1x1_L2.png");
		commercial_L_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Commercial_1x1_L3.png");
		// commercial_S_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L1.png");
		// commercial_M_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L2.png");
		// commercial_L_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L3.png");

		industrial_S_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Industrial_1x1_L1.png");
		industrial_M_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Industrial_1x1_L2.png");
		industrial_L_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Industrial_1x1_L3.png");

		tile_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Tile.png");
		nature_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Nature_Tree.png");
		rock_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Nature_Rock.png");

		card_Tex = AEGfxTextureLoad("Assets/Textures/UI/Card.png");
		pause_Tex = AEGfxTextureLoad("Assets/Textures/UI/GameWindow.png");
		pauseButton_Tex = AEGfxTextureLoad("Assets/Textures/UI/Pause_Button.png");

		//IN PROGRESS
		splash_Tex = AEGfxTextureLoad("Assets/Textures/UI/SplashScreen.png");
		synergy_Tex = AEGfxTextureLoad("Assets/Textures/UI/synergyTex.png");
		positiveSynergy_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/PositiveSynergy.png");
		negativeSynergy_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/NegativeSynergy.png");
	}

	/*!***********************************************************************
	\brief
		Initialize texture sheets and properties for animations.
	*************************************************************************/
	void InitializeTextures() {
		// TODO: READ FROM JSON
		// TO GET: ROWS, COLS AND FRAME INTERVAL, 
		textures.push_back(TextureSheet{ NONE, 1, 1,-1 });

		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L1, 1, 6, .2 });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L2, 1, 6, .2 });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L3, 3, 6, .2 });

		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L1, 1, 1, -1 });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L2, 1, 1, -1 });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L3, 1, 1, -1 });

		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L1, 4, 6, .15 });
		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L2, 1, 1, -1 });
		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L3, 1, 1, -1 });

		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L1, 3, 9, .2 });
		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L2,	3, 9, .2 });
		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L3, 1, 27, .2 });

		textures.push_back(TextureSheet{ TILE_TEX, 1, 2, .7 });
		textures.push_back(TextureSheet{ NATURE_TREE, 1, 1, -1 });
		textures.push_back(TextureSheet{ NATURE_ROCK, 1, 1, -1 });

		textures.push_back(TextureSheet{ CARD_BLUE, 1, 1, -1 });
		textures.push_back(TextureSheet{ PAUSE_WINDOW, 1, 1, -1 });
		textures.push_back(TextureSheet{ PAUSE_BUTTON, 1, 1, -1 });
		textures.push_back(TextureSheet{ SPLASH_SCREEN, 1, 1, -1 });

		textures.push_back(TextureSheet{ POSITIVE_SYNERGY, 1, 1, -1 });
		textures.push_back(TextureSheet{ NEGATIVE_SYNERGY, 1, 1, -1 });

		// Initialize frame UV size based on number of cols and rows.
		for (TextureSheet& t : textures) {
			t.texWidth = 1.0 / t.cols;
			t.texHeight = 1.0 / t.rows;
			t.currInterval = t.frameInterval;
		}
	}

	/*!***********************************************************************
	\brief
		Generate mesh for each textures, mesh UV uses height and width offsets for animating.
	*************************************************************************/
	void GenerateMesh() {
		// Generate mesh for each texture sheet.
		for (TextureSheet& t : textures) {
			AEGfxMeshStart();
			// I'm sorry.
			if (t.tex == INDUSTRIAL_1X1_L3) {
				AEGfxTriAdd(0.0f, 0.35f, 0xFFFFFFFF, 0.0f, 0.0f,
					0.0f, -0.75f, 0xFFFFFFFF, 0.0f, t.texHeight,
					1.0f, -0.75f, 0xFFFFFFFF, t.texWidth, t.texHeight);
				AEGfxTriAdd(1.0f, -0.75f, 0xFFFFFFFF, t.texWidth, t.texHeight,
					1.0f, 0.35f, 0xFFFFFFFF, t.texWidth, 0.0f,
					0.0f, 0.35f, 0xFFFFFFFF, 0.0f, 0.0f);
				meshes.push_back({ t.tex,AEGfxMeshEnd() });
				continue;
			}


			// UVS ARE BASED ON NUMBER OF ROWS AND COLS FOR ANIMATIONS.
			AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
				0.0f, -1.0f, 0xFFFFFFFF, 0.0f, t.texHeight,
				1.0f, -1.0f, 0xFFFFFFFF, t.texWidth, t.texHeight);
			AEGfxTriAdd(1.0f, -1.0f, 0xFFFFFFFF, t.texWidth, t.texHeight,
				1.0f, 0.0f, 0xFFFFFFFF, t.texWidth, 0.0f,
				0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
			meshes.push_back({ t.tex,AEGfxMeshEnd() });
		}
	}

	void TextureManager::Update() {
		UpdateTextureSheetAnimation();
	}

	/*!***********************************************************************
	\brief
		Update current UV offset in texture sheet.
	*************************************************************************/
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
				t.currTexWidth += t.texWidth;
				// End of row
				if (t.currTexWidth >= 1) {
					// Reset X
					t.currTexWidth = 0;
					// Go down Y
					t.currTexHeight += t.texHeight;
				}
				// End of Y
				if (t.currTexHeight >= 1) {
					// Back to the top of spritesheet.
					t.currTexHeight = 0;
				}
				// Reset timer.
				t.currInterval = t.frameInterval;
			}
		}
	}

	/*!***********************************************************************
	\brief
		Get texture width offset.
	*************************************************************************/
	float GetTexWidth(const TEX_TYPE& type) {

		for (TextureSheet& t : textures) {
			if (t.tex == type) {
				return t.currTexWidth;
			}
		}
		std::cerr << "ERROR " << __FILE__ << "ln" << __LINE__ << ": WRONG TEXTURE WIDTH DUMMY!!\n";
		return 0;
	}

	/*!***********************************************************************
	\brief
		Get texture height offset.
	*************************************************************************/
	float GetTexHeight(const TEX_TYPE& type) {

		for (TextureSheet& t : textures) {
			if (t.tex == type) {
				return t.currTexHeight;
			}
		}
		std::cerr << "ERROR " << __FILE__ << "ln" << __LINE__ << ": WRONG TEXTURE TYPE DUMMY!!\n";
		return 0;
	}

	/*!***********************************************************************
	\brief
		Get mesh from texture type.
	*************************************************************************/
	AEGfxVertexList* GetMesh(const TEX_TYPE& type) {
		for (Mesh& t : meshes) {
			if (t.tex == type) {
				return t.mesh;
			}
		}
		std::cerr << "ERROR " << __FILE__ << "ln" << __LINE__ << ": WRONG TEXTURE MESH DUMMY!!\n";
		return nullptr;
	}

	/*!***********************************************************************
	\brief
		Get texture from texture type.
	*************************************************************************/
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
			return residential_1x2_M_Tex;
		case RESIDENTIAL_1X2_L3:
			return residential_1x2_L_Tex;
		case CARD_BLUE:
			return card_Tex;
		case PAUSE_WINDOW:
			return pause_Tex;
		case PAUSE_BUTTON:
			return pauseButton_Tex;
		case SPLASH_SCREEN:
			return splash_Tex;
		case SYNERGY:
			return synergy_Tex;
		case POSITIVE_SYNERGY:
			return positiveSynergy_Tex;
		case NEGATIVE_SYNERGY:
			return negativeSynergy_Tex;
		default:
			break;
		}
		std::cout << "INVALID SPRITE TYPE ( " << type << " ) WHEN CALLING GetTexture()\n";
		return nullptr;
	}
}