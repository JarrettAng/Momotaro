//#include "RenderSystem.h"
//#include <iostream>
//#include <string>
//#include <InputManager.h>
//#include <TextureManager.h>
//#include <PauseManager.h>
//#include <SceneManager.h>
//
////ADDED
//#include "AEEngine.h"
//#include <UIManager.h>
//
//
//namespace UIManager {
//
//	//TRIAL---------------------------------------------------------------------------------------------------------------------
//	//INTEGRATE INTO SYSTEMS LATER-----------------------------------------------------------------------------------------------
//
//
//	AEGfxVertexList* pMesh = 0;
//	AEGfxTexture* pTex;
//
//	float height, width;
//
//	void Initialize() {
//
//		//Debugs--------------------
//		//std::cout << "INITIALIZED\n";
//
//		MakeMesh();
//		Texture();
//
//		height = 1.0f;
//		width = 1.0f;
//	}
//
//	void MakeMesh() {
//		
//
//		AEGfxMeshStart();
//
//		AEGfxTriAdd(0.0f,  0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
//					0.0f, -1.0f, 0xFFFFFFFF, 0.0f, height,
//					1.0f, -1.0f, 0xFFFFFFFF, width, height);
//
//		AEGfxTriAdd(1.0f, -1.0f, 0xFFFFFFFF, width, height,
//					1.0f,  0.0f, 0xFFFFFFFF, width, 0.0f,
//					0.0f,  0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
//
//		pMesh = AEGfxMeshEnd();
//
//		//Debugs--------------------
//		//std::cout << "MESH MADE\n";
//	}
//
//	void Texture() {
//		pTex = AEGfxTextureLoad("Assets/synergyTex.png");
//		AE_ASSERT_MESG(pTex, "Failed to create texture!");
//
//		//Debugs--------------------
//		//std::cout << "TXT LOADED\n";
//	}
//
//	//TEST---------------------------
//	void DrawUI() {
//		// Tell the engine to get ready to draw something with texture.
//		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
//
//		// Set the tint to white, so that the sprite can 
//		// display the full range of colors (default is black).
//		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
//
//		// Set blend mode to AE_GFX_BM_BLEND
//		// This will allow transparency.
//		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
//		AEGfxSetTransparency(1.0f);
//
//		// Set the texture to pTex
//		AEGfxTextureSet(pTex, 0.5f, 0.5f);
//
//		// Create a scale matrix that scales by 100 x and y
//		AEMtx33 scale = { 0 };
//		AEMtx33Scale(&scale, 10.0f, 10.0f);
//
//		// Create a rotation matrix that rotates by 45 degrees
//		AEMtx33 rotate = { 0 };
//		AEMtx33Rot(&rotate, 0);
//
//		// Create a translation matrix that translates by
//		// 100 in the x-axis and 100 in the y-axis
//		AEMtx33 translate = { 0 };
//		AEMtx33Trans(&translate, 1.0f, 1.0f);
//
//		// Concatenate the matrices (TRS)
//		AEMtx33 transform = { 0 };
//		AEMtx33Concat(&transform, &rotate, &scale);
//		AEMtx33Concat(&transform, &translate, &transform);
//
//
//		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
//
//
//		//Debugs--------------------
//		//std::cout << "DRAWING UI\n";
//	}
//
//	void Draw() {
//
//		RenderSystem::AddRectToBatch(
//			RenderSystem::UI_BATCH,
//			10.0f, 10.0f,
//			100.0f, 100.0f, 
//			TextureManager::SYNERGY, 
//			0, 0
//		);
//	}

//	/*!***********************************************************************
//	* FONTS
//	*************************************************************************/
//	//FONT roboto;
//	/*************************************************************************/
//
//	/*!***********************************************************************
//	* TEXT VARIABLE CACHE
//	*************************************************************************/
//	f32 textWidth, textHeight;
//	/*************************************************************************/
//
//	/*!***********************************************************************
//	* FUNCTION FORWARD DECLARATIONS
//	*************************************************************************/
//	void InitializeFont();
//	void AddButtonToBatch(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, AEGfxTexture* tex, const Vec3<float>& txtColor, const Vec4<float>& btnColor);
//	/*************************************************************************/
//
//	/*!***********************************************************************
//	* HELPER FUNCTIONS FOR CREATING UI DATA.
//	*************************************************************************/
//	void TransformDataToUIData(UIData& data, const float& x, const float& y, const float& width, const float& height);
//	void GraphicsDataToUIData(UIData& data, AEGfxTexture* tex, const Vec4<float>& color);
//	void TextDataToUIData(UIData& data, const s8& font, const float& x, const float& y, std::string text, const Vec3<float>& color, const float& scale);
//	/*************************************************************************/
//
//	/*!***********************************************************************
//	* HELPER FUNCTIONS FOR BUTTON.
//	*************************************************************************/
//	AEVec2 GetButtonSize(const float& xPadding, const float& yPadding);
//	AEVec2 GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& textWidth, const float& textHeight);
//	/*************************************************************************/
//
//
//	void Initialize() {
//		//InitializeFont();
//	}
//
//	//void InitializeFont() {
//	//	roboto.type = ROBOTO;
//	//	roboto.S = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 20);
//	//	roboto.M = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 40);
//	//	roboto.L = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 60);
//	//}
//
//	void AddTextToBatch(const s8& font, const float& x, const float& y, const int& layer, std::string text, const Vec3<float>& color)
//	{
//		UIData data;
//		// Order of rendering UI.
//		data.layer = layer;
//
//		// Initialize text data.
//		TextDataToUIData(data, font, x, y, text, color, 1);
//
//		// Add text data to UI batch queue in render system.
//		//RenderSystem::AddUIBatch(data);
//	}
//
//	void AddRectToBatch(const float& x, const float& y, const float& width, const float& height, const int& layer, const Vec4<float>& color, AEGfxTexture* tex)
//	{
//		UIData data;
//		// Order of rendering UI.
//		data.layer = layer;
//		/*!***********************************************************************
//		* Initialize rect data.
//		*************************************************************************/
//		TransformDataToUIData(data, x, y, width, height);
//
//		/*!***********************************************************************
//		* Initialize graphics data.
//		*************************************************************************/
//		GraphicsDataToUIData(data, tex, color);
//
//		// Add rect data to UI batch queue in render system.
//		//RenderSystem::AddUIBatch(data);
//	}
//
//	void AddButtonToBatch(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, AEGfxTexture* tex, const Vec3<float>& txtColor, const Vec4<float>& btnColor) {
//
//		UIData data;
//		// Order of rendering UI.
//		data.layer = layer;
//
//		// Get width and height to position text in middle of rect.
//		AEGfxGetPrintSize(font, const_cast<char*>(text.c_str()), 1, textWidth, textHeight);
//
//		/*!***********************************************************************
//		* Initialize rect data.
//		*************************************************************************/
//		// Set width and height based on text size.
//		AEVec2 buttonSize = GetButtonSize(xPadding, yPadding);
//		TransformDataToUIData(data, x, y, buttonSize.x, buttonSize.y);
//
//		/*!***********************************************************************
//		* Initialize graphics data.
//		*************************************************************************/
//		GraphicsDataToUIData(data, tex, btnColor);
//
//		/*!***********************************************************************
//		* Initialize text data.
//		*************************************************************************/
//		// Position text to middle of rect.
//		AEVec2 textPos = RenderSystem::GetPivotPos(GetCenteredTextPos(x, y, buttonSize.x, buttonSize.y, textWidth, textHeight), buttonSize.x / AEGetWindowWidth() * 2, buttonSize.y / AEGetWindowHeight() * 2);
//		TextDataToUIData(data, font, textPos.x, textPos.y, text, txtColor, 1);
//
//		// Add button data to UI batch queue in render system.
//		//RenderSystem::AddUIBatch(data);
//	}
//
//	/*!***********************************************************************
//	* OVERLOADED RENDER BUTTON FUNCS
//	*************************************************************************/
//
//	// Button with TEXTURE + text.
//	void AddButtonToBatch(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, AEGfxTexture* tex, const Vec3<float>& txtColor) {
//		AddButtonToBatch(x, y, xPadding, yPadding, layer, font, text, tex, txtColor, Vec4<float>{});
//	}
//
//	// Button with COLOR + text.
//	void AddButtonToBatch(const float& x, const float& y, const float& xPadding, const float& yPadding, const int& layer, const s8& font, const std::string& text, const Vec4<float>& btnColor, const Vec3<float>& txtColor) {
//		AddButtonToBatch(x, y, xPadding, yPadding, layer, font, text, nullptr, txtColor, btnColor);
//	}
//	/*************************************************************************/
//
//	/*!***********************************************************************
//	* OVERLOADED RENDER RECT FUNCS
//	*************************************************************************/
//	// Rect with TEXTURE.
//	void AddRectToBatch(const float& x, const float& y, const float& width, const float& height, const int& layer, AEGfxTexture* tex) {
//		AddRectToBatch(x, y, width, height, layer, Vec4<float>{}, tex);
//	}
//	// Rect with COLOR.
//	void AddRectToBatch(const float& x, const float& y, const float& width, const float& height, const int& layer, const Vec4<float>& btnColor) {
//		AddRectToBatch(x, y, width, height, layer, btnColor, nullptr);
//	}
//	/*************************************************************************/
//
//	//FONT GetFont(const FONT_TYPE& type) {
//	//	switch (type)
//	//	{
//	//	case ROBOTO:
//	//		return roboto;
//	//	default:
//	//		break;
//	//	}
//	//	// Default font.
//	//	std::cout << "FONT TYPE INVALID, RETURN DEFAULT FONT.";
//	//	return roboto;
//	//}
//
//	AEVec2 GetButtonSize(const float& xPadding, const float& yPadding) {
//		// Get button's width and height by adding text width/height with padding given.
//		return AEVec2{ textWidth * AEGetWindowWidth() / 2 + xPadding * 2 , textHeight * AEGetWindowHeight() / 2 + yPadding * 2 };
//	}
//
//	AEVec2 GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& textWidth, const float& textHeight) {
//		// It just works
//		return AEVec2{ ((x / AEGetWindowWidth()) * 2) + ((((width / AEGetWindowWidth()) * 2) - textWidth) / 2) , ((y / AEGetWindowHeight()) * 2) - ((height / 2) / AEGetWindowHeight()) * 2 - (textHeight / 2) };
//	}
//
//	void TransformDataToUIData(UIData& data, const float& x, const float& y, const float& width, const float& height) {
//		data.transform.x = x;
//		data.transform.y = y;
//		data.transform.width = width;
//		data.transform.height = height;
//	}
//
//	void GraphicsDataToUIData(UIData& data, AEGfxTexture* tex, const Vec4<float>& color) {
//		data.graphics.hasGraphics = true;
//		if (tex) {
//			data.graphics.tex = tex;
//		}
//		else {
//			data.graphics.color = color;
//		}
//	}
//
//	void TextDataToUIData(UIData& data, const s8& font, const float& x, const float& y, std::string text, const Vec3<float>& color, const float& scale) {
//		data.text.hasText = true;
//		data.text.fontID = font;
//		data.text.x = x;
//		data.text.y = y;
//		data.text.text = text;
//		data.text.color = color;
//		data.text.scale = scale;
//	}
//}

#include <UIManager.h>
#include <FontManager.h>
#include <ColorTable.h>
#include <MomoMaths.h>

namespace UI {
	TextBox::TextBox() {
		transform = RenderSystem::Transform();
		maxWidth = 500;
		fontSize = 20;
		alignment = LEFT_JUSTIFY;
		color = COLOR_BLACK;
	}

	TextBox::TextBox(Vec2<float> screenPos, std::string text, TextBoxAlignment _alignment, float _maxWidth, float _fontSize, Vec3<float> _color) {
		// Set the size of the text (normalized already)
		AEGfxGetPrintSize(FontManager::GetFont(FontManager::ROBOTO), const_cast<char*>(text.c_str()), _fontSize / FontManager::DEFAULT_FONT_SIZE, transform.size.x, transform.size.y);

		// Normalize the positions (0 - 1)
		transform.pos.x = screenPos.x / AEGfxGetWinMaxX();
		transform.pos.y = screenPos.y / AEGfxGetWinMaxY();
		
		fontSize = _fontSize;								// How big the font should be (XX/100)
		maxWidth = _maxWidth / AEGfxGetWinMaxX();			// Max width before wrapping the text
		float charWidth = transform.size.x / text.size();	// The x-length of each character
		alignment = _alignment;								// Set the current alignment of the texts (Left, center, right)
		color = _color;										// Set the color of the text box

		Vec2<float> currPos;								// The render pos of each line of text

		size_t start = 0, end = text.size(), cutoff = text.find_last_of(" \n\t"); // Pointers to the different parts of the string

		// Case 1: string fits max width, add to texts then finish
		if (charWidth * (float)(end - start + 1) < maxWidth) {
			switch (alignment) { // Set the x-pos of the text based on the current alignment
				case LEFT_JUSTIFY:
				currPos.x = transform.pos.x;
				break;
				case CENTER_JUSTIFY:
				currPos.x = transform.pos.x + (maxWidth / 2.0f) - (charWidth * (end - start + 1) * 0.5f);
				break;
				case RIGHT_JUSTIFY:
				currPos.x = transform.pos.x + maxWidth - (charWidth * (end - start + 1));
				break;
			}
			currPos.y = transform.pos.y;

			texts.emplace_back(text, currPos);
		}
		else { // Case 2: Wrap the string around based on the max width
			while (start < end) {
				// Keep looping until the largest spacing index within the max-width is reached
				while (charWidth * (float)(cutoff - start + 1) > maxWidth) { 
					cutoff = text.find_last_of(" \n\t", cutoff - 1);

					if (cutoff < start) {
						cutoff = end;
						break;
					}
					if (cutoff == std::string::npos) {
						cutoff = text.find_last_of(" \n\t");
						break;
					}
				}

				switch (alignment) { // Set the x-pos of the text based on the current alignment
				case LEFT_JUSTIFY:
					currPos.x = transform.pos.x;
					break;
				case CENTER_JUSTIFY:
					currPos.x = transform.pos.x + (maxWidth / 2.0f) - (charWidth * (cutoff - start + 1) * 0.5f);
					break;
				case RIGHT_JUSTIFY:
					currPos.x = transform.pos.x + maxWidth - (charWidth * (cutoff - start + 1));
					break;
				}
				currPos.y = transform.pos.y - texts.size() * transform.size.y * 1.25f;

				texts.emplace_back(text.substr(start, cutoff - start), currPos);

				start = cutoff == std::string::npos ? cutoff : cutoff + 1;
				cutoff = end;
			}
		}
	}

	TextBox& TextBox::operator=(TextBox const& rhs) {
		this->alignment = rhs.alignment;
		this->color = rhs.color;
		this->fontSize = rhs.fontSize;
		this->maxWidth = rhs.maxWidth;
		this->transform = rhs.transform;

		this->texts.clear();
		for (auto text : rhs.texts) {
			texts.push_back(text);
		}
		return *this;
	}


	const RenderSystem::Transform& TextBox::GetPos() const {
		return transform;
	}

	void TextBox::Render() {
		for (size_t index = 0; index < texts.size(); ++index) {
			RenderSystem::AddTextToBatch(RenderSystem::UI_BATCH, texts[index].pos.x, texts[index].pos.y,
										 FontManager::GetFont(FontManager::ROBOTO), fontSize, texts[index].text, 5, color);
		}
	}
}
