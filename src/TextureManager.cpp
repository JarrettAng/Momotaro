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

	// BUILDINGS.
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

	// NATURES.
	AEGfxTexture* pond_Tex;
	AEGfxTexture* mushroom_Tex;
	AEGfxTexture* rock_Tex;

	// NORMAL TILE.
	AEGfxTexture* tile_Tex;

	// UI.
	AEGfxTexture* card_Tex;
	AEGfxTexture* synergy_Tex;

	AEGfxTexture* pause_Tex;
	AEGfxTexture* pauseButton_Tex;

	// SPLASH SCREEN.
	AEGfxTexture* digipen_Logo_Tex;

	// MAIN MENU.
	AEGfxTexture* background_Tex;
	AEGfxTexture* pointer_Tex;

	AEGfxTexture* startGame_Tex;
	AEGfxTexture* controls_Tex;
	AEGfxTexture* options_Tex;
	AEGfxTexture* editor_Tex;
	AEGfxTexture* credits_Tex;
	AEGfxTexture* quit_Tex;

	AEGfxTexture* confirm_Prompt_Tex;
	AEGfxTexture* yes_Tex;
	AEGfxTexture* no_Tex;

	void Initialize() {
		LoadTextures();
		InitializeTextures();
		GenerateMesh();
	}

	/*!***********************************************************************
	\brief
		Load textures from file.
	*************************************************************************/
	void LoadTextures() {
		// BUILDINGS.
		residential_S_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L1.png");
		residential_M_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L2.png");
		residential_L_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L3.png");

		residential_1x2_S_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/residential_1x2_test.png");
		residential_1x2_M_Tex = AEGfxTextureLoad("Assets/Textures/UI/BlueRect.png");
		residential_1x2_L_Tex = AEGfxTextureLoad("Assets/Textures/UI/BlueRect.png");

		commercial_S_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Commercial_1x1_L1.png");
		commercial_M_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Commercial_1x1_L2.png");
		commercial_L_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Commercial_1x1_L3.png");

		industrial_S_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Industrial_1x1_L1.png");
		industrial_M_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Industrial_1x1_L2.png");
		industrial_L_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Industrial_1x1_L3.png");

		// NORMAL TILE.
		tile_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Tile.png");

		// NATURE.
		pond_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Nature_Pond.png");
		mushroom_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Nature_Mushroom.png");
		rock_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Nature_Rock.png");

		// UI.
		synergy_Tex = AEGfxTextureLoad("Assets/Textures/Game Pieces/Synergy.png");
		card_Tex = AEGfxTextureLoad("Assets/Textures/UI/Card.png");
		pause_Tex = AEGfxTextureLoad("Assets/Textures/UI/GameWindow.png");
		pauseButton_Tex = AEGfxTextureLoad("Assets/Textures/UI/Pause_Button.png");

		// SPLASH SCREEN.
		digipen_Logo_Tex = AEGfxTextureLoad("Assets/Textures/UI/Digipen_Logo.png");

		// MAIN MENU.
		background_Tex = AEGfxTextureLoad("Assets/Textures/UI/Main Menu/Background.png");
		pointer_Tex = AEGfxTextureLoad("Assets/Textures/UI/Main Menu/Pointer.png");

		startGame_Tex = AEGfxTextureLoad("Assets/Textures/UI/Main Menu/StartGame.png");
		editor_Tex = AEGfxTextureLoad("Assets/Textures/UI/Main Menu/Editor.png");
		options_Tex = AEGfxTextureLoad("Assets/Textures/UI/Main Menu/Options.png");
		controls_Tex = AEGfxTextureLoad("Assets/Textures/UI/Main Menu/Controls.png");
		credits_Tex = AEGfxTextureLoad("Assets/Textures/UI/Main Menu/Credits.png");
		quit_Tex = AEGfxTextureLoad("Assets/Textures/UI/Main Menu/Quit.png");

		confirm_Prompt_Tex = AEGfxTextureLoad("Assets/Textures/UI/Confirmation_Prompt.png");
		yes_Tex = AEGfxTextureLoad("Assets/Textures/UI/Yes_Btn.png");
		no_Tex = AEGfxTextureLoad("Assets/Textures/UI/No_Btn.png");
	}

	/*!***********************************************************************
	\brief
		Initialize texture sheets and properties for animations.
	*************************************************************************/
	void InitializeTextures() {
		// TODO: READ FROM JSON
		// TO GET: ROWS, COLS AND FRAME INTERVAL, 
		textures.push_back(TextureSheet{ NONE,nullptr, 1, 1,-1 });

		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L1,residential_S_Tex,  1, 6, .2f });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L2,residential_M_Tex, 1, 6, .2f });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L3,residential_L_Tex, 3, 6, .2f });

		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L1,residential_1x2_S_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L2,residential_1x2_M_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L3,residential_1x2_L_Tex, 1, 1, -1.f });

		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L1,commercial_S_Tex, 4, 6, .2f });
		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L2,commercial_M_Tex, 4, 6, .2f });
		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L3,commercial_L_Tex, 4, 6, .2f });

		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L1,industrial_S_Tex, 3, 9, .2f });
		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L2,industrial_M_Tex,	3, 9, .2f });
		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L3,industrial_L_Tex, 1, 27, .2f });

		textures.push_back(TextureSheet{ TILE_TEX,tile_Tex, 1, 2, .7f });
		textures.push_back(TextureSheet{ NATURE_POND, pond_Tex,1, 2, 1.f });
		textures.push_back(TextureSheet{ NATURE_MUSHROOM,mushroom_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ NATURE_ROCK, rock_Tex,1, 1, -1.f });

		textures.push_back(TextureSheet{ CARD_BLUE,card_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ PAUSE_WINDOW,pause_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ PAUSE_BUTTON,pauseButton_Tex, 1, 1, -1.f });

		textures.push_back(TextureSheet{ SYNERGY,synergy_Tex, 1, 1, -1.f });

		textures.push_back(TextureSheet{ DIGIPEN_LOGO,digipen_Logo_Tex, 1, 1, -1.f });

		textures.push_back(TextureSheet{ MENU_BG,background_Tex, 1, 13, .1f });
		textures.push_back(TextureSheet{ POINTER,pointer_Tex, 1, 1, -1.f });

		textures.push_back(TextureSheet{ STARTGAME_BTN,startGame_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ EDITOR_BTN,editor_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ OPTIONS_BTN,options_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ CONTROLS_BTN,controls_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ CREDITS_BTN,credits_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ QUIT_BTN,quit_Tex, 1, 1, -1.f });

		textures.push_back(TextureSheet{ CONFIRM_PROMPT,confirm_Prompt_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ YES_BTN,yes_Tex, 1, 1, -1.f });
		textures.push_back(TextureSheet{ NO_BTN,no_Tex, 1, 1, -1.f });

		// Initialize frame UV size based on number of cols and rows.
		for (TextureSheet& t : textures) {
			t.texWidth = 1.f / t.cols;
			t.texHeight = 1.f / t.rows;
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
			if (t.type == INDUSTRIAL_1X1_L3) {
				AEGfxTriAdd(0.0f, 0.35f, 0xFFFFFFFF, 0.0f, 0.0f,
					0.0f, -0.75f, 0xFFFFFFFF, 0.0f, t.texHeight,
					1.0f, -0.75f, 0xFFFFFFFF, t.texWidth, t.texHeight);
				AEGfxTriAdd(1.0f, -0.75f, 0xFFFFFFFF, t.texWidth, t.texHeight,
					1.0f, 0.35f, 0xFFFFFFFF, t.texWidth, 0.0f,
					0.0f, 0.35f, 0xFFFFFFFF, 0.0f, 0.0f);
				meshes.push_back({ t.type,AEGfxMeshEnd() });
				continue;
			}

			// UVS ARE BASED ON NUMBER OF ROWS AND COLS FOR ANIMATIONS.
			AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
				0.0f, -1.0f, 0xFFFFFFFF, 0.0f, t.texHeight,
				1.0f, -1.0f, 0xFFFFFFFF, t.texWidth, t.texHeight);
			AEGfxTriAdd(1.0f, -1.0f, 0xFFFFFFFF, t.texWidth, t.texHeight,
				1.0f, 0.0f, 0xFFFFFFFF, t.texWidth, 0.0f,
				0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
			meshes.push_back({ t.type,AEGfxMeshEnd() });
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
				t.currInterval -= (f32)AEFrameRateControllerGetFrameTime();
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
			if (t.type == type) {
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
			if (t.type == type) {
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
		for (TextureSheet& t : textures) {
			if (t.type == type) {
				return t.tex;
			}
		}

		std::cout << "INVALID SPRITE TYPE ( " << type << " ) WHEN CALLING GetTexture()\n";
		return nullptr;
	}

	void TextureManager::Unload() {
		for (Mesh& m : meshes) {
			AEGfxMeshFree(m.mesh);
		}

		// Access violation error?
		//for (TextureSheet& t : textures) {
		//	AEGfxTextureUnload(t.tex);
		//}

		AEGfxTextureUnload(residential_S_Tex);
		AEGfxTextureUnload(residential_M_Tex);
		AEGfxTextureUnload(residential_L_Tex);

		AEGfxTextureUnload(residential_1x2_S_Tex);
		AEGfxTextureUnload(residential_1x2_M_Tex);
		AEGfxTextureUnload(residential_1x2_L_Tex);

		AEGfxTextureUnload(commercial_S_Tex);
		AEGfxTextureUnload(commercial_M_Tex);
		AEGfxTextureUnload(commercial_L_Tex);

		AEGfxTextureUnload(industrial_S_Tex);
		AEGfxTextureUnload(industrial_M_Tex);
		AEGfxTextureUnload(industrial_L_Tex);

		AEGfxTextureUnload(tile_Tex);
		AEGfxTextureUnload(pond_Tex);
		AEGfxTextureUnload(mushroom_Tex);
		AEGfxTextureUnload(rock_Tex);

		AEGfxTextureUnload(synergy_Tex);
		AEGfxTextureUnload(card_Tex);
		AEGfxTextureUnload(pause_Tex);
		AEGfxTextureUnload(pauseButton_Tex);

		AEGfxTextureUnload(digipen_Logo_Tex);

		AEGfxTextureUnload(background_Tex);
		AEGfxTextureUnload(pointer_Tex);

		AEGfxTextureUnload(startGame_Tex);
		AEGfxTextureUnload(editor_Tex);
		AEGfxTextureUnload(options_Tex);
		AEGfxTextureUnload(controls_Tex);
		AEGfxTextureUnload(credits_Tex);
		AEGfxTextureUnload(quit_Tex);

		AEGfxTextureUnload(confirm_Prompt_Tex);
		AEGfxTextureUnload(yes_Tex);
		AEGfxTextureUnload(no_Tex);
	}

	/*!***********************************************************************
	\brief
		Increment operator overload to get next enum.
	*************************************************************************/
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
}