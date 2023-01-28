#include "UIManager.h"
#include "RenderSystem.h"
#include <iostream>



namespace UIManager {

	/*!***********************************************************************
	* DECLARATIONS.
	*************************************************************************/

	//Individual Card Variables
	struct card {
		float x;
		float y;
		bool clicked = false;
	}; card c1, c2, c3, c4, c5;

	float init_x = -500, init_y = -300;


	//Shared Card variables
	float hand_x, hand_y, c_height, c_width;

	//Cursor Var (s32 is an int)
	int cursor_x, cursor_y;

	s8 robotoFont_s;
	s8 robotoFont_m;
	f32 textWidth, textHeight;

	s8 GetFontID(const FONT_TYPE& type);

	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const FONT_TYPE& font, const std::string& text, const int& layer, AEGfxTexture* tex, const Vec3<float>& txtColor, const Vec3<float>& btnColor);

	/*!***********************************************************************
	* HELPER FUNCTIONS FOR CREATING UI DATA.
	*************************************************************************/
	void TransformDataToUIData(UIData& data, const float& x, const float& y, const float& width, const float& height);
	void GraphicsDataToUIData(UIData& data, AEGfxTexture* tex, const Vec4<float>& color);
	void TextDataToUIData(UIData& data, const FONT_TYPE& font, const float& x, const float& y, std::string text, const Vec3<float>& color, const float& scale);

	/*!***********************************************************************
	* HELPER FUNCTIONS FOR BUTTON.
	*************************************************************************/
	AEVec2 GetButtonSize(const float& xPadding, const float& yPadding);
	AEVec2 GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& textWidth, const float& textHeight);


	void Initialize() {
		robotoFont_s = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 20);
		robotoFont_m = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 40);

		//Default location of card deck
		//hand_x = (AEGfxGetWinMaxX() - AEGfxGetWinMinX()) * (2.5f / 8.0f);
		hand_x = AEGfxGetWinMinX() + 100.0f;
		hand_y = (AEGfxGetWinMaxY() - AEGfxGetWinMinY()) * (3.0f / 8.0f);

		//Mesh is a square
		c_height = 40.0f;
		c_width = 40.0f;


		//TEMP--------------------------------------------------------
		// World x and y is different from cursor x and y
		// world (signed (+-) windowsize/2), cursor (window size) 
		// ex. -300 to +300 in engine is 0 to 600 on window
		c1.x = init_x;
		c1.y = init_y;

		c2.x = init_x - 50;
		c2.y = init_y;

		c3.x = init_x - 100;
		c3.y = init_y;

		c4.x = init_x - 150;
		c4.y = init_y;

		c5.x = init_x - 200;
		c5.y = init_y;
	}

	void RenderText(const FONT_TYPE& font, const float& x, const float& y, std::string text, const Vec3<float>& color)
	{
		// Initialize text data.
		UIData data;
		TextDataToUIData(data, font, x, y, text, color, 1);

		// Add text data to text batch queue in render system.
		RenderSystem::AddUIBatch(data);

	}

	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const FONT_TYPE& font, const std::string& text, const int& layer, AEGfxTexture* tex, const Vec3<float>& txtColor, const Vec4<float>& btnColor) {

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
	// Button with TEXTURE + NO text.
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, AEGfxTexture* tex = nullptr) {
		RenderButton(x, y, xPadding, yPadding, NONE, {}, layer, tex, Vec3<float>{}, Vec4<float>{});
	}
	// Button with COLOR + NO text.
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const Vec4<float>& btnColor) {
		RenderButton(x, y, xPadding, yPadding, NONE, {}, layer, nullptr, Vec3<float>{}, btnColor);
	}
	// Button with TEXTURE + text.
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const FONT_TYPE& font, const std::string& text, AEGfxTexture* tex, const Vec3<float>& txtColor) {
		RenderButton(x, y, xPadding, yPadding, font, text, layer, tex, txtColor, Vec4<float>{});
	}
	// Button with COLOR + text.
	void RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const FONT_TYPE& font, const std::string& text, const Vec4<float>& btnColor, const Vec3<float>& txtColor) {
		RenderButton(x, y, xPadding, yPadding, font, text, layer, nullptr, txtColor, btnColor);
	}
	/*************************************************************************/

	s8 GetFontID(const FONT_TYPE& type) {
		switch (type)
		{
		case ROBOTO_S:
			return robotoFont_s;
			break;
		case ROBOTO_M:
			return robotoFont_m;
			break;
		default:
			break;
		}
		// Default font.
		std::cout << "FONT TYPE INVALID, RETURN DEFAULT FONT.";
		return robotoFont_s;
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

	void TextDataToUIData(UIData& data, const FONT_TYPE& font, const float& x, const float& y, std::string text, const Vec3<float>& color, const float& scale) {
		data.text.hasText = true;
		data.text.fontID = GetFontID(font);
		data.text.x = x;
		data.text.y = y;
		data.text.text = text;
		data.text.color = color;
		data.text.scale = scale;
	}


	/*!***********************************************************************
	* Render the cards on the screen & allow the player to move them
	*************************************************************************/
	void UIManager::PrepareUIRenderBatch() {

		//Return s32 == int
		AEInputGetCursorPosition(&cursor_x, &cursor_y);
		//Change cursor to float
		static_cast<double>(cursor_x);
		static_cast<double>(cursor_y);

		//RMD: Make into ONE function in future
		//Card1
		{
			//If clicked...
			if (AEInputCheckPrev(AEVK_LBUTTON)) {

				//within card mesh
				if (-cursor_y + 480.0f > c1.y - c_height && -cursor_y + 480.0f < c1.y
					&& cursor_x - 830.0f > c1.x && cursor_x - 830.0f < c1.x + c_width)
				{
					c1.clicked = true;
				}
			}
			else { c1.clicked = false; }

			//Card 1 check
			if (!c2.clicked && !c3.clicked && !c4.clicked && !c5.clicked) {
				if (AEInputCheckCurr(AEVK_LBUTTON) && c1.clicked == true) {
					c1.x = cursor_x - 800.0f - c_width;
					c1.y = -cursor_y + 450.0f + c_height;
					//if(AEInputCheckReleased(AEVK_LBUTTON))
				}
				else {
					//Default locations
					c1.x = init_x;
					c1.y = init_y;
				}
			}

			RenderSystem::AddBatch(RenderSystem::UI_BATCH, RenderSystem::UI, c1.x, c1.y);
		}

		//Card2
		{

			//If clicked...
			if (AEInputCheckPrev(AEVK_LBUTTON)) {

				//within card mesh
				if (-cursor_y + 480.0f > c2.y - c_height && -cursor_y + 480.0f < c2.y
					&& cursor_x - 830.0f > c2.x && cursor_x - 830.0f < c2.x + c_width)
				{
					c2.clicked = true;
				}
			}
			else { c2.clicked = false; }

			//Card 2 check
			if (!c1.clicked && !c3.clicked && !c4.clicked && !c5.clicked) {
				if (AEInputCheckCurr(AEVK_LBUTTON) && c2.clicked == true) {
					c2.x = cursor_x - 800.0f - c_width;
					c2.y = -cursor_y + 450.0f + c_height;
					//if(AEInputCheckReleased(AEVK_LBUTTON))
				}
				else {
					//Default locations
					c2.x = init_x - 50;
					c2.y = init_y;
				}
			}

			RenderSystem::AddBatch(RenderSystem::UI_BATCH, RenderSystem::UI, c2.x, c2.y);
		}

		//Card3
		{

			//If clicked...
			if (AEInputCheckPrev(AEVK_LBUTTON)) {

				//within card mesh
				if (-cursor_y + 480.0f > c3.y - c_height && -cursor_y + 480.0f < c3.y
					&& cursor_x - 830.0f > c3.x && cursor_x - 830.0f < c3.x + c_width)
				{
					c3.clicked = true;
				}
			}
			else { c3.clicked = false; }

			//Card 3 check
			if (!c1.clicked && !c2.clicked && !c4.clicked && !c5.clicked) {
				if (AEInputCheckCurr(AEVK_LBUTTON) && c3.clicked == true) {
					c3.x = cursor_x - 800.0f - c_width;
					c3.y = -cursor_y + 450.0f + c_height;
					//if(AEInputCheckReleased(AEVK_LBUTTON))
				}
				else {
					//Default locations
					c3.x = init_x - 100;
					c3.y = init_y;
				}
			}


			RenderSystem::AddBatch(RenderSystem::UI_BATCH, RenderSystem::UI, c3.x, c3.y);
		}

		//Card4
		{

			//If clicked...
			if (AEInputCheckPrev(AEVK_LBUTTON)) {

				//within card mesh
				if (-cursor_y + 480.0f > c4.y - c_height && -cursor_y + 480.0f < c4.y
					&& cursor_x - 830.0f > c4.x && cursor_x - 830.0f < c4.x + c_width)
				{
					c4.clicked = true;
				}
			}
			else { c4.clicked = false; }

			//Card 4 check
			if (!c1.clicked && !c2.clicked && !c3.clicked && !c5.clicked) {
				if (AEInputCheckCurr(AEVK_LBUTTON) && c4.clicked == true) {
					c4.x = cursor_x - 800.0f - c_width;
					c4.y = -cursor_y + 450.0f + c_height;
					//if(AEInputCheckReleased(AEVK_LBUTTON))
				}
				else {
					//Default locations
					c4.x = init_x - 150;
					c4.y = init_y;
				}
			}

			RenderSystem::AddBatch(RenderSystem::UI_BATCH, RenderSystem::UI, c4.x, c4.y);
		}

		//Card5
		{

			//If clicked...
			if (AEInputCheckPrev(AEVK_LBUTTON)) {

				//within card mesh
				if (-cursor_y + 480.0f > c5.y - c_height && -cursor_y + 480.0f < c5.y
					&& cursor_x - 830.0f > c5.x && cursor_x - 830.0f < c5.x + c_width)
				{
					c5.clicked = true;
				}
			}
			else { c5.clicked = false; }

			//Card 5 check
			if (!c1.clicked && !c2.clicked && !c3.clicked && !c4.clicked) {
				if (AEInputCheckCurr(AEVK_LBUTTON) && c5.clicked == true) {
					c5.x = cursor_x - 800.0f - c_width;
					c5.y = -cursor_y + 450.0f + c_height;
					//if(AEInputCheckReleased(AEVK_LBUTTON))
				}
				else {
					//Default locations
					c5.x = init_x - 200;
					c5.y = init_y;
				}
			}

			RenderSystem::AddBatch(RenderSystem::UI_BATCH, RenderSystem::UI, c5.x, c5.y);
		}

	}
}



