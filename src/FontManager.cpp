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
	std::vector<FONT> fonts;

	void FontManager::Initialize() {
		InitializeFont();
	}

	void InitializeFont() {
		fonts.push_back({ ROBOTO,  AEGfxCreateFont("Assets/Fonts/Roboto-Regular.ttf", (int)DEFAULT_FONT_SIZE) });
	}

	s8 GetFont(const FONT_TYPE& type) {
		for (const FONT& f : fonts) {
			if (f.type == type) {
				return f.font;
			}
		}
		// Default font.
		std::cerr << "Error : " << __FILE__ << " ln" << __LINE__ << " Font type invalid, returning font type roboto." << std::endl;
		return roboto.font;
	}
}
