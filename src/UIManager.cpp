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
	* CARDS DECLARATIONS.
	*************************************************************************/
	//Individual Card Variables
	struct card {
		float x, y;
		bool clicked = false;
	}; card c1, c2, c3, c4, c5;

	float init_x = -500, init_y = -300;

	//Shared Card variables
	float hand_x, hand_y, c_height, c_width;

	//Cursor Var (s32 is an int)
	int cursor_x, cursor_y;

	// std::list<UIData> ListOfButtons; 
	std::vector<UIData> ListOfButtons;
	/*************************************************************************/


	/*!***********************************************************************
	* FONTS
	*************************************************************************/
	FONT roboto;
	/*************************************************************************/

	/*!***********************************************************************
	* TEXTURES
	*************************************************************************/
	AEGfxTexture* pauseTex;
	/*************************************************************************/

	/*!***********************************************************************
	* VARIABLE CACHE
	*************************************************************************/
	f32 textWidth, textHeight;
	/*************************************************************************/

	/*!***********************************************************************
	* FUNCTION FORWARD DECLARATIONS
	*************************************************************************/
	void InitializeFont();
	void OnMouseButtonclicked();
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
	UIData pauseButtonData{};
	UIData continueButtonData{};
	UIData exitButtonData{};

	void Initialize() {
		InitializeFont();

		//Default location of card deck
		//hand_x = (AEGfxGetWinMaxX() - AEGfxGetWinMinX()) * (2.5f / 8.0f);
		hand_x = AEGfxGetWinMinX() + 100.0f;
		hand_y = (AEGfxGetWinMaxY() - AEGfxGetWinMinY()) * (3.0f / 8.0f);

		//Mesh is a square
		c_height = 60.0f;
		c_width = 60.0f;


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
		//std::cout << "UIMANAGER INIT" << '\n';

		pauseButtonData.isActive = true;
		TransformDataToUIData(pauseButtonData, 700.0f, -425.0f, 70.0f, 70.0f);
		pauseButtonData.FunctionPtr = PauseManager::TogglePause;

		continueButtonData.isActive = false;
		TransformDataToUIData(continueButtonData, -250.0f, 50.0f, 250.0f, 100.0f);
		continueButtonData.FunctionPtr = PauseManager::TogglePause;
		continueButtonData.text.text = "CONTINUE";

		exitButtonData.isActive = false;
		TransformDataToUIData(exitButtonData, 50.0f, 50.0f, 250.0f, 100.0f);
		exitButtonData.FunctionPtr = quitgame;


		ListOfButtons.push_back(pauseButtonData);
		ListOfButtons.push_back(continueButtonData);
		ListOfButtons.push_back(exitButtonData);

		InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, OnMouseButtonclicked);
	}
	void quitgame() {
		SceneManager::LoadScene(SceneManager::QUIT);
	}
	void UpdateUI() {
		ListOfButtons[1].isActive = PauseManager::IsPaused();
		ListOfButtons[2].isActive = PauseManager::IsPaused();

		// continueButtonData.isActive = PauseManager::IsPaused();
		// exitButtonData.isActive = PauseManager::IsPaused();
		// std::cout << continueButtonData.isActive << '\n';
	}
	void OnMouseButtonclicked() {
		Vec2<int> mousePos = InputManager::GetMousePos();
		// std::cout << "Mouse has been clicked in UIMANAGER!" << '\n';
		// std::cout << "LIST  COUNT : " << ListOfButtons.size() << '\n';
		for (auto& button : ListOfButtons) {
			std::cout << "ButtonStats : " << button.text.text << '\n';
			std::cout << "Button Pos : " << button.transform.x << ", " << button.transform.y << '\n';
			std::cout << "Button Size : " << button.transform.width << ", " << button.transform.height << '\n';
			std::cout << "Mouse Pos : " << mousePos.x << ", " << mousePos.y << '\n';
			std::cout << "Mouse pos offset : " << (mousePos.x - AEGetWindowWidth() / 2) << ", " << (mousePos.y - AEGetWindowHeight() / 2) << '\n';
			std::cout << "Button pos offset : " << button.transform.y + button.transform.height << '\n';
			std::cout << "==================================================================" << '\n';
			//Left and right bounds (since mouse is offset by half screen size)
			if (((mousePos.x - AEGetWindowWidth() / 2) > button.transform.x) && (mousePos.x - AEGetWindowWidth() / 2 < button.transform.x + button.transform.width)) {
				std::cout << "Continue is " << continueButtonData.isActive << '\n';
				//Top and bottom bounds
				if (((mousePos.y - AEGetWindowHeight() / 2) > button.transform.y) && ((mousePos.y - AEGetWindowHeight() / 2) < button.transform.y + button.transform.height)) {
					std::cout << "BUTTON CLICKED : " << button.text.text << '\n';
					if (button.isActive) button.FunctionPtr();
					// PauseManager::TogglePause();
				}
			}
		}
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


	/*!***********************************************************************
	* Shows a pause UI when calling the pause function
	*************************************************************************/

	//TEMP---------------------------------------------
	//change to only rendering the screen when pause function is done

	void MakeText() {
		//PLACEHOLDER values
		int s = { 50 }, t = { 100 };
		std::string synergy = std::to_string(s);
		std::string treshold = std::to_string(t);


		//AddTextToBatch(roboto.M, ((-AEGfxGetWinMinX() + AEGfxGetWinMaxX()) * 0.0001f), 0.9f, 0, "SYNERGY", {(0.0f), (0.0f), (0.0f)});
		AddTextToBatch(roboto.M, -0.9f, 0.9f, 0, ("SYNERGY    " + synergy), { (0.0f), (0.0f), (0.0f) });
		//AddTextToBatch(roboto.M, -0.6f, 0.9f, 0, synergy,   { (0.0f), (0.0f), (0.0f) });
		AddTextToBatch(roboto.M, -0.55f, 0.9f, 0, ("/ " + treshold), { (0.0f), (0.0f), (0.0f) });

	}

	/*!***********************************************************************
	* UI that appers throughout ALL levels here
	*************************************************************************/

	void LevelUI() {
		//InputManager::onMouseClick.Subscribe(TogglePause);

		int x, y;
		AEInputGetCursorPosition(&x, &y);
		//bool isPaused;

		// EventSystem::Event<bool> onTogglePause;

		//TEMP---------------------------------------------------------------------
		// if (AEInputCheckTriggered(AEVK_LBUTTON) && (y <= 93 && y >= 23 && x <= 1568 && x >= 1502)) {
		// 	// std::cout << "PAUSE CLICKED\n";
		// 	//printf("%d.x, %d.y\n",x,y );
		// 	//InputManager::onEscPressed.Subscribe(TogglePause);
		// 	PauseManager::onTogglePause;
		// 	//onTogglePause.Invoke(isPaused);

		// }


		if (!PauseManager::IsPaused()) {
			UIManager::AddRectToBatch(700.0f, 425.0f, 70.0f, 70.0f, 0, TextureManager::GetTexture(TextureManager::PAUSE_BUTTON));
		}
		// UIManager::AddRectToBatch(700.0f, 425.0f, 70.0f, 70.0f, 0, TextureManager::GetTexture(TextureManager::PAUSE_BUTTON));

		//RenderSystem::AddSpriteBatch(RenderSystem::CARD_BATCH, RenderSystem::CARD, TextureManager::CARD_BLUE, c1.x, c1.y);

	}


	/*!***********************************************************************
	* Render the cards on the screen & allow the player to move them
	*************************************************************************/
	void UIManager::PrepareUIRenderBatch() {

		//Return s32 == int
		AEInputGetCursorPosition(&cursor_x, &cursor_y);
		//Change cursor to float
		//static_cast<double>(cursor_x);
		//static_cast<double>(cursor_y);

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

			RenderSystem::AddSpriteBatch(RenderSystem::CARD_BATCH, RenderSystem::CARD, TextureManager::CARD_BLUE, c1.x, c1.y);
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

			RenderSystem::AddSpriteBatch(RenderSystem::CARD_BATCH, RenderSystem::CARD, TextureManager::CARD_BLUE, c2.x, c2.y);
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


			RenderSystem::AddSpriteBatch(RenderSystem::CARD_BATCH, RenderSystem::CARD, TextureManager::CARD_BLUE, c3.x, c3.y);
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

			RenderSystem::AddSpriteBatch(RenderSystem::CARD_BATCH, RenderSystem::CARD, TextureManager::CARD_BLUE, c4.x, c4.y);
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

			RenderSystem::AddSpriteBatch(RenderSystem::CARD_BATCH, RenderSystem::CARD, TextureManager::CARD_BLUE, c5.x, c5.y);
		}

	}
}



