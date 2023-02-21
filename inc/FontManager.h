#pragma once
#include <AEEngine.h>
#include <vector>
#include <iostream>

namespace FontManager {
	const float DEFAULT_FONT_SIZE = 100.0;	// Base font size to reference to when scaling down font size.

	enum FONT_TYPE {
		NONE,
		ROBOTO
	};

	struct FONT {
		FONT_TYPE type;
		s8 font;
	};

	void Initialize();
	s8 GetFont(const FONT_TYPE& type);
}
