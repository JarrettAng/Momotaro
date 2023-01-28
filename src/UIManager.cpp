#include "UIManager.h"
#include "RenderSystem.h"

s8 robotoFont;

void UIManager::Initialize() {
	robotoFont = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 20);
}

void UIManager::RenderText(FONT_TYPE font, const float& x, const float& y, std::string text, const Vec3<float>& color, const float& scale)
{
	RenderSystem::TextData data;

	// Initialize text data.
	data.fontID = GetFontID(font);
	data.x = x / AEGetWindowWidth();
	data.y = y / AEGetWindowHeight();
	data.text = text;
	data.color = color;
	data.scale = scale;

	// Add text data to text batch queue in render system.
	RenderSystem::AddTextBatch(data);
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
}



