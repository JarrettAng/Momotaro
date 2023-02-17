#pragma once
#include <AEEngine.h>
#include <iostream>

namespace FontManager {
	enum FONT_TYPE {
		NONE,
		ROBOTO
	};

	struct FONT {
		FONT_TYPE type;
		s8 S;
		s8 M;
		s8 L;
	};

	void Initialize();
	FONT GetFont(const FONT_TYPE& type);
}
