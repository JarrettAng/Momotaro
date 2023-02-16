#include "UIManager.h"
#include "RenderSystem.h"
#include <iostream>
#include <string>
#include <InputManager.h>
#include <TextureManager.h>
#include <PauseManager.h>
#include <SceneManager.h>


namespace UIManager {
	/*!***********************************************************************
	* FONTS
	*************************************************************************/
	FONT roboto;
	/*************************************************************************/

	/*!***********************************************************************
	* TEXT VARIABLE CACHE
	*************************************************************************/
	f32 textWidth, textHeight;
	/*************************************************************************/

	/*!***********************************************************************
	* FUNCTION FORWARD DECLARATIONS
	*************************************************************************/
	void InitializeFont();
	void AddButtonToBatch(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, AEGfxTexture* tex, const Vec3<float>& txtColor, const Vec4<float>& btnColor);
	/*************************************************************************/

	/*!***********************************************************************
	* HELPER FUNCTIONS FOR CREATING UI DATA.
	*************************************************************************/
	void TransformDataToUIData(UIData& data, const float& x, const float& y, const float& width, const float& height);
	void GraphicsDataToUIData(UIData& data, AEGfxTexture* tex, const Vec4<float>& color);
	void TextDataToUIData(UIData& data, const s8& font, const float& x, const float& y, std::string text, const Vec3<float>& color, const float& scale);
	/*************************************************************************/

	/*!***********************************************************************
	* HELPER FUNCTIONS FOR BUTTON.
	*************************************************************************/
	AEVec2 GetButtonSize(const float& xPadding, const float& yPadding);
	AEVec2 GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& textWidth, const float& textHeight);
	/*************************************************************************/


	void Initialize() {
		InitializeFont();
	}

	void InitializeFont() {
		roboto.type = ROBOTO;
		roboto.S = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 20);
		roboto.M = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 40);
		roboto.L = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 60);
	}

	void AddTextToBatch(const s8& font, const float& x, const float& y, const int& layer, std::string text, const Vec3<float>& color)
	{
		UIData data;
		// Order of rendering UI.
		data.layer = layer;

		// Initialize text data.
		TextDataToUIData(data, font, x, y, text, color, 1);

		// Add text data to UI batch queue in render system.
		RenderSystem::AddUIBatch(data);
	}

	void AddRectToBatch(const float& x, const float& y, const float& width, const float& height, const int& layer, const Vec4<float>& color, AEGfxTexture* tex)
	{
		UIData data;
		// Order of rendering UI.
		data.layer = layer;
		/*!***********************************************************************
		* Initialize rect data.
		*************************************************************************/
		TransformDataToUIData(data, x, y, width, height);

		/*!***********************************************************************
		* Initialize graphics data.
		*************************************************************************/
		GraphicsDataToUIData(data, tex, color);

		// Add rect data to UI batch queue in render system.
		RenderSystem::AddUIBatch(data);
	}

	void AddButtonToBatch(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, AEGfxTexture* tex, const Vec3<float>& txtColor, const Vec4<float>& btnColor) {

		UIData data;
		// Order of rendering UI.
		data.layer = layer;

		// Get width and height to position text in middle of rect.
		AEGfxGetPrintSize(font, const_cast<char*>(text.c_str()), 1, textWidth, textHeight);

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
		AEVec2 textPos = RenderSystem::GetPivotPos(GetCenteredTextPos(x, y, buttonSize.x, buttonSize.y, textWidth, textHeight), buttonSize.x / AEGetWindowWidth() * 2, buttonSize.y / AEGetWindowHeight() * 2);
		TextDataToUIData(data, font, textPos.x, textPos.y, text, txtColor, 1);

		// Add button data to UI batch queue in render system.
		RenderSystem::AddUIBatch(data);
	}

	/*!***********************************************************************
	* OVERLOADED RENDER BUTTON FUNCS
	*************************************************************************/

	// Button with TEXTURE + text.
	void AddButtonToBatch(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, AEGfxTexture* tex, const Vec3<float>& txtColor) {
		AddButtonToBatch(x, y, xPadding, yPadding, layer, font, text, tex, txtColor, Vec4<float>{});
	}

	// Button with COLOR + text.
	void AddButtonToBatch(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, const Vec4<float>& btnColor, const Vec3<float>& txtColor) {
		AddButtonToBatch(x, y, xPadding, yPadding, layer, font, text, nullptr, txtColor, btnColor);
	}
	/*************************************************************************/

	/*!***********************************************************************
	* OVERLOADED RENDER RECT FUNCS
	*************************************************************************/
	// Rect with TEXTURE.
	void AddRectToBatch(const float& x, const float& y, const float& width, const float& height, const int& layer, AEGfxTexture* tex) {
		AddRectToBatch(x, y, width, height, layer, Vec4<float>{}, tex);
	}
	// Rect with COLOR.
	void AddRectToBatch(const float& x, const float& y, const float& width, const float& height, const int& layer, const Vec4<float>& btnColor) {
		AddRectToBatch(x, y, width, height, layer, btnColor, nullptr);
	}
	/*************************************************************************/

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

	void GraphicsDataToUIData(UIData& data, AEGfxTexture* tex, const Vec4<float>& color) {
		data.graphics.hasGraphics = true;
		if (tex) {
			data.graphics.tex = tex;
		}
		else {
			data.graphics.color = color;
		}
	}

	void TextDataToUIData(UIData& data, const s8& font, const float& x, const float& y, std::string text, const Vec3<float>& color, const float& scale) {
		data.text.hasText = true;
		data.text.fontID = font;
		data.text.x = x;
		data.text.y = y;
		data.text.text = text;
		data.text.color = color;
		data.text.scale = scale;
	}
}



