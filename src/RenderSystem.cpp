/*!************************************************************************
\file RenderSystem.cpp
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 18-01-2023
\brief
This source file declares

**************************************************************************/

#include<RenderSystem.h>

namespace RenderSystem {

	/*!***********************************************************************
	* FORWARD DECLARATIONS
	*************************************************************************/
	void SetRenderPivot(const RENDER_PIVOT& pivot);
	AEVec2 GetPivotPos(const AEVec2& pos, const float& width, const float& height);

	void SortBatch(std::vector<Renderable>& batch);

	void UpdateRenderSetting(RenderSetting setting = {});
	void UpdateRenderTransformMtx(const int& x, const int& y, const AEVec2& scale, const float& rot = 0);

	void AddRectToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& width, const float& height, const float& rot, const int& layer, const Vec4<float>& color, TextureManager::TEX_TYPE tex);
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, const int& layer, TextureManager::TEX_TYPE tex = TextureManager::NONE, const Vec4<float>& btnColor = { 1,1,1,1 }, const Vec3<float>& txtColor = { 1,1,1 });

	void RenderText(float x, float y, s8 fontID, int fontSize, std::string text, Vec3<float> color = { 1.0f,1.0f,1.0f });
	void RenderRect(const float& x, const float& y, const float& width, const float& height, TextureManager::TEX_TYPE  tex);
	void RenderRect(const float& x, const float& y, const float& width, const float& height, Vec4<float> color = { 1.0f,1.0f,1.0f,1.0f });

	AEVec2 GetButtonSize(const float& xPadding, const float& yPadding);
	AEVec2 GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& textWidth, const float& textHeight);

	/*!***********************************************************************
	* RENDER BATCHES
	*************************************************************************/
	std::vector<Renderable> tileBatch;
	std::vector<Renderable> gamePieceBatch;
	std::vector<Renderable> UIBatch;
	std::vector<std::vector<Renderable>> renderBatches = { tileBatch, gamePieceBatch, UIBatch };

	/*!***********************************************************************
	* MATRICES
	*************************************************************************/
	AEMtx33 identityMtx{
		1,0,0,
		0,1,0,
		0,0,1
	};
	AEMtx33 transformMtx, translateMtx, scaleMtx, rotMtx = identityMtx;

	/*!***********************************************************************
	* VARIABLES
	*************************************************************************/
	f32 textWidth, textHeight;	// Cache text width and height for calculating text position when drawing button.
	RENDER_PIVOT renderPivot;


	/*!***********************************************************************
	\brief
		Initialize render system.
	*************************************************************************/
	void RenderSystem::Initialize() {
		SetRenderPivot(TOP_LEFT);
	}

	/*!***********************************************************************
	\brief
		Main rendering func.
			- Loop through all batches and render each renderable in batch.
	*************************************************************************/
	void Render() {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		UpdateRenderSetting();
		for (auto& batch : renderBatches) {
			// Sort batch based on layer before drawing.
			SortBatch(batch);
			// Render every renderable object.
			for (auto& obj : batch) {
				switch (obj.type) {
				case RECT:
					if (obj.rect.graphics.tex != TextureManager::NONE) {
						// RENDER RECT WITH TEXTURE.
						RenderRect(obj.rect.transform.pos.x, obj.rect.transform.pos.y, obj.rect.transform.size.x, obj.rect.transform.size.y, obj.rect.graphics.tex);
					}
					else {
						// RENDER RECT WITH COLOR.
						RenderRect(obj.rect.transform.pos.x, obj.rect.transform.pos.y, obj.rect.transform.size.x, obj.rect.transform.size.y, obj.rect.graphics.color);
					}
					break;
				case TEXT:
					// RENDER TEXT
					RenderText(obj.text.pos.x, obj.text.pos.y, obj.text.fontID, obj.text.fontSize, const_cast<char*>(obj.text.text.c_str()), obj.text.color);
					break;
				default:
					break;
				}
			}
			// Clear batch.
			batch.clear();
		}
	}

	/*!***********************************************************************
	\brief
		Add text obj to batch.
	*************************************************************************/
	void AddTextToBatch(const BATCH_TYPE& id, const float& x, const float& y, const s8& font, const int& fontSize, std::string text, const int& layer, const Vec3<float>& color)
	{
		Renderable obj;
		obj.type = TEXT;
		obj.layer = layer;

		obj.text.fontID = font;
		obj.text.text = text;

		obj.text.pos = { x,y };
		obj.text.color = color;
		obj.text.fontSize = fontSize;

		// Add to batch.
		renderBatches[id].push_back(obj);
	}

	/*!***********************************************************************
	* OVERLOADED RECT FUNCTIONS
	*************************************************************************/
	// Rect with TEXTURE.
	void AddRectToBatch(const BATCH_TYPE& batch, const float& x, const float& y, const float& width, const float& height, TextureManager::TEX_TYPE tex, const int& layer, const float& rot) {
		AddRectToBatch(batch, x, y, width, height, rot, layer, {}, tex);
	}
	// Rect with COLOR.
	void AddRectToBatch(const BATCH_TYPE& batch, const float& x, const float& y, const float& width, const float& height, const Vec4<float>& btnColor, const int& layer, const float& rot) {
		AddRectToBatch(batch, x, y, width, height, rot, layer, btnColor, TextureManager::NONE);
	}
	/************************************************************************/

	/*!***********************************************************************
	\brief
		Add rect obj to batch.
	*************************************************************************/
	void AddRectToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& width, const float& height, const float& rot, const int& layer, const Vec4<float>& color, TextureManager::TEX_TYPE tex)
	{
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
		renderBatches[id].push_back(obj);
	}


	/*!***********************************************************************
	* OVERLOADED BUTTON FUNCTIONS
	*************************************************************************/
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, TextureManager::TEX_TYPE tex, const int& layer, const Vec3<float>& txtColor) {
		AddButtonToBatch(id, x, y, xPadding, yPadding, font, fontSize, text, layer, tex, {}, txtColor);
	}
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, const int& layer, const Vec4<float>& btnColor, const Vec3<float>& txtColor) {
		AddButtonToBatch(id, x, y, xPadding, yPadding, font, fontSize, text, layer, TextureManager::NONE, btnColor, txtColor);
	}
	/************************************************************************/

	/*!***********************************************************************
	\brief
		Add button obj to batch.
	*************************************************************************/
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const int& fontSize, const std::string& text, const int& layer, TextureManager::TEX_TYPE tex, const Vec4<float>& btnColor, const Vec3<float>& txtColor) {
		// Get text width and height based on text.
		AEGfxGetPrintSize(font, const_cast<char*>(text.c_str()), fontSize / FontManager::DEFAULT_FONT_SIZE, textWidth, textHeight);

		// Get button width and height.
		AEVec2 buttonSize = GetButtonSize(xPadding, yPadding);

		// Get centereed text position based on button size.
		AEVec2 textPos = RenderSystem::GetPivotPos(GetCenteredTextPos(x, y, buttonSize.x, buttonSize.y, textWidth, textHeight), buttonSize.x / AEGetWindowWidth() * 2, buttonSize.y / AEGetWindowHeight() * 2);

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
	*************************************************************************/
	AEVec2 GetButtonSize(const float& xPadding, const float& yPadding) {
		// Get button's width and height by adding text width/height with padding given.
		return AEVec2{ textWidth * AEGetWindowWidth() / 2 + xPadding * 2 , textHeight * AEGetWindowHeight() / 2 + yPadding * 2 };
	}

	/*!***********************************************************************
	\brief
		Calculate centered text position based on rect width and height.
			- Assuming rect is drawn with TOP-LEFT pivot.
	*************************************************************************/
	AEVec2 GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& textWidth, const float& textHeight) {
		// It just works
		return AEVec2{ ((x / AEGetWindowWidth()) * 2) + ((((width / AEGetWindowWidth()) * 2) - textWidth) / 2) , ((y / AEGetWindowHeight()) * 2) - ((height / 2) / AEGetWindowHeight()) * 2 - (textHeight / 2) };
	}

	/*!***********************************************************************
	\brief
		Render text on screen.
			- Font start to get blurry after font size 100.
	*************************************************************************/
	void RenderText(float x, float y, s8 fontID, int fontSize, std::string text, Vec3<float> color) {
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(fontID, const_cast<char*>(text.c_str()), x, y, fontSize / FontManager::DEFAULT_FONT_SIZE, color.x, color.y, color.z);
	}

	/*!***********************************************************************
	\brief
		Render rect with TEXTURE.
	*************************************************************************/
	void RenderRect(const float& x, const float& y, const float& width, const float& height, TextureManager::TEX_TYPE tex) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Get texture + offset UVS for animations.
		AEGfxTextureSet(TextureManager::GetTexture(tex), TextureManager::GetTexWidth(tex), TextureManager::GetTexHeight(tex));
		// Position rect.
		UpdateRenderTransformMtx(x, y, AEVec2{ width,height });
		// Draw rect.
		AEGfxMeshDraw(TextureManager::GetMesh(tex), AE_GFX_MDM_TRIANGLES);
	}

	/*!***********************************************************************
	\brief
		Render rect with COLOR.
	*************************************************************************/
	void RenderRect(const float& x, const float& y, const float& width, const float& height, Vec4<float> color) {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set color.
		AEGfxSetTintColor(color.w, color.x, color.y, color.z);
		// Position rect.
		UpdateRenderTransformMtx(x, y, AEVec2{ width,height });
		// Draw rect.
		AEGfxMeshDraw(TextureManager::GetMesh(TextureManager::NONE), AE_GFX_MDM_TRIANGLES);
	}

	/*!***********************************************************************
	\brief
		Sort batch list based on layer value.
	*************************************************************************/
	void SortBatch(std::vector<Renderable>& batch) {
		std::sort(batch.begin(), batch.end(), [](const Renderable& a, const Renderable& b) {  return a.layer < b.layer; });
	}

	/*!***********************************************************************
	\brief
		Update engine render settings.
	*************************************************************************/
	void UpdateRenderSetting(RenderSetting setting) {
		// Spcify blend mode. AE_GFX_BM_BLEND to allow transperency.
		AEGfxSetBlendMode(setting.blendMode);
		// Add a color overlay on top of texture/mesh.
		AEGfxSetBlendColor(setting.blendColor.w, setting.blendColor.x, setting.blendColor.y, setting.blendColor.z);
		// Add tint color on top of texture/mesh.
		AEGfxSetTintColor(setting.tint.w, setting.tint.x, setting.tint.y, setting.tint.z);
		// Affects global transparency for drawn mesh.
		AEGfxSetTransparency(setting.transperancy);
	}

	/*!***********************************************************************
	\brief
		Update global transform mtx for subsequent sprites to be drawn.
	*************************************************************************/
	void UpdateRenderTransformMtx(const int& x, const int& y, const AEVec2& scale, const float& rot) {
		// Set scaling
		AEMtx33Scale(&scaleMtx, scale.x, scale.y);
		// Set rotation.
		AEMtx33RotDeg(&rotMtx, rot);
		// Set translation.
		AEMtx33Trans(&translateMtx, x, y);

		// Apply scaling -> rotatation -> translation to transform matrix.
		AEMtx33Concat(&transformMtx, &scaleMtx, &rotMtx);
		AEMtx33Concat(&transformMtx, &translateMtx, &transformMtx);

		// Apply transform matrix to mesh to be drawn.
		AEGfxSetTransform(transformMtx.m);
	}

	/*!***********************************************************************
	\brief
		Set current pivot position.
	*************************************************************************/
	void SetRenderPivot(const RENDER_PIVOT& pivot) {
		renderPivot = pivot;
	}

	/*!***********************************************************************
	\brief
		Convert position based on current pivot position.
	*************************************************************************/
	AEVec2 GetPivotPos(const AEVec2& pos, const float& width, const float& height) {
		switch (renderPivot)
		{
		case RenderSystem::TOP_LEFT:
			return pos;
		case RenderSystem::TOP_MID:
			return AEVec2{ pos.x - width / 2, pos.y };
		case RenderSystem::TOP_RIGHT:
			return AEVec2{ pos.x - width, pos.y };
		case RenderSystem::MID_LEFT:
			return AEVec2{ pos.x, pos.y + height / 2 };
		case RenderSystem::MID:
			return AEVec2{ pos.x - width / 2, pos.y + height / 2 };
		case RenderSystem::MID_RIGHT:
			return AEVec2{ pos.x - width, pos.y + height / 2 };
		case RenderSystem::BOT_LEFT:
			return AEVec2{ pos.x, pos.y + height };
		case RenderSystem::BOT_MID:
			return AEVec2{ pos.x - width / 2, pos.y + height };
		case RenderSystem::BOT_RIGHT:
			return AEVec2{ pos.x - width , pos.y + height };
		default:
			break;
		}
		std::cerr << "Error : " << __FILE__ << " ln" << __LINE__ << " UNABLE TO GET PIVOT POS" << std::endl;
		return pos;
	}
}
