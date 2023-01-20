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

	int GetBatch(const BATCH_TYPE& id);
	void SortBatchList(const BATCH_TYPE& id);

	void UpdateRenderSetting(RenderSetting setting = {});
	void UpdateRenderTransformMtx(const int& x, const int& y, const AEVec2& scale, const float& rot = 0);

	//void AlignToPivot(int& x, int& y, const Mesh&, const DRAW_PIVOT& pivot);

	std::list<Sprite> tileBatch;
	std::list<Sprite> buildingBatch;
	std::vector<std::list<Sprite>> renderBatches = { tileBatch,buildingBatch };

	AEMtx33 transformMtx{
	1,0,0,
	0,1,0,
	0,0,1
	};
	AEMtx33 translateMtx{
	1,0,0,
	0,1,0,
	0,0,1
	};
	AEMtx33 scaleMtx{
	1,0,0,
	0,1,0,
	0,0,1
	};
	AEMtx33 rotMtx{
	1,0,0,
	0,1,0,
	0,0,1
	};

	AEGfxVertexList* spriteMesh;

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
#pragma endregion

	void Initialize() {
		InitMesh();
		LoadTextures();
	}

	void Render() {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		UpdateRenderSetting();

		for (auto& batch : renderBatches) {
			for (auto& sprite : batch) {
				AEGfxTextureSet(sprite.tex, 0, 0);
				// Change render setting if needed.
				if (!sprite.setting.isDefault()) UpdateRenderSetting(sprite.setting);

				UpdateRenderTransformMtx(sprite.x, sprite.y, sprite.scale, sprite.rot);
				AEGfxMeshDraw(spriteMesh, AE_GFX_MDM_TRIANGLES);

				// Reset back to default render setting if changed, for next sprite.
				if (!sprite.setting.isDefault()) UpdateRenderSetting();
			}
			// Clear sprites in batch.
			batch.clear();
		}
	}

	void AddBatch(const BATCH_TYPE& id, const SPRITE_TYPE& type, const int& x, const int& y, const int& layer, const DRAW_PIVOT& pivot, RenderSetting setting) {

		Sprite sprite = GetSprite(type);
		sprite.type = type;
		sprite.x = x;
		sprite.y = y;
		sprite.layer = layer;
		sprite.setting = setting;

		// Add to batch.
		renderBatches[GetBatch(id)].push_back(sprite);
		// Sort batch based on sprite's layer.
		SortBatchList(id);
	}

	void AddBatch(const SpriteInfo& batch) {
		AddBatch(batch.id, batch.type, batch.x, batch.y, batch.layer, batch.pivot, batch.setting);
	}

	//void AlignToPivot(int& x, int& y, const Mesh& mesh, const DRAW_PIVOT& pivot) {
	//	switch (pivot)
	//	{
	//	case RenderSystem::TOP_LEFT:
	//		// Drawn from TOP_LEFT
	//		break;
	//	case RenderSystem::TOP_MID:
	//		x -= mesh.midWidth;
	//		break;
	//	case RenderSystem::TOP_RIGHT:
	//		x -= mesh.width;
	//		break;
	//	case RenderSystem::MID_LEFT:
	//		y += mesh.midHeight;
	//		break;
	//	case RenderSystem::MID:
	//		x -= mesh.midWidth;
	//		y += mesh.midHeight;
	//		break;
	//	case RenderSystem::MID_RIGHT:
	//		x -= mesh.width;
	//		y += mesh.midHeight;
	//		break;
	//	case RenderSystem::BOT_LEFT:
	//		y += mesh.height;
	//		break;
	//	case RenderSystem::BOT_MID:
	//		x -= mesh.midWidth;
	//		y += mesh.height;
	//		break;
	//	case RenderSystem::BOT_RIGHT:
	//		x -= mesh.width;
	//		y += mesh.height;
	//		break;
	//	default:
	//		std::cout << "Invalid Pivot.";
	//		break;
	//	}
	//}

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
		default:
			break;
		}
	}

	int GetBatch(const BATCH_TYPE& id) {
		switch (id)
		{
		case TILE_BATCH:
			return TILE_BATCH;
			break;
		case BUILDING_BATCH:
			return BUILDING_BATCH;
			break;
		default:
			break;
		}
	}

	void SortBatchList(const BATCH_TYPE& id) {
		renderBatches[GetBatch(id)].sort([](const Sprite& a, const Sprite& b) { return a.layer < b.layer; });
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

	void InitMesh() {
		/*!***********************************************************************
		\brief
			// TODO: GET INFO USING FILE I/O
			Initialize mesh width and height.
		*************************************************************************/
		// TILE
		tileSprite.width = 100;
		tileSprite.height = 50;
		tileSprite.scale = { tileSprite.width,tileSprite.height };
		tileSprite.midWidth = tileSprite.width / 2;
		tileSprite.midHeight = tileSprite.height / 2;

		/*!***********************************************************************
		\brief
			Initialize mesh vertices.
		*************************************************************************/
		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, 0.0f, -1.0f, 0xFFFFFFFF, 0.0f, 1.0f, 1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, 1.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f, 1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		spriteMesh = AEGfxMeshEnd();
	}

	void LoadTextures() {
		tileSprite.tex = AEGfxTextureLoad("Assets/Tile.png");
		// tileSprite.tex = AEGfxTextureLoad("Assets/BlueRect.png");
		residential_S_Sprite.tex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	}
}
