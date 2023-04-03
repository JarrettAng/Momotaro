/*!************************************************************************
\file			RenderSystem.cpp
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			18-01-2023
\brief
This source file handles rendering of graphics of the game.

All content � 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#include <RenderSystem.h>
#include <DebugManager.h>

namespace RenderSystem {

	/*!***********************************************************************
	* Forward Declarations.
	*************************************************************************/
	void SetRenderPivot(const RENDER_PIVOT& pivot);
	Vec2<float> GetPivotPos(const Vec2<float>& pos, const float& width, const float& height);

	void SortBatch(std::vector<std::pair<Renderable, RenderSetting>>& batch);

	void UpdateRenderSetting(RenderSetting setting = {});
	void UpdateRenderTransformMtx(const float& x, const float& y, const Vec2<float>& scale, const float& rot = 0);

	void AddRectToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& width, const float& height, const float& rot, const int& layer, const Vec4<float>& color, TextureManager::TEX_TYPE tex);
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, const int& layer, TextureManager::TEX_TYPE tex = TextureManager::NONE, const Vec4<float>& btnColor = { 1,1,1,1 }, const Vec3<float>& txtColor = { 1,1,1 });

	void RenderText(float x, float y, s8 fontID, int fontSize, std::string text, Vec3<float> color = { 1.0f,1.0f,1.0f });
	void RenderRect(const float& x, const float& y, const float& width, const float& height, TextureManager::TEX_TYPE  tex);
	void RenderRect(const float& x, const float& y, const float& width, const float& height, Vec4<float> color = { 1.0f,1.0f,1.0f,1.0f });

	Vec2<float> GetButtonSize(const float& xPadding, const float& yPadding);
	Vec2<float> GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& textWidth, const float& textHeight);

	/*!***********************************************************************
	* Render Batches.
	*************************************************************************/
	std::vector<std::pair<Renderable, RenderSetting>> tileBatch;
	std::vector<std::pair<Renderable, RenderSetting>> gamePieceBatch;
	std::vector<std::pair<Renderable, RenderSetting>> UIBatch;
	std::vector<std::vector<std::pair<Renderable, RenderSetting>>> renderBatches = { tileBatch, gamePieceBatch, UIBatch };

	/*!***********************************************************************
	* Matrices.
	*************************************************************************/
	AEMtx33 identityMtx{
		1,0,0,
		0,1,0,
		0,0,1
	};
	AEMtx33 transformMtx, translateMtx, scaleMtx, rotMtx = identityMtx;

	/*!***********************************************************************
	* Variables.
	*************************************************************************/
	f32 textWidth, textHeight;	// Cache text width and height for calculating text position when drawing button.
	RENDER_PIVOT renderPivot;
	RenderSetting setting{};

	/*!***********************************************************************
	\brief
		Initialize RenderSystem.
	*************************************************************************/
	void RenderSystem::Initialize() {
		// 30x30 grid.
		tileBatch.reserve(900);

		// Estimated number of game pieces and UI in game.
		gamePieceBatch.reserve(30);
		UIBatch.reserve(30);
	}

	/*!***********************************************************************
	\brief
		Free RenderSystem.
	*************************************************************************/
	void RenderSystem::Free() {
		tileBatch.clear();
		gamePieceBatch.clear();
		UIBatch.clear();
	}

	/*!***********************************************************************
	\brief
		Main rendering func.
			- Loop through all batches and render each renderable in batch.
	*************************************************************************/
	void Render() {
		// Enable rendering of textures.
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Affects global transparency for drawn mesh.
		AEGfxSetTransparency(1.0f);
		// Using default render setting.
		UpdateRenderSetting();
		// Loop through each batch.
		for (auto& batch : renderBatches) {
			// Sort batch based on layer before drawing.
			SortBatch(batch);
			// Render every renderable object.
			for (auto& obj : batch) {
				// Use object render setting if required.
				if (!obj.second.isDefault()) UpdateRenderSetting(obj.second);
				switch (obj.first.type) {
				// Render rect.
				case RECT:
					if (obj.first.rect.graphics.tex != TextureManager::NONE) {
						// RENDER RECT WITH TEXTURE.
						RenderRect(obj.first.rect.transform.pos.x, obj.first.rect.transform.pos.y, obj.first.rect.transform.size.x, obj.first.rect.transform.size.y, obj.first.rect.graphics.tex);
					}
					else {
						// RENDER RECT WITH COLOR.
						RenderRect(obj.first.rect.transform.pos.x, obj.first.rect.transform.pos.y, obj.first.rect.transform.size.x, obj.first.rect.transform.size.y, obj.first.rect.graphics.color);
					}
					break;
				// Render text.
				case TEXT:
					// RENDER TEXT
					RenderText(obj.first.text.pos.x, obj.first.text.pos.y, obj.first.text.fontID, obj.first.text.fontSize, const_cast<char*>(obj.first.text.text.c_str()), obj.first.text.color);
					break;
				default:
					break;
				}
				// Switch back to default render setting if changed.
				if (!obj.second.isDefault()) UpdateRenderSetting();
			}
			// Clear batch.
			batch.clear();
		}
	}

	/*!***********************************************************************
	\brief
		Add text obj to batch.
	\param id
		Batch type.
	\param x
		X position.
	\param y
		Y position.
	\param font
		Font of text.
	\param fontSize
		Size of text.
	\param text
		Text string.
	\param layer
		Layer of text.
	\param color
		Color of text.
	*************************************************************************/
	void AddTextToBatch(const BATCH_TYPE& id, const float& x, const float& y, const s8& font, const int& fontSize, std::string text, const int& layer, const Vec3<float>& color)
	{
		// Initializing text object.
		Renderable obj;
		obj.type = TEXT;
		obj.layer = layer;

		// Set font and text.
		obj.text.fontID = font;
		obj.text.text = text;

		// Set position and visuals.
		obj.text.pos = { x,y };
		obj.text.color = color;
		obj.text.fontSize = fontSize;

		// Add to batch.
		renderBatches[id].push_back({ obj,{} });
	}

	/*!***********************************************************************
	\brief
		OVERLOADED RECT FUNCTION.
		Rect with TEXTURE.
	*************************************************************************/
	void AddRectToBatch(const BATCH_TYPE& batch, const float& x, const float& y, const float& width, const float& height, TextureManager::TEX_TYPE tex, const int& layer, const float& rot) {
		AddRectToBatch(batch, x, y, width, height, rot, layer, {}, tex);
	}

	/*!***********************************************************************
	\brief
		OVERLOADED RECT FUNCTION.
		Rect with COLOR.
	*************************************************************************/
	void AddRectToBatch(const BATCH_TYPE& batch, const float& x, const float& y, const float& width, const float& height, const Vec4<float>& btnColor, const int& layer, const float& rot) {
		AddRectToBatch(batch, x, y, width, height, rot, layer, btnColor, TextureManager::NONE);
	}

	/*!***********************************************************************
	\brief
		Add rect obj to batch.
	\param id
		Batch type.
	\param x
		X position.
	\param y
		Y position.
	\param width
		Width of rect.
	\param height
		Height of rect.
	\param rot
		Rotation of rect.
	\param layer
		Layer of rect.
	\param color
		Color of rect.
	\param tex
		Texture of rect.
	*************************************************************************/
	void AddRectToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& width, const float& height, const float& rot, const int& layer, const Vec4<float>& color, TextureManager::TEX_TYPE tex)
	{	
		// Initialize rect object.
		Renderable obj;
		obj.type = RECT;
		obj.layer = layer;

		// Set transform.
		obj.rect.transform.pos = { x,y };
		obj.rect.transform.size = { width,height };
		obj.rect.transform.rot = rot;

		// Set graphics.
		obj.rect.graphics.color = color;
		obj.rect.graphics.tex = tex;

		// Add to batch.
		renderBatches[id].push_back({ obj,setting });

		// Reset setting to default if it is changed.
		if (!setting.isDefault()) setting.setDefault();
	}

	/*!***********************************************************************
	\brief
		OVERLOADED BUTTON FUNCTION.
		Rect with TEXTURE.
	*************************************************************************/
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, TextureManager::TEX_TYPE tex, const int& layer, const Vec3<float>& txtColor) {
		AddButtonToBatch(id, x, y, xPadding, yPadding, font, fontSize, text, layer, tex, {}, txtColor);
	}

	/*!***********************************************************************
	\brief
		OVERLOADED BUTTON FUNCTION.
		Rect with COLOR.
	*************************************************************************/
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, const int& layer, const Vec4<float>& btnColor, const Vec3<float>& txtColor) {
		AddButtonToBatch(id, x, y, xPadding, yPadding, font, fontSize, text, layer, TextureManager::NONE, btnColor, txtColor);
	}

	/*!***********************************************************************
	\brief
		Add button obj to batch.
	\param id
		Batch type.
	\param x
		X position.
	\param y
		Y position.
	\param xPadding
		X padding from the sides.
	\param yPadding
		Y padding from top / bottom.
	\param font
		Font of text.
	\param fontSize
		Size of text.
	\param text
		Text string.
	\param layer
		Layer of button.
	\param tex
		Texture of button.
	\param btnColor
		Rect color.
	\param txtColor
		Text color.
	*************************************************************************/
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, const int& layer, TextureManager::TEX_TYPE tex, const Vec4<float>& btnColor, const Vec3<float>& txtColor) {
		// Get text width and height based on text.
		AEGfxGetPrintSize(font, const_cast<char*>(text.c_str()), fontSize / FontManager::DEFAULT_FONT_SIZE, textWidth, textHeight);

		// Get button width and height.
		Vec2<float> buttonSize = GetButtonSize(xPadding, yPadding);

		// Get centereed text position based on button size.
		Vec2<float> textPos = RenderSystem::GetPivotPos(GetCenteredTextPos(x, y, buttonSize.x, buttonSize.y, textWidth, textHeight), buttonSize.x / AEGetWindowWidth() * 2, buttonSize.y / AEGetWindowHeight() * 2);

		if (tex == TextureManager::NONE) {
			// Add rect with TEXTURE to batch.
			AddRectToBatch(id, x, y, buttonSize.x, buttonSize.y, btnColor, layer);
		}
		else {
			// Add rect with COLOR to batch.
			AddRectToBatch(id, x, y, buttonSize.x, buttonSize.y, tex, layer);
		}

		// Add text to batch.
		AddTextToBatch(id, textPos.x, textPos.y, font, fontSize, text, layer, txtColor);
	}

	/*!***********************************************************************
	\brief
		Get button width and height based on pos and padding.
	\param xPadding
		X padding from the sides.
	\param yPadding
		Y padding from top / bottom.
	\return
		Size of button.
	*************************************************************************/
	Vec2<float> GetButtonSize(const float& xPadding, const float& yPadding) {
		// Get button's width and height by adding text width/height with padding given.
		return Vec2<float>{ textWidth* AEGetWindowWidth() / 2 + xPadding * 2, textHeight* AEGetWindowHeight() / 2 + yPadding * 2 };
	}

	/*!***********************************************************************
	\brief
		Calculate centered text position based on rect width and height.
			- Assuming rect is drawn with TOP-LEFT pivot.
	\param x
		X position.
	\param y
		Y position.
	\param width
		Width of rect.
	\param height
		Height of rect.
	\param _textWidth
		Width of text.
	\param _textHeight
		Height of text.
	\return
		Final position of text.
	*************************************************************************/
	Vec2<float> GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& _textWidth, const float& _textHeight) {
		// It just works
		return Vec2<float>{ ((x / AEGetWindowWidth()) * 2) + ((((width / AEGetWindowWidth()) * 2) - _textWidth) / 2), ((y / AEGetWindowHeight()) * 2) - ((height / 2) / AEGetWindowHeight()) * 2 - (_textHeight / 2) };
	}

	/*!***********************************************************************
	\brief
		Render text on screen.
			- Font start to get blurry after font size 100.
	\param x
		X position.
	\param y
		Y position.
	\param fontID
		Font type.
	\param fontSize
		Size of font.
	\param text
		Text string.
	\param color
		Text color.
	*************************************************************************/
	void RenderText(float x, float y, s8 fontID, int fontSize, std::string text, Vec3<float> color) {
		// Enable drawing of text.
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// Draw text on screen.
		AEGfxPrint(fontID, const_cast<char*>(text.c_str()), x, y, fontSize / FontManager::DEFAULT_FONT_SIZE, color.x, color.y, color.z);
	}

	/*!***********************************************************************
	\brief
		Render rect with TEXTURE.
	\param x
		X position.
	\param y
		Y position.
	\param width
		Width of rect.
	\param height
		Height of rect.
	\param tex
		Texture of rect.
	*************************************************************************/
	void RenderRect(const float& x, const float& y, const float& width, const float& height, TextureManager::TEX_TYPE tex) {
		// Enable drawing of rect.
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Get texture + offset UVS for animations.
		AEGfxTextureSet(TextureManager::GetTexture(tex), TextureManager::GetTexWidth(tex), TextureManager::GetTexHeight(tex));
		// Position rect.
		UpdateRenderTransformMtx(x, y, Vec2<float>{ width, height });
		// Draw rect.
		AEGfxMeshDraw(TextureManager::GetMesh(tex), AE_GFX_MDM_TRIANGLES);
	}

	/*!***********************************************************************
	\brief
		Render rect with COLOR.
	\param x
		X position.
	\param y
		Y position.
	\param width
		Width of rect.
	\param height
		Height of rect.
	\param color
		Color of rect.
	*************************************************************************/
	void RenderRect(const float& x, const float& y, const float& width, const float& height, Vec4<float> color) {
		// Enable drawing of rect.
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set color.
		AEGfxSetTintColor(color.w, color.x, color.y, color.z);
		// Position rect.
		UpdateRenderTransformMtx(x, y, Vec2<float>{ width, height });
		// Draw rect.
		AEGfxMeshDraw(TextureManager::GetMesh(TextureManager::NONE), AE_GFX_MDM_TRIANGLES);
	}

	/*!***********************************************************************
	\brief
		Sort batch list based on layer value.
	\param batch
		Batch to sort.
	*************************************************************************/
	void SortBatch(std::vector<std::pair<Renderable, RenderSetting>>& batch) {
		std::sort(batch.begin(), batch.end(), [](const std::pair<Renderable, RenderSetting>& lhs, const std::pair<Renderable, RenderSetting>& rhs) {  return lhs.first.layer < rhs.first.layer; });
	}

	/*!***********************************************************************
	\brief
		Update current render setting to use for subsequent object when adding to batch.
	\param tint
		Tinting applied on object.
	\param blendColor
		Blend color applied on object.
	\param blendMode
		Blend mode of object.
	*************************************************************************/
	void SetRenderSetting(Vec4<float> tint, Vec4<float> blendColor, AEGfxBlendMode blendMode) {
		setting.tint = tint;
		setting.blendColor = blendColor;
		setting.blendMode = blendMode;
	}

	/*!***********************************************************************
	\brief
		Update engine render settings.
	\param _setting
		Setting to apply.
	*************************************************************************/
	void UpdateRenderSetting(RenderSetting _setting) {
		// Spcify blend mode. AE_GFX_BM_BLEND to allow transperency.
		AEGfxSetBlendMode(_setting.blendMode);
		// Add a color overlay on top of texture/mesh.
		AEGfxSetBlendColor(_setting.blendColor.w, _setting.blendColor.x, _setting.blendColor.y, _setting.blendColor.z);
		// Add tint color on top of texture/mesh.
		AEGfxSetTintColor(_setting.tint.w, _setting.tint.x, _setting.tint.y, _setting.tint.z);
	}

	/*!***********************************************************************
	\brief
		Update global transform mtx for subsequent sprites to be drawn.
	\param x
		X position.
	\param y
		Y position.
	\param scale
		Scale of object.
	\param rot
		Rotation of object.
	*************************************************************************/
	void UpdateRenderTransformMtx(const float& x, const float& y, const Vec2<float>& scale, const float& rot) {
		// Set scaling
		AEMtx33Scale(&scaleMtx, scale.x, scale.y);
		// Set rotation.
		AEMtx33RotDeg(&rotMtx, rot);
		// Set translation.
		AEMtx33Trans(&translateMtx, (f32)x, (f32)y);

		// Apply scaling -> rotatation -> translation to transform matrix.
		AEMtx33Concat(&transformMtx, &scaleMtx, &rotMtx);
		AEMtx33Concat(&transformMtx, &translateMtx, &transformMtx);

		// Apply transform matrix to mesh to be drawn.
		AEGfxSetTransform(transformMtx.m);
	}

	/*!***********************************************************************
	\brief
		Set current pivot position.
	\param pivot
		Pivot type.
	*************************************************************************/
	void SetRenderPivot(const RENDER_PIVOT& pivot) {
		renderPivot = pivot;
	}

	/*!***********************************************************************
	\brief
		Convert position based on current pivot position.
	\param pos
		Position of object.
	\param width
		Width of object.
	\param height
		Height of object.
	\return
		Final position.
	*************************************************************************/
	Vec2<float> GetPivotPos(const Vec2<float>& pos, const float& width, const float& height) {
		switch (renderPivot)
		{
		case RenderSystem::TOP_LEFT:
			return pos;
		case RenderSystem::TOP_MID:
			return Vec2<float>{ pos.x - width / 2, pos.y };
		case RenderSystem::TOP_RIGHT:
			return Vec2<float>{ pos.x - width, pos.y };
		case RenderSystem::MID_LEFT:
			return Vec2<float>{ pos.x, pos.y + height / 2 };
		case RenderSystem::MID:
			return Vec2<float>{ pos.x - width / 2, pos.y + height / 2 };
		case RenderSystem::MID_RIGHT:
			return Vec2<float>{ pos.x - width, pos.y + height / 2 };
		case RenderSystem::BOT_LEFT:
			return Vec2<float>{ pos.x, pos.y + height };
		case RenderSystem::BOT_MID:
			return Vec2<float>{ pos.x - width / 2, pos.y + height };
		case RenderSystem::BOT_RIGHT:
			return Vec2<float>{ pos.x - width, pos.y + height };
		default:
			break;
		}
		Debug::PrintError(__FILE__, __LINE__, "UNABLE TO GET PIVOT POS");
		return pos;
	}
}
