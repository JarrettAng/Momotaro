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

#include<AEEngine.h>
#include<RenderSystem.h>
#include <iostream>
#include <algorithm>
#include <TextureManager.h>
#include <FontManager.h>

namespace RenderSystem {

#pragma region Foward Declaration & Variables
	void InitMesh();
	void SortBatch(std::vector<Renderable> batch);

	void UpdateRenderSetting(RenderSetting setting = {});
	void UpdateRenderTransformMtx(const int& x, const int& y, const AEVec2& scale, const float& rot = 0);

	void RenderText(s8 fontID, std::string text, float x, float y, Vec3<float> color = { 1.0f,1.0f,1.0f });
	void RenderRect(const float& x, const float& y, const float& width, const float& height, TextureManager::TEX_TYPE  tex);
	void RenderRect(const float& x, const float& y, const float& width, const float& height, Vec4<float> color = { 1.0f,1.0f,1.0f,1.0f });

	void AddRectToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& width, const float& height, const float& rot, const int& layer, const Vec4<float>& color, TextureManager::TEX_TYPE tex);
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const std::string& text, const int& layer, TextureManager::TEX_TYPE tex = TextureManager::NONE, const Vec4<float>& btnColor = { 1,1,1,1 }, const Vec3<float>& txtColor = { 1,1,1 });

	AEVec2 GetButtonSize(const float& xPadding, const float& yPadding);
	AEVec2 GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& textWidth, const float& textHeight);
	f32 textWidth, textHeight;

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
	* MESH FOR RENDERING + PIVOT MESHES.
	*************************************************************************/
	RENDER_PIVOT renderPivot;

	// renderMesh is assigned to one of the pivot meshes below to tell the renderer how to render subsequent obj.
	AEGfxVertexList* TOP_LEFT_MESH;
	AEGfxVertexList* TOP_MID_MESH;
	AEGfxVertexList* TOP_RIGHT_MESH;
	AEGfxVertexList* MID_LEFT_MESH;
	AEGfxVertexList* MID_MESH;
	AEGfxVertexList* MID_RIGHT_MESH;
	AEGfxVertexList* BOT_LEFT_MESH;
	AEGfxVertexList* BOT_MID_MESH;
	AEGfxVertexList* BOT_RIGHT_MESH;

#pragma endregion

	void Initialize() {
		InitMesh();
		SetRenderPivot(TOP_LEFT);
	}

	void Render() {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		UpdateRenderSetting();
		for (auto& batch : renderBatches) {
			// Sort batch based on sprite's layer before drawing.
			SortBatch(batch);
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
					RenderText(obj.text.fontID, const_cast<char*>(obj.text.text.c_str()), obj.text.pos.x, obj.text.pos.y, obj.text.color);
					break;
				default:
					break;
				}
			}
			// Clear batch.
			batch.clear();
		}
	}

	// Rect with TEXTURE.
	void AddRectToBatch(const BATCH_TYPE& batch, const float& x, const float& y, const float& width, const float& height, TextureManager::TEX_TYPE tex, const int& layer, const float& rot) {
		AddRectToBatch(batch, x, y, width, height, rot, layer, {}, tex);
	}
	// Rect with COLOR.
	void AddRectToBatch(const BATCH_TYPE& batch, const float& x, const float& y, const float& width, const float& height, const Vec4<float>& btnColor, const int& layer, const float& rot) {
		AddRectToBatch(batch, x, y, width, height, rot, layer, btnColor, TextureManager::NONE);
	}

	void AddRectToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& width, const float& height, const float& rot, const int& layer, const Vec4<float>& color, TextureManager::TEX_TYPE tex)
	{
		Renderable obj;
		obj.type = RECT;
		obj.layer = layer;

		obj.rect.transform.pos = { x,y };
		obj.rect.transform.size = { width,height };
		obj.rect.transform.rot = rot;
		obj.rect.graphics.color = color;
		obj.rect.graphics.tex = tex;

		renderBatches[id].push_back(obj);
	}

	void AddTextToBatch(const BATCH_TYPE& id, const s8& font, const float& x, const float& y, std::string text, const Vec3<float>& color, const int& layer)
	{
		Renderable obj;
		obj.type = TEXT;
		obj.layer = layer;

		obj.text.pos = { x,y };
		obj.text.fontID = font;
		obj.text.text = text;
		obj.text.color = color;

		renderBatches[id].push_back(obj);
	}

	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const std::string& text, TextureManager::TEX_TYPE tex, const int& layer, const Vec3<float>& txtColor) {
		AddButtonToBatch(id, x, y, xPadding, yPadding, font, text, layer, tex, {}, txtColor);
	}
	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const std::string& text, const int& layer, const Vec4<float>& btnColor, const Vec3<float>& txtColor) {
		AddButtonToBatch(id, x, y, xPadding, yPadding, font, text, layer, TextureManager::NONE, btnColor, txtColor);
	}

	void AddButtonToBatch(const BATCH_TYPE& id, const float& x, const float& y, const float& xPadding, const float& yPadding, const s8& font, const std::string& text, const int& layer, TextureManager::TEX_TYPE tex, const Vec4<float>& btnColor, const Vec3<float>& txtColor) {
		AEGfxGetPrintSize(font, const_cast<char*>(text.c_str()), 1, textWidth, textHeight);
		AEVec2 buttonSize = GetButtonSize(xPadding, yPadding);
		AEVec2 textPos = RenderSystem::GetPivotPos(GetCenteredTextPos(x, y, buttonSize.x, buttonSize.y, textWidth, textHeight), buttonSize.x / AEGetWindowWidth() * 2, buttonSize.y / AEGetWindowHeight() * 2);

		if (tex == TextureManager::NONE) {
			AddRectToBatch(id, x, y, buttonSize.x, buttonSize.y, btnColor, layer);
		}
		else {
			AddRectToBatch(id, x, y, buttonSize.x, buttonSize.y, tex, layer);
		}
		AddTextToBatch(id, font, textPos.x, textPos.y, text, txtColor, layer);
	}

	AEVec2 GetButtonSize(const float& xPadding, const float& yPadding) {
		// Get button's width and height by adding text width/height with padding given.
		return AEVec2{ textWidth * AEGetWindowWidth() / 2 + xPadding * 2 , textHeight * AEGetWindowHeight() / 2 + yPadding * 2 };
	}

	AEVec2 GetCenteredTextPos(const float& x, const float& y, const float& width, const float& height, const float& textWidth, const float& textHeight) {
		// It just works
		return AEVec2{ ((x / AEGetWindowWidth()) * 2) + ((((width / AEGetWindowWidth()) * 2) - textWidth) / 2) , ((y / AEGetWindowHeight()) * 2) - ((height / 2) / AEGetWindowHeight()) * 2 - (textHeight / 2) };
	}

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
		std::cout << "UNABLE TO GET PIVOT POS";
		return pos;
	}

	/*!***********************************************************************
	* FOR RENDERING UI ELEMENTS
	*************************************************************************/
	void RenderText(s8 fontID, std::string text, float x, float y, Vec3<float> color) {
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// Allow transperant text for fading?
		AEGfxPrint(fontID, const_cast<char*>(text.c_str()), x, y, 1, color.x, color.y, color.z);
	}

	void RenderRect(const float& x, const float& y, const float& width, const float& height, TextureManager::TEX_TYPE tex) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(TextureManager::GetTexture(tex), TextureManager::GetTW(tex), TextureManager::GetTW(tex));
		UpdateRenderTransformMtx(x, y, AEVec2{ width,height });
		AEGfxMeshDraw(TextureManager::GetMesh(tex), AE_GFX_MDM_TRIANGLES);
	}

	void RenderRect(const float& x, const float& y, const float& width, const float& height, Vec4<float> color) {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTintColor(color.w, color.x, color.y, color.z);
		UpdateRenderTransformMtx(x, y, AEVec2{ width,height });
		AEGfxMeshDraw(TextureManager::GetMesh(TextureManager::NONE), AE_GFX_MDM_TRIANGLES);
	}
	/*************************************************************************/

	/*!***********************************************************************
	\brief
		Sort batch list based on layer value.
	*************************************************************************/
	void SortBatch(std::vector<Renderable> batch) {
		std::sort(batch.begin(), batch.end(), [](const Renderable& a, const Renderable& b) { return a.layer < b.layer; });
	}
	/*************************************************************************/

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
		Set current render pivot to determine how meshes are drawn.
	*************************************************************************/
	void SetRenderPivot(const RENDER_PIVOT& pivot) {
		renderPivot = pivot;
	}

	/*!***********************************************************************
	\brief
		Get render mesh based on current render pivot.
	*************************************************************************/
	AEGfxVertexList* GetRenderMesh() {
		switch (renderPivot)
		{
		case TOP_LEFT:
			return TOP_LEFT_MESH;
		case TOP_MID:
			return TOP_MID_MESH;
		case TOP_RIGHT:
			return TOP_RIGHT_MESH;
		case MID_LEFT:
			return MID_LEFT_MESH;
		case MID:
			return MID_MESH;
		case MID_RIGHT:
			return MID_RIGHT_MESH;
		case BOT_LEFT:
			return BOT_LEFT_MESH;
		case BOT_MID:
			return BOT_MID_MESH;
		case BOT_RIGHT:
			return BOT_RIGHT_MESH;
		default:
			break;
		}
		return TOP_LEFT_MESH;
	}
	/*************************************************************************/


	void InitMesh() {
		/*!***********************************************************************
		\brief
			Initialize meshes.
		*************************************************************************/

		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
			0.0f, -1.0f, 0xFFFFFFFF, 0.0f, 1.0f,
			1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
			1.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f,
			0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
		TOP_LEFT_MESH = AEGfxMeshEnd();


		AEGfxMeshStart();
		AEGfxTriAdd(-0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
			-0.5f, -1.0f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(-0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
			0.5f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f,
			0.5f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		TOP_MID_MESH = AEGfxMeshEnd();

		AEGfxMeshStart();
		AEGfxTriAdd(-1.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
			-1.0f, -1.0f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(-1.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
			0.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f,
			0.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		TOP_RIGHT_MESH = AEGfxMeshEnd();

		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
			0.0f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			1.0f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(1.0f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			1.0f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			0.0f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		MID_LEFT_MESH = AEGfxMeshEnd();

		AEGfxMeshStart();
		AEGfxTriAdd(-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		MID_MESH = AEGfxMeshEnd();

		AEGfxMeshStart();
		AEGfxTriAdd(-1.0f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
			-1.0f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.0f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.0f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.0f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-1.0f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		MID_RIGHT_MESH = AEGfxMeshEnd();

		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f,
			0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 1.0f,
			1.0f, 0.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(1.0f, 0.0f, 0xFFFFFFFF, 1.0f, 1.0f,
			1.0f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f,
			0.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f);
		BOT_LEFT_MESH = AEGfxMeshEnd();

		AEGfxMeshStart();
		AEGfxTriAdd(-0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f,
			-0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, 0.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.5f, 0.0f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f);
		BOT_MID_MESH = AEGfxMeshEnd();

		AEGfxMeshStart();
		AEGfxTriAdd(-1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f,
			-1.0f, 0.0f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.0f, 0.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.0f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f,
			-1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f);
		BOT_RIGHT_MESH = AEGfxMeshEnd();
	}
}
