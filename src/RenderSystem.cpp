/*!************************************************************************
\file RenderSystem.cpp
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\date 18/01/2023
\brief
This source file declares

**************************************************************************/

#include<AEEngine.h>
#include<RenderSystem.h>
#include <iostream>
#include <algorithm>
#include <TextureManager.h>

namespace RenderSystem {

#pragma region Foward Declaration & Variables
	void InitMesh();
	void InitializeSprite();

	void SortUIBatch();
	void SortSpriteBatch(std::list<Sprite> batch);

	void UpdateRenderSetting(RenderSetting setting = {});
	void UpdateRenderTransformMtx(const int& x, const int& y, const AEVec2& scale, const float& rot = 0);

	void RenderText(s8 fontID, std::string text, float x, float y, float scale, Vec3<float> color = { 1.0f,1.0f,1.0f });
	void RenderRect(const float& x, const float& y, const float& width, const float& height, AEGfxTexture* tex);
	void RenderRect(const float& x, const float& y, const float& width, const float& height, Vec4<float> color = { 1.0f,1.0f,1.0f,1.0f });

	Sprite& GetSprite(const  SPRITE_TYPE& type);
	AEVec2 GetSpriteSize(const SPRITE_TYPE& type);

	/*!***********************************************************************
	* SPRITE BATCHES
	*************************************************************************/
	std::list<Sprite> tileBatch;
	std::list<Sprite> buildingBatch;
	std::list<Sprite> natureBatch;
	std::list<Sprite> cardBatch;
	std::vector<std::list<Sprite>> spriteBatches = { tileBatch, buildingBatch, natureBatch, cardBatch };

	/*!***********************************************************************
	* UI BATCHES (DATA IS ADDED THROUGH UIMANAGER)
	*************************************************************************/
	std::list<UIManager::UIData> UIBatch;

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

	/*!***********************************************************************
	* SPRITES
	*************************************************************************/
	Sprite tile_Sprite;
	Sprite nature_Sprite;
	Sprite building_Sprite;
	Sprite card_Sprite;


#pragma endregion

	void Initialize() {
		InitMesh();
		SetRenderPivot(TOP_LEFT);
		InitializeSprite();
	}

	void Render() {
		/*!***********************************************************************
		* SPRITE RENDERING
		*************************************************************************/
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		UpdateRenderSetting();

		for (auto& batch : spriteBatches) {
			// Sort batch based on sprite's layer before drawing.
			SortSpriteBatch(batch);
			for (auto& sprite : batch) {
				AEGfxTextureSet(TextureManager::GetTexure(sprite.tex), 0, 0);
				// Change render setting if needed.
				if (!sprite.setting.isDefault()) UpdateRenderSetting(sprite.setting);

				// Set position, rotation and scale of sprite.
				UpdateRenderTransformMtx(sprite.x, sprite.y, sprite.size, sprite.rot);
				// Render sprites on screen.
				AEGfxMeshDraw(GetRenderMesh(), AE_GFX_MDM_TRIANGLES);

				// Reset back to default render setting if changed, for next sprite.
				if (!sprite.setting.isDefault()) UpdateRenderSetting();
			}
			// Clear sprites in batch.
			batch.clear();
		}

		/*!***********************************************************************
		* UI RENDERING
		*************************************************************************/
		SortUIBatch();
		for (auto& data : UIBatch) {
			// Sort batch before drawing.
			if (data.graphics.hasGraphics) {
				// Mesh CANNOT contain both texture and color.
				if (data.graphics.tex) {
					// Render rect with texture.
					RenderRect(data.transform.x, data.transform.y, data.transform.width, data.transform.height, data.graphics.tex);
				}
				else {
					// Render rect with color.
					RenderRect(data.transform.x, data.transform.y, data.transform.width, data.transform.height, data.graphics.color);
				}
			}
			// Draw text above graphics.
			if (data.text.hasText) {
				RenderText(data.text.fontID, const_cast<char*>(data.text.text.c_str()), data.text.x, data.text.y, data.text.scale, data.text.color);
			}
		}
		// Clear UI in batch.
		UIBatch.clear();
	}

	void AddSpriteBatch(const SpriteInfo& batch) {
		AddSpriteBatch(batch.id, batch.type, batch.tex, batch.x, batch.y, batch.layer, batch.rot, batch.setting);
	}

	void AddSpriteBatch(const SPRITE_BATCH_TYPE& id, const SPRITE_TYPE& type, const TextureManager::TEX_TYPE& tex, const int& x, const int& y, const int& layer, const float& rot, RenderSetting setting) {
		Sprite sprite = GetSprite(type);
		sprite.tex = tex;
		sprite.x = x;
		sprite.y = y;
		sprite.rot = rot;
		sprite.layer = layer;
		sprite.setting = setting;

		// Add to batch.
		spriteBatches[id].push_back(sprite);
	}

	void AddUIBatch(UIManager::UIData data) {
		UIBatch.push_back(data);
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
	void RenderText(s8 fontID, std::string text, float x, float y, float scale, Vec3<float> color) {
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// Allow transperant text for fading?
		AEGfxPrint(fontID, const_cast<char*>(text.c_str()), x, y, scale, color.x, color.y, color.z);
	}

	void RenderRect(const float& x, const float& y, const float& width, const float& height, AEGfxTexture* tex) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(tex, 0, 0);
		UpdateRenderTransformMtx(x, y, AEVec2{ width,height });
		AEGfxMeshDraw(GetRenderMesh(), AE_GFX_MDM_TRIANGLES);
	}

	void RenderRect(const float& x, const float& y, const float& width, const float& height, Vec4<float> color) {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTintColor(color.w, color.x, color.y, color.z);
		UpdateRenderTransformMtx(x, y, AEVec2{ width,height });
		AEGfxMeshDraw(GetRenderMesh(), AE_GFX_MDM_TRIANGLES);
	}
	/*************************************************************************/

	Sprite& GetSprite(const SPRITE_TYPE& type) {
		switch (type)
		{
		case TILE:
			return tile_Sprite;
		case NATURE:
			return nature_Sprite;
		case BUILDING:
			return building_Sprite;
		case CARD:
			return card_Sprite;
		default:
			break;
		}
		std::cout << "INVALID SPRITE TYPE WHEN CALLING GetSprite().";
	}

	AEVec2 GetSpriteSize(const SPRITE_TYPE& type) {
		switch (type)
		{
		case TILE:
			return tile_Sprite.size;
		case NATURE:
			return nature_Sprite.size;
		case BUILDING:
			return building_Sprite.size;
		case CARD:
			return card_Sprite.size;
		default:
			break;
		}
		std::cout << "INVALID SPRITE TYPE WHEN CALLING GetSpriteSize()";
		return AEVec2{};
	}

	/*!***********************************************************************
	\brief
		Sort batch list based on layer value.
	*************************************************************************/
	void SortSpriteBatch(std::list<Sprite> batch) {
		batch.sort([](const Sprite& a, const Sprite& b) { return a.layer < b.layer; });
	}
	void SortUIBatch() {
		UIBatch.sort([](const UIManager::UIData& a, const  UIManager::UIData& b) { return a.layer < b.layer; });
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

	/*!***********************************************************************
	\brief
		Initialize sprite type and size. (Size is used for mesh scaling)
	*************************************************************************/
	void InitializeSprite() {
		tile_Sprite.type = TILE;
		nature_Sprite.type = NATURE;
		building_Sprite.type = BUILDING;
		card_Sprite.type = CARD;

		// TODO: GET SIZE USING FILE I/O
		tile_Sprite.size = { 100,100 };
		nature_Sprite.size = { 100,100 };
		building_Sprite.size = { 100,100 };
		card_Sprite.size = { 100,100 };
	}

	void InitMesh() {
		/*!***********************************************************************
		\brief
			Initialize meshes.
		*************************************************************************/

		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
			0.0f, -1.0f, 0xFFFFFFFF, 0.0f, 1.0f,
			1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
			1.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f,
			1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f);
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
