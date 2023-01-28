#include "UIManager.h"
#include "RenderSystem.h"
namespace UIManager {


	s8 GetFontID(const FONT_TYPE& type);

	s8 robotoFont;

	void UIManager::Initialize() {
		robotoFont = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 20);
	}

	void UIManager::RenderText(const FONT_TYPE& font, const float& x, const float& y, std::string text, const Vec3<float>& color, const float& scale)
	{
		UIData data;
		// Initialize text data.
		data.text.hasText = true;
		data.text.fontID = GetFontID(font);
		data.text.x = x / AEGetWindowWidth();
		data.text.y = y / AEGetWindowHeight();
		data.text.text = text;
		data.text.color = color;
		data.text.scale = scale;

		// Add text data to text batch queue in render system.
		RenderSystem::AddUIBatch(data);
	}

	void UIManager::RenderButton(const float& x, const float& y, const float& width, const float& height, FONT_TYPE font, std::string text, const int& layer, AEGfxTexture* tex, const Vec3<float>& txtColor, const Vec3<float>& btnColor) {

		UIData data;
		data.graphics.hasGraphics = true;
		data.text.hasText = true;
		data.layer = layer;

		// Initialize rect data.
		data.transform.x = x;
		data.transform.y = y;
		data.transform.width = width;
		data.transform.height = height;

		// Initialize graphics data.
		if (tex) {
			data.graphics.tex = tex;
		}
		else {
			data.graphics.color = btnColor;
		}

		// Initialize text data.
		data.text.fontID = GetFontID(font);
		data.text.x = x / AEGetWindowWidth();
		data.text.y = y / AEGetWindowHeight();
		data.text.text = text;
		data.text.color = txtColor;
		data.text.scale = 1;

		RenderSystem::AddUIBatch(data);
	}

	s8 UIManager::GetFontID(const FONT_TYPE& type) {
		switch (type)
		{
		case ROBOTO:
			return robotoFont;
			break;
		default:
			break;
		}
		// Default font.
		return robotoFont;
	}

}



