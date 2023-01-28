#include "UIManager.h"
#include "RenderSystem.h"
#include <iostream>



namespace UIManager {

	//Individual Card Variables
	struct card {
		float x;
		float y;
		bool clicked = false;
	}; card c1, c2, c3, c4, c5;

	float init_x = -500;
	float init_y = -300;


	//Shared Card variables
	float hand_x;
	float hand_y;
	float c_height;
	float c_width;

	//Cursor Var
	int cursor_x;
	int cursor_y;

	//char fontid;
	//AEGfxVertexList* pMesh = 0;

	char hello[] = { 'H','E','L','L','O', '\0' };

	s8 robotoFont;
	s8 GetFontID(const FONT_TYPE& type);

	f32 textWidth, textHeight;


	void UIManager::Initialize() {
		robotoFont = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 20);

		//Default location of card deck
			//hand_x = (AEGfxGetWinMaxX() - AEGfxGetWinMinX()) * (2.5f / 8.0f);
		hand_x = AEGfxGetWinMinX() + 100.0f;
		hand_y = (AEGfxGetWinMaxY() - AEGfxGetWinMinY()) * (3.0f / 8.0f);

		//Mesh is a square
		c_height = 40.0f;
		c_width = 40.0f;

		/*c1.x = hand_x * (1.0f / 5.0f);
		c2.x = hand_x * (2.0f / 5.0f);
		c3.x = hand_x * (3.0f / 5.0f);
		c4.x = hand_x * (4.0f / 5.0f);
		c5.x = hand_x;*/

		/*c1.y = -hand_y;
		c2.y = -hand_y;
		c3.y = -hand_y;
		c4.y = -hand_y;
		c5.y = -hand_y;*/

		//World x and y is different from cursor x and y
		// world (signed (+-) windowsize/2), cursor (window size) 
		// -300 to +300								0 to 600
		//TEMP--------------------------------------------------------
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

	void UIManager::RenderButton(const float& x, const float& y, const float& xPadding, const float& yPadding, FONT_TYPE font, std::string text, const int& layer, AEGfxTexture* tex, const Vec3<float>& txtColor, const Vec3<float>& btnColor) {

		UIData data;
		data.graphics.hasGraphics = true;
		data.text.hasText = true;
		data.layer = layer;

		// Get width and height to position text in middle of rect.
		AEGfxGetPrintSize(GetFontID(font), const_cast<char*>(text.c_str()), 1, textWidth, textHeight);

		// Initialize rect data.
		data.transform.x = x;
		data.transform.y = y;
		data.transform.width = textWidth * AEGetWindowWidth() / 2 + xPadding * 2;
		data.transform.height = textHeight * AEGetWindowHeight() / 2 + yPadding * 2;

		// Initialize graphics data.
		if (tex) {
			data.graphics.tex = tex;
		}
		else {
			data.graphics.color = btnColor;
		}

		// Initialize text data.
		data.text.fontID = GetFontID(font);

		// Position text to middle of rect.
		data.text.x = ((x / AEGetWindowWidth()) * 2) + ((((data.transform.width / AEGetWindowWidth()) * 2) - textWidth) / 2);
		data.text.y = ((y / AEGetWindowHeight()) * 2) - ((data.transform.height / 2) / AEGetWindowHeight()) * 2 - (textHeight / 2);

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


	void UIManager::PrepareUIRenderBatch() {

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



