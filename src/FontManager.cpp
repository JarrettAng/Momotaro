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
