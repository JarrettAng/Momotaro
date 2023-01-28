#pragma once
#include <AEEngine.h>
#include <MomoMaths.h>
#include <string>

namespace UIManager {

	enum FONT_TYPE {
		ROBOTO,
	};

	void Initialize();
	void RenderText(FONT_TYPE font, const float& x, const float& y, std::string text, const Vec3<float>& color = { 1.0f,1.0f,1.0f }, const float& scale = 1);
	s8 GetFontID(const FONT_TYPE& type);
}

