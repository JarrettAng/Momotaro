/*!************************************************************************
\file			TextureManager.cpp
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			18-02-2023
\brief
This source file load and store textures ofthe game. It also animates them.

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <TextureManager.h>
#include <DebugManager.h>
///////////////////////////////////////////////////////////////////////////

namespace TextureManager {
	/*!***********************************************************************
	* Forward Declarations.
	*************************************************************************/
	void InitializeTextures();
	void GenerateMesh();
	void UpdateTextureSheetAnimation();

	/*!***********************************************************************
	* Textures & mesh cache.
	*************************************************************************/
	std::vector<TextureSheet> textures;
	std::vector<Mesh> meshes;

	/*!***********************************************************************
	\brief
		Initialize TextureManager.
	*************************************************************************/
	void Initialize() {
		InitializeTextures();
		GenerateMesh();
	}

	/*!***********************************************************************
	\brief
		Initialize texture sheets and properties for animations.
	*************************************************************************/
	void InitializeTextures() {
		// ROW, COL, FRAME DELAY, ANIMATION DELAY.
		textures.push_back(TextureSheet{ NONE, nullptr, 1, 1,-1 });

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

		textures.push_back(TextureSheet{ TILE_TEX, AEGfxTextureLoad("Assets/Textures/Game Pieces/Tile.png"), 1, 2, 1.5f });
		textures.push_back(TextureSheet{ WATER_TEX, AEGfxTextureLoad("Assets/Textures/Game Pieces/Water.png"), 1, 2, 1.5f });
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
		textures.push_back(TextureSheet{ RESUME_BTN, AEGfxTextureLoad("Assets/Textures/UI/Buttons/Resume_Btn.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ PAUSE_BUTTON,AEGfxTextureLoad("Assets/Textures/UI/Buttons/Pause_Btn.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ RESTART_BUTTON,AEGfxTextureLoad("Assets/Textures/UI/Buttons/Restart_Btn.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ RETRY_BTN,AEGfxTextureLoad("Assets/Textures/UI/Buttons/Retry_Btn.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ SHOWHIDE_BTN,AEGfxTextureLoad("Assets/Textures/UI/Buttons/ShowHide_Btn.png"), 1, 1, -1.f });

		textures.push_back(TextureSheet{ MENU_BG,AEGfxTextureLoad("Assets/Textures/UI/Backgrounds/MainMenu_Background.png"), 1, 13, .1f, 4.0f });
		textures.push_back(TextureSheet{ CLOUD_BG,AEGfxTextureLoad("Assets/Textures/UI/Backgrounds/MainMenu_Clouds.png"), 1, 42, 0.15f });
		textures.push_back(TextureSheet{ SKY_BG,AEGfxTextureLoad("Assets/Textures/UI/Backgrounds/MainMenu_Sky.png"), 1, 1, -1.f });

		textures.push_back(TextureSheet{ CREDITS_BG, AEGfxTextureLoad("Assets/Textures/UI/Backgrounds/Credit_Background.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ GAMEOVER_BG, AEGfxTextureLoad("Assets/Textures/UI/Backgrounds/GameOver_Background.png"), 1, 8, .05f, 4.0f });

		textures.push_back(TextureSheet{ BLANK_PROMPT ,AEGfxTextureLoad("Assets/Textures/UI/Prompts/Blank_Prompt.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ CONFIRM_PROMPT,AEGfxTextureLoad("Assets/Textures/UI/Prompts/Confirmation_Prompt.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ PAUSE_WINDOW,AEGfxTextureLoad("Assets/Textures/UI/Prompts/Paused_Prompt.png"), 1, 1, -1.f });

		textures.push_back(TextureSheet{ CARD,AEGfxTextureLoad("Assets/Textures/UI/Card.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ CARD_COUNT,AEGfxTextureLoad("Assets/Textures/UI/CardCount_Icon.png"), 1, 1, -1.f });
		textures.push_back(TextureSheet{ SYNERGY,AEGfxTextureLoad("Assets/Textures/Game Pieces/Synergy.png"), 1, 1, -1.f });

		textures.push_back(TextureSheet{ MOMO_THE_PEACH,AEGfxTextureLoad("Assets/Textures/UI/MomoThePeach.png"), 1, 1, -1.f });

		textures.push_back(TextureSheet{ DIGIPEN_LOGO,AEGfxTextureLoad("Assets/Textures/UI/Digipen_Logo.png"), 1, 1, -1.f });

		// Initialize frame UV size based on number of cols and rows.
		for (TextureSheet& t : textures) {
			t.texWidth = 1.f / t.cols;
			t.texHeight = 1.f / t.rows;
			t.currFrameDelay = t.frameDelay;
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

	/*!***********************************************************************
	\brief
		Update TextureManager.
	*************************************************************************/
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
			if (t.frameDelay == -1) continue;

			// Tick animation timer.
			if (t.currAnimeDelay > 0) {
				t.currAnimeDelay -= (f32)AEFrameRateControllerGetFrameTime();
				continue;
			}

			// Tick frame timer.
			if (t.currFrameDelay > 0) {
				t.currFrameDelay -= (f32)AEFrameRateControllerGetFrameTime();
				continue;
			}

			// Goto next anim frame.
			if (t.currFrameDelay <= 0) {
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

					// End of animation loop.
					t.currAnimeDelay = t.animDelay;
				}
				// Reset timer.
				t.currFrameDelay = t.frameDelay;
			}
		}
	}

	/*!***********************************************************************
	\brief
		Get texture width offset.
	\param type
		Texture type.
	\return
		Width offset.
	*************************************************************************/
	float GetTexWidth(const TEX_TYPE& type) {

		for (TextureSheet& t : textures) {
			if (t.type == type) {
				return t.currTexWidth;
			}
		}

		Debug::PrintError(__FILE__, __LINE__, "WRONG TEXTURE WIDTH DUMMY!!");
		return 0;
	}

	/*!***********************************************************************
	\brief
		Get texture height offset.
	\param type
		Texture type.
	\return
		Height offset.
	*************************************************************************/
	float GetTexHeight(const TEX_TYPE& type) {

		for (TextureSheet& t : textures) {
			if (t.type == type) {
				return t.currTexHeight;
			}
		}
		Debug::PrintError(__FILE__, __LINE__, "WRONG TEXTURE TYPE DUMMY!!");
		return 0;
	}

	/*!***********************************************************************
	\brief
		Get mesh from texture type.
	\param type
		Texture type.
	\return
		Mesh.
	*************************************************************************/
	AEGfxVertexList* GetMesh(const TEX_TYPE& type) {
		for (Mesh& t : meshes) {
			if (t.tex == type) {
				return t.mesh;
			}
		}
		Debug::PrintError(__FILE__, __LINE__, "WRONG TEXTURE MESH DUMMY!!");
		return nullptr;
	}

	/*!***********************************************************************
	\brief
		Get texture from texture type.
	\param type
		Texture type.
	\return
		Texture.
	*************************************************************************/
	AEGfxTexture* GetTexture(const TEX_TYPE& type) {
		for (TextureSheet& t : textures) {
			if (t.type == type) {
				return t.tex;
			}
		}

		Debug::PrintError(__FILE__, __LINE__, "INVALID SPRITE TYPE ( " + std::to_string(type) + " ) WHEN CALLING GetTexture()");
		return nullptr;
	}

	/*!***********************************************************************
	\brief
		Unload TextureManager.
	*************************************************************************/
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
	\param _texture
		Texture type.
	\return
		Texture type.
	*************************************************************************/
	TEX_TYPE operator++(TEX_TYPE& _texture, int)
	{
		if (_texture == BUILDING_TEXTURE_COUNT) {
			Debug::PrintError(__FILE__, __LINE__, "Texture values wrapped!! Reached max building texture count!");
		}
		_texture = (_texture == BUILDING_TEXTURE_COUNT) ?
			RESIDENTIAL_1X1_L1 :
			TEX_TYPE(static_cast<int>(_texture + 1));
		return _texture;
	}
}
