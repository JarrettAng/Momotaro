/*!************************************************************************
\file			FontManager.cpp
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			18-02-2023
\brief
This source file initializes and store fonts in the game.

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <FontManager.h>
#include <DebugManager.h>
///////////////////////////////////////////////////////////////////////////

namespace FontManager {
	/*!***********************************************************************
	* Forward Declarations.
	*************************************************************************/
	void InitializeFont();

	/*!***********************************************************************
	* Fonts
	*************************************************************************/
	FONT roboto;
	FONT shin_go;

	std::vector<FONT> fonts;

	/*!***********************************************************************
	\brief
		Initialize font manager.
	*************************************************************************/
	void FontManager::Initialize() {
		// Initialize fonts.
		InitializeFont();
	}

	/*!***********************************************************************
	\brief
		Add fonts to vector.
	*************************************************************************/
	void InitializeFont() {
		fonts.push_back({ ROBOTO,  AEGfxCreateFont("Assets/Fonts/RobotoMono-Regular.ttf", (int)DEFAULT_FONT_SIZE) });
		fonts.push_back({ SHIN_GO,  AEGfxCreateFont("Assets/Fonts/Shin-Go.otf", (int)DEFAULT_FONT_SIZE) });
	}

	/*!***********************************************************************
	\brief
		Get font based on font type.
	*************************************************************************/
	s8 GetFont(const FONT_TYPE& type) {
		// Loop through all fonts.
		for (const FONT& f : fonts) {
			// Return font if found.
			if (f.type == type) {
				return f.font;
			}
		}
		// Default font.
		Debug::PrintError(__FILE__, __LINE__, "Font type invalid, returning font type roboto.");
		return roboto.font;
	}

	/*!***********************************************************************
	\brief
		Destory all font that is loaded.
	*************************************************************************/
	void FontManager::Unload() {
		for (FONT f : fonts) {
			AEGfxDestroyFont(f.font);
		}
	}
}
