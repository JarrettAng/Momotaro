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

	void FontManager::Initialize() {
		InitializeFont();
	}

	void InitializeFont() {
		roboto.type = ROBOTO;
		roboto.XS = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 10);
		roboto.S = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 20);
		roboto.M = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 40);
		roboto.L = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 60);
	}

	FONT GetFont(const FONT_TYPE& type) {
		switch (type)
		{
		case ROBOTO:
			return roboto;
		default:
			break;
		}
		// Default font.
		std::cout << "FONT TYPE INVALID, RETURN DEFAULT FONT.";
		return roboto;
	}
}
