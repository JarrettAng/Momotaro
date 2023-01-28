#include "UIManager.h"
#include "RenderSystem.h"
#include <iostream>
namespace UIManager {

	s8 robotoFont;
	f32 textWidth, textHeight;

	s8 GetFontID(const FONT_TYPE& type);

	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const FONT_TYPE& font, const std::string& text, const int& layer, AEGfxTexture* tex, const Vec3<float>& txtColor, const Vec3<float>& btnColor);

	/*!***********************************************************************
	* HELPER FUNCTIONS FOR CREATING UI DATA.
	*************************************************************************/
	void TransformDataToUIData(UIData& data, const float& x, const float& y, const float& width, const float& height);
	void GraphicsDataToUIData(UIData& data, AEGfxTexture* tex, const Vec3<float>& color);
	void TextDataToUIData(UIData& data, const FONT_TYPE& font, const float& x, const float& y, std::string text, const Vec3<float>& color, const float& scale);

	/*!***********************************************************************
	* HELPER FUNCTIONS FOR BUTTON.
	*************************************************************************/
	AEVec2 GetButtonSize(const float& xPadding, const float& yPadding);
	AEVec2 GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& textWidth, const float& textHeight);

	void Initialize() {
		robotoFont = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 20);
	}

	void RenderText(const FONT_TYPE& font, const float& x, const float& y, std::string text, const Vec3<float>& color, const float& scale)
	{
		// Initialize text data.
		UIData data;
		TextDataToUIData(data, font, x, y, text, color, scale);

		// Add text data to text batch queue in render system.
		RenderSystem::AddUIBatch(data);
	}

	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const FONT_TYPE& font, const std::string& text, const int& layer, AEGfxTexture* tex, const Vec3<float>& txtColor, const Vec3<float>& btnColor) {

		UIData data;
		// Order of rendering UI.
		data.layer = layer;

		// Get width and height to position text in middle of rect.
		AEGfxGetPrintSize(GetFontID(font), const_cast<char*>(text.c_str()), 1, textWidth, textHeight);

		/*!***********************************************************************
		* Initialize rect data.
		*************************************************************************/
		// Set width and height based on text size.
		AEVec2 buttonSize = GetButtonSize(xPadding, yPadding);
		TransformDataToUIData(data, x, y, buttonSize.x, buttonSize.y);

		/*!***********************************************************************
		* Initialize graphics data.
		*************************************************************************/
		GraphicsDataToUIData(data, tex, btnColor);

		/*!***********************************************************************
		* Initialize text data.
		*************************************************************************/
		// Position text to middle of rect.
		AEVec2 textPos = GetCenteredTextPos(x, y, buttonSize.x, buttonSize.y, textWidth, textHeight);
		TextDataToUIData(data, font, textPos.x, textPos.y, text, txtColor, 1);

		RenderSystem::AddUIBatch(data);
	}

	/*!***********************************************************************
	* OVERLOADED RENDER BUTTON FUNCS
	*************************************************************************/
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, AEGfxTexture* tex = nullptr) {
		RenderButton(x, y, xPadding, yPadding, NONE, {}, layer, tex, Vec3<float>{}, Vec3<float>{});
	}
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const Vec3<float>& btnColor) {
		RenderButton(x, y, xPadding, yPadding, NONE, {}, layer, nullptr, Vec3<float>{}, btnColor);
	}
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const FONT_TYPE& font, const std::string& text, AEGfxTexture* tex, const Vec3<float>& txtColor) {
		RenderButton(x, y, xPadding, yPadding, font, text, layer, tex, txtColor, Vec3<float>{});
	}
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const FONT_TYPE& font, const std::string& text, const Vec3<float>& btnColor, const Vec3<float>& txtColor) {
		RenderButton(x, y, xPadding, yPadding, font, text, layer, nullptr, txtColor, btnColor);
	}
	/*************************************************************************/

	s8 GetFontID(const FONT_TYPE& type) {
		switch (type)
		{
		case ROBOTO:
			return robotoFont;
			break;
		default:
			break;
		}
		// Default font.
		std::cout << "FONT TYPE INVALID, RETURN DEFAULT FONT.";
		return robotoFont;
	}

	AEVec2 GetButtonSize(const float& xPadding, const float& yPadding) {
		// Get button's width and height by adding text width/height with padding given.
		return AEVec2{ textWidth * AEGetWindowWidth() / 2 + xPadding * 2 , textHeight * AEGetWindowHeight() / 2 + yPadding * 2 };
	}

	AEVec2 GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& textWidth, const float& textHeight) {
		// It just works
		return AEVec2{ ((x / AEGetWindowWidth()) * 2) + ((((width / AEGetWindowWidth()) * 2) - textWidth) / 2) , ((y / AEGetWindowHeight()) * 2) - ((height / 2) / AEGetWindowHeight()) * 2 - (textHeight / 2) };
	}

	void TransformDataToUIData(UIData& data, const float& x, const float& y, const float& width, const float& height) {
		data.transform.x = x;
		data.transform.y = y;
		data.transform.width = width;
		data.transform.height = height;
	}

	void GraphicsDataToUIData(UIData& data, AEGfxTexture* tex, const Vec3<float>& color) {
		data.graphics.hasGraphics = true;
		if (tex) {
			data.graphics.tex = tex;
		}
		else {
			data.graphics.color = color;
		}
	}

	void TextDataToUIData(UIData& data, const FONT_TYPE& font, const float& x, const float& y, std::string text, const Vec3<float>& color, const float& scale) {
		data.text.hasText = true;
		data.text.fontID = GetFontID(font);
		data.text.x = x;
		data.text.y = y;
		data.text.text = text;
		data.text.color = color;
		data.text.scale = scale;
	}

}



