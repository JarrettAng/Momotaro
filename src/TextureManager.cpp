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
	void InitializeTextures();
	void GenerateMesh();

	void UpdateTextureSheetAnimation();

	/*!***********************************************************************
	* VARIABLES
	*************************************************************************/
	std::vector<TextureSheet> textures;
	std::vector<Mesh> meshes;

	void Initialize() {
		InitializeTextures();
		GenerateMesh();
	}

	/*!***********************************************************************
	\brief
		Initialize texture sheets and properties for animations.
	*************************************************************************/
	void InitializeTextures() {
		// TODO: READ FROM JSON
		// TO GET: ROWS, COLS AND FRAME INTERVAL, 
		textures.push_back(TextureSheet{ NONE,nullptr, 1, 1,-1 });

		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L1, AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L1.png"),  1, 6, .2f });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L2, AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L2.png"), 1, 6, .2f });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X1_L3,AEGfxTextureLoad("Assets/Textures/Game Pieces/Residential_1x1_L3.png"), 3, 6, .2f });

		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L1,AEGfxTextureLoad("Assets/Textures/Game Pieces/residential_1x2_test.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L2, AEGfxTextureLoad("Assets/Textures/UI/BlueRect.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ RESIDENTIAL_1X2_L3,AEGfxTextureLoad("Assets/Textures/UI/BlueRect.png"), 1, 1, -1.f });

		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L1,AEGfxTextureLoad("Assets/Textures/Game Pieces/Commercial_1x1_L1.png"), 4, 6, .2f });
		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L2,AEGfxTextureLoad("Assets/Textures/Game Pieces/Commercial_1x1_L2.png"), 4, 6, .2f });
		textures.push_back(TextureSheet{ COMMERCIAL_1X1_L3,AEGfxTextureLoad("Assets/Textures/Game Pieces/Commercial_1x1_L3.png"), 4, 6, .2f });

		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L1,AEGfxTextureLoad("Assets/Textures/Game Pieces/Industrial_1x1_L1.png"), 3, 9, .2f });
		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L2,AEGfxTextureLoad("Assets/Textures/Game Pieces/Industrial_1x1_L2.png"), 3, 9, .2f });
		textures.push_back(TextureSheet{ INDUSTRIAL_1X1_L3,AEGfxTextureLoad("Assets/Textures/Game Pieces/Industrial_1x1_L3.png"), 1, 27, .2f });

		textures.push_back(TextureSheet{ TILE_TEX, AEGfxTextureLoad("Assets/Textures/Game Pieces/Tile.png"), 1, 2, .7f });
		textures.push_back(TextureSheet{ WATER_TEX, AEGfxTextureLoad("Assets/Textures/Game Pieces/Water.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ NATURE_POND, AEGfxTextureLoad("Assets/Textures/Game Pieces/Nature_Pond.png"),1, 2, 1.f });
		textures.push_back(TextureSheet{ NATURE_MUSHROOM,AEGfxTextureLoad("Assets/Textures/Game Pieces/Nature_Mushroom.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ NATURE_ROCK, AEGfxTextureLoad("Assets/Textures/Game Pieces/Nature_Rock.png"),1, 1, -1.f });

		textures.push_back(TextureSheet{ POINTER,AEGfxTextureLoad("Assets/Textures/UI/Buttons/Pointer.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ STARTGAME_BTN,AEGfxTextureLoad("Assets/Textures/UI/Buttons/StartGame.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ EDITOR_BTN,AEGfxTextureLoad("Assets/Textures/UI/Buttons/Editor.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ OPTIONS_BTN,AEGfxTextureLoad("Assets/Textures/UI/Buttons/Options.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ CONTROLS_BTN,AEGfxTextureLoad("Assets/Textures/UI/Buttons/Controls.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ CREDITS_BTN,AEGfxTextureLoad("Assets/Textures/UI/Buttons/Credits.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ QUIT_BTN, AEGfxTextureLoad("Assets/Textures/UI/Buttons/Quit.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ YES_BTN,AEGfxTextureLoad("Assets/Textures/UI/Buttons/Yes_Btn.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ NO_BTN,AEGfxTextureLoad("Assets/Textures/UI/Buttons/No_Btn.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ BACK_BTN, AEGfxTextureLoad("Assets/Textures/UI/Buttons/Back_Btn.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ PAUSE_BUTTON,AEGfxTextureLoad("Assets/Textures/UI/Buttons/Pause_Btn.png"), 1, 1, -1.f });

		textures.push_back(TextureSheet{ MENU_BG,AEGfxTextureLoad("Assets/Textures/UI/Backgrounds/MainMenu_Background.png"), 1, 13, .1f });
		textures.push_back(TextureSheet{ CREDITS_BG, AEGfxTextureLoad("Assets/Textures/UI/Backgrounds/Credit_Background.png"), 1, 1, -1.f });

		textures.push_back(TextureSheet{ BLANK_PROMPT ,AEGfxTextureLoad("Assets/Textures/UI/Prompts/Blank_Prompt.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ CONFIRM_PROMPT,AEGfxTextureLoad("Assets/Textures/UI/Prompts/Confirmation_Prompt.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ PAUSE_WINDOW,AEGfxTextureLoad("Assets/Textures/UI/Prompts/Paused_Prompt.png"), 1, 1, -1.f });

		textures.push_back(TextureSheet{ CARD,AEGfxTextureLoad("Assets/Textures/UI/Card.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ CARD_COUNT,AEGfxTextureLoad("Assets/Textures/UI/CardCount_Icon.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ SYNERGY,AEGfxTextureLoad("Assets/Textures/Game Pieces/Synergy.png"), 1, 1, -1.f });

		textures.push_back(TextureSheet{ DIGIPEN_LOGO,AEGfxTextureLoad("Assets/Textures/UI/Digipen_Logo.png"), 1, 1, -1.f });

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

		for (TextureSheet& t : textures) {
			// Cant unload nullptr. So skip.
			if (t.type == NONE) continue;

			AEGfxTextureUnload(t.tex);
		}
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