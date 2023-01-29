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

namespace RenderSystem {

#pragma region Foward Declaration & Variables
	void InitMesh();
	void LoadTextures();

	Sprite& GetSprite(const  SPRITE_TYPE& type);

	void SortUIBatch();
	void SortSpriteBatch(std::list<Sprite> batch);

	void UpdateRenderSetting(RenderSetting setting = {});
	void UpdateRenderTransformMtx(const int& x, const int& y, const AEVec2& scale, const float& rot = 0);

	void RenderText(s8 fontID, std::string text, float x, float y, float scale, Vec3<float> color = { 1.0f,1.0f,1.0f });
	void RenderRect(const float& x, const float& y, const float& width, const float& height, AEGfxTexture* tex);
	void RenderRect(const float& x, const float& y, const float& width, const float& height, Vec4<float> color = { 1.0f,1.0f,1.0f,1.0f });


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
	AEGfxVertexList* renderMesh;

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
	* TILE SPRITE
	*************************************************************************/
	Sprite tileSprite;

	/*!***********************************************************************
	* NATURE SPRITE
	*************************************************************************/
	Sprite nature_Sprite;

	/*!***********************************************************************
	* BUILDING SPRITE
	*************************************************************************/
	Sprite residential_S_Sprite;
	Sprite residential_M_Sprite;
	Sprite residential_L_Sprite;

	Sprite Commercial_S_Sprite;
	Sprite Commercial_M_Sprite;
	Sprite Commercial_L_Sprite;

	Sprite industrial_S_Sprite;
	Sprite industrial_M_Sprite;
	Sprite industrial_L_Sprite;

	/*!***********************************************************************
	* CARD SPRITE
	*************************************************************************/
	Sprite card_Sprite;

#pragma endregion

	void Initialize() {
		InitMesh();
		SetRenderMesh(TOP_LEFT);
		LoadTextures();
	}

	void Render() {
		/*!***********************************************************************
		* SPRITE
		*************************************************************************/
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		UpdateRenderSetting();

		for (auto& batch : spriteBatches) {
			// Sort batch based on sprite's layer before drawing.
			SortSpriteBatch(batch);
			for (auto& sprite : batch) {
				AEGfxTextureSet(sprite.tex, 0, 0);
				// Change render setting if needed.
				if (!sprite.setting.isDefault()) UpdateRenderSetting(sprite.setting);

				// Set position, rotation and scale of sprite.
				UpdateRenderTransformMtx(sprite.x, sprite.y, sprite.scale, sprite.rot);
				// Render sprites on screen.
				AEGfxMeshDraw(TOP_LEFT_MESH, AE_GFX_MDM_TRIANGLES);

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

	void AddSpriteBatch(const SPRITE_BATCH_TYPE& id, const SPRITE_TYPE& type, const int& x, const int& y, const int& layer, const RENDER_PIVOT& pivot, RenderSetting setting) {
		Sprite sprite = GetSprite(type);
		sprite.type = type;
		sprite.x = x;
		sprite.y = y;
		sprite.layer = layer;
		sprite.setting = setting;

		// Add to batch.
		spriteBatches[id].push_back(sprite);
	}

	void AddSpriteBatch(const SpriteInfo& batch) {
		AddSpriteBatch(batch.id, batch.type, batch.x, batch.y, batch.layer, batch.pivot, batch.setting);
	}

	void AddUIBatch(UIManager::UIData data) {
		UIBatch.push_back(data);
	}

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

	Sprite& GetSprite(const SPRITE_TYPE& type) {
		switch (type)
		{
		case TILE:
			return tileSprite;
			break;
		case RESIDENTIAL_S:
			return residential_S_Sprite;
		case RESIDENTIAL_M:
			return residential_M_Sprite;
			break;
		case NATURE:
			return nature_Sprite;
			break;
		case CARD:
			return card_Sprite;
			break;
		default:
			break;
		}
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

	void SetRenderMesh(RENDER_PIVOT pivot) {
		switch (pivot)
		{
		case RenderSystem::TOP_LEFT:
			renderMesh = TOP_LEFT_MESH;
			break;
		case RenderSystem::TOP_MID:
			renderMesh = TOP_MID_MESH;
			break;
		case RenderSystem::TOP_RIGHT:
			renderMesh = TOP_RIGHT_MESH;
			break;
		case RenderSystem::MID_LEFT:
			renderMesh = MID_LEFT_MESH;
			break;
		case RenderSystem::MID:
			renderMesh = MID_MESH;
			break;
		case RenderSystem::MID_RIGHT:
			renderMesh = MID_RIGHT_MESH;
			break;
		case RenderSystem::BOT_LEFT:
			renderMesh = BOT_LEFT_MESH;
			break;
		case RenderSystem::BOT_MID:
			renderMesh = BOT_MID_MESH;
			break;
		case RenderSystem::BOT_RIGHT:
			renderMesh = BOT_RIGHT_MESH;
			break;
		default:
			std::cout << "Unable to set render mesh.";
			break;
		}
	}
	AEGfxVertexList* GetRenderMesh() {
		return renderMesh;
	}

	void InitMesh() {
		/*!***********************************************************************
		\brief
			// TODO: GET INFO USING FILE I/O
			Initialize mesh width and height.
		*************************************************************************/
		// TILE
		tileSprite.width = 100;
		tileSprite.height = 100;
		tileSprite.scale = { tileSprite.width,tileSprite.height };
		tileSprite.midWidth = tileSprite.width / 2;
		tileSprite.midHeight = tileSprite.height / 2;

		//RESIDENTIAL_S
		residential_S_Sprite.width = 50;
		residential_S_Sprite.height = 100;
		residential_S_Sprite.scale = { residential_S_Sprite.width,residential_S_Sprite.height };
		residential_S_Sprite.midWidth = residential_S_Sprite.width / 2;
		residential_S_Sprite.midHeight = residential_S_Sprite.height / 2;

		//NATURE
		nature_Sprite.width = 25;
		nature_Sprite.height = 50;
		nature_Sprite.scale = { nature_Sprite.width,nature_Sprite.height };
		nature_Sprite.midWidth = nature_Sprite.width / 2;
		nature_Sprite.midHeight = nature_Sprite.height / 2;

		//CARD
		card_Sprite.width = 100;
		card_Sprite.height = 100;
		card_Sprite.scale = { card_Sprite.width,card_Sprite.height };
		card_Sprite.midWidth = card_Sprite.width / 2;
		card_Sprite.midHeight = card_Sprite.height / 2;

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

	void LoadTextures() {
		tileSprite.tex = AEGfxTextureLoad("Assets/Tile.png");
		card_Sprite.tex = AEGfxTextureLoad("Assets/Card.png");
		// tileSprite.tex = AEGfxTextureLoad("Assets/BlueRect.png");
		residential_S_Sprite.tex = AEGfxTextureLoad("Assets/residential_s_test.png");
		nature_Sprite.tex = AEGfxTextureLoad("Assets/tree_test.png");
	}
}
