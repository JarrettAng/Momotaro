/*!************************************************************************
\file FontManager.cpp
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 18-02-2023
\brief
This source file declares

**************************************************************************/

#include <FontManager.h>

namespace FontManager {
	void InitializeFont();

	/*!***********************************************************************
	* FONTS
	*************************************************************************/
	FONT roboto;

	// Stores fonts.
	std::vector<FONT> fonts;

	void FontManager::Initialize() {
		InitializeFont();
	}

	/*!***********************************************************************
	\brief
		Add fonts to vector.
	*************************************************************************/
	void InitializeFont() {
		fonts.push_back({ ROBOTO,  AEGfxCreateFont("Assets/Fonts/Roboto-Regular.ttf", (int)DEFAULT_FONT_SIZE) });
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
		std::cerr << "Error : " << __FILE__ << " ln" << __LINE__ << " Font type invalid, returning font type roboto." << std::endl;
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
