/*!************************************************************************
\file			TextureManager.h
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			18-01-2023
\brief
This header file contains prototype functions and declarations for TextureManager.cpp.

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <AEEngine.h>
#include <iostream>
#include <vector>
///////////////////////////////////////////////////////////////////////////

/*!***********************************************************************
\brief
	Type of textures in the game.
*************************************************************************/
namespace TextureManager {
	enum TEX_TYPE {
		NONE = 0,
		RESIDENTIAL_1X1_L1 = 1,
		RESIDENTIAL_1X1_L2,
		RESIDENTIAL_1X1_L3,
		RESIDENTIAL_1X2_L1 = 4,
		RESIDENTIAL_1X2_L2,
		RESIDENTIAL_1X2_L3,
		COMMERCIAL_1X1_L1 = 7,
		COMMERCIAL_1X1_L2,
		COMMERCIAL_1X1_L3,
		INDUSTRIAL_1X1_L1 = 10,
		INDUSTRIAL_1X1_L2,
		INDUSTRIAL_1X1_L3,
		BUILDING_TEXTURE_COUNT,
		TILE_TEX,
		NATURE_POND,
		NATURE_MUSHROOM,
		NATURE_ROCK,
		CARD,
		CARD_COUNT,
		PAUSE_WINDOW,
		PAUSE_BUTTON,
		RESTART_BUTTON,
		DIGIPEN_LOGO,
		SYNERGY,
		MENU_BG,
		POINTER,
		STARTGAME_BTN,
		EDITOR_BTN,
		OPTIONS_BTN,
		CONTROLS_BTN,
		CREDITS_BTN,
		QUIT_BTN,
		CONFIRM_PROMPT,
		YES_BTN,
		NO_BTN,
		CREDITS_BG,
		BACK_BTN,
		RESUME_BTN,
		BLANK_PROMPT,
		WATER_TEX,
		MOMO_THE_PEACH,
		CLOUD_BG,
		SKY_BG,
		RETRY_BTN,
		SHOWHIDE_BTN,
		GAMEOVER_BG
	};
	TEX_TYPE operator++(TEX_TYPE&, int);

	/*!***********************************************************************
	\brief
		Texture sheet object that also contains animation information.
	*************************************************************************/
	struct TextureSheet {
		TEX_TYPE type;				// Type of texture.
		AEGfxTexture* tex;			// Texture.

		int	rows;					// Number of rows in tex sheet.
		int cols;					// Number of columns in tex sheet.

		// Animation.
		float frameDelay;			// Time between each frame.
		float animDelay = 0;		// Time between animation loop. 0 = No delay between animation loop.

		float currFrameDelay = 0;
		float currAnimeDelay = 0;

		float texWidth;				// Texture frame UV width offset:	1/cols
		float texHeight;			// Texture frame UV height offset:	1/rows
		float currTexWidth = 0;		// Current width offset
		float currTexHeight = 0;	// Current height offset
	};

	/*!***********************************************************************
	\brief
		Mesh object to be use for rendering.
	*************************************************************************/
	struct Mesh {
		TEX_TYPE tex;
		AEGfxVertexList* mesh;
	};

	/*!***********************************************************************
	\brief
		Initialize TextureManager.
	*************************************************************************/
	void Initialize();

	/*!***********************************************************************
	\brief
		Update TextureManager.
	*************************************************************************/
	void Update();

	/*!***********************************************************************
	\brief
		Unload TextureManager.
	*************************************************************************/
	void Unload();

	/*!***********************************************************************
	\brief
		Get texture width offset.
	*************************************************************************/
	float GetTexWidth(const TEX_TYPE& type);

	/*!***********************************************************************
	\brief
		Get texture height offset.
	*************************************************************************/
	float GetTexHeight(const TEX_TYPE& type);

	/*!***********************************************************************
	\brief
		Get mesh from texture type.
	*************************************************************************/
	AEGfxVertexList* GetMesh(const TEX_TYPE& type);

	/*!***********************************************************************
	\brief
		Get texture from texture type.
	*************************************************************************/
	AEGfxTexture* GetTexture(const TEX_TYPE& type);
}
