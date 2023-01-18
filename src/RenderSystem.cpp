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

	void Renderer::Render() {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		UpdateRenderSetting();

		for (auto& batch : renderBatches) {
			for (auto& sprite : batch) {
				AEGfxTextureSet(sprite.tex, 0, 0);
				// Change render setting if needed.
				if (!sprite.setting.isDefault()) UpdateRenderSetting(sprite.setting);

				UpdateRenderTransformMtx(sprite.x, sprite.y, sprite.scale, sprite.rot);
				AEGfxMeshDraw(GetMesh(sprite.type).vertices, AE_GFX_MDM_TRIANGLES);

				// Reset back to default render setting if changed, for next sprite.
				if (!sprite.setting.isDefault()) UpdateRenderSetting();
			}
			// Clear sprites in batch.
			batch.clear();
		}
	}

	void Renderer::AddBatch(const BATCH_TYPE& id, const SPRITE_TYPE& type, const int& x, const int& y, const int& layer, const DRAW_PIVOT& pivot, RenderSetting setting) {

		Sprite sprite = GetSprite(type);
		sprite.type = type;
		sprite.x = x;
		sprite.y = y;
		sprite.layer = layer;
		sprite.setting = setting;

		// Adjust sprite X and Y position based on given pivot point.
		AlignToPivot(sprite.x, sprite.y, GetMesh(type), pivot);

		// Add to batch.
		renderBatches[GetBatch(id)].push_back(sprite);
		// Sort batch based on sprite's layer.
		SortBatchList(id);
	}

	void Renderer::AlignToPivot(int& x, int& y, const Mesh& mesh, const DRAW_PIVOT& pivot) {
		switch (pivot)
		{
		case RenderSystem::TOP_LEFT:
			// Drawn from TOP_LEFT
			break;
		case RenderSystem::TOP_MID:
			x -= mesh.midWidth;
			break;
		case RenderSystem::TOP_RIGHT:
			x -= mesh.width;
			break;
		case RenderSystem::MID_LEFT:
			y += mesh.midHeight;
			break;
		case RenderSystem::MID:
			x -= mesh.midWidth;
			y += mesh.midHeight;
			break;
		case RenderSystem::MID_RIGHT:
			x -= mesh.width;
			y += mesh.midHeight;
			break;
		case RenderSystem::BOT_LEFT:
			y += mesh.height;
			break;
		case RenderSystem::BOT_MID:
			x -= mesh.midWidth;
			y += mesh.height;
			break;
		case RenderSystem::BOT_RIGHT:
			x -= mesh.width;
			y += mesh.height;
			break;
		default:
			std::cout << "Invalid Pivot.";
			break;
		}
	}

	Mesh Renderer::GetMesh(SPRITE_TYPE type) {
		switch (type)
		{
		case RenderSystem::TILE:
			return tileMesh;
			break;
		case RenderSystem::RESIDENTIAL_S:
		case RenderSystem::RESIDENTIAL_M:
			return buildingMesh;
			break;
		default:
			break;
		}
	}

	Sprite& Renderer::GetSprite(const SPRITE_TYPE& type) {
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

	int Renderer::GetBatch(const BATCH_TYPE& id) {
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

	void Renderer::SortBatchList(const BATCH_TYPE& id) {
		renderBatches[GetBatch(id)].sort([](const Sprite& a, const Sprite& b) { return a.layer < b.layer; });
	}

	/*!***********************************************************************
	\brief
		Update engine render settings.
	*************************************************************************/
	void Renderer::UpdateRenderSetting(RenderSetting setting) {
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
	void Renderer::UpdateRenderTransformMtx(const int& x, const int& y, const float& scale, const float& rot) {
		// Apply rotation.
		AEMtx33RotDeg(&transform, rot);
		// Apply scaling
		AEMtx33ScaleApply(&transform, &transform, scale, scale);
		// Apply translation.
		AEMtx33TransApply(&transform, &transform, x, y);
		// Apply transform matrix to mesh to be drawn.
		AEGfxSetTransform(transform.m);
	}

	Renderer::Renderer() {
		InitMesh();
		LoadTextures();
	}

	void Renderer::InitMesh() {
		/*!***********************************************************************
		\brief
			// TODO: GET INFO USING FILE I/O
			Initialize mesh width and height.
		*************************************************************************/
		// TILE
		tileMesh.width = 100;
		tileMesh.height = 50;
		tileMesh.midWidth = tileMesh.width / 2;
		tileMesh.midHeight = tileMesh.height / 2;
		// BUILDING
		buildingMesh.width = 100;
		buildingMesh.height = 100;
		buildingMesh.midWidth = buildingMesh.width / 2;
		buildingMesh.midHeight = buildingMesh.height / 2;

		/*!***********************************************************************
		\brief
			Initialize mesh vertices.
		*************************************************************************/
		// TILE
		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, 0.0f, -tileMesh.height, 0xFFFFFFFF, 0.0f, 1.0f, tileMesh.width, -tileMesh.height, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, tileMesh.width, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f, tileMesh.width, -tileMesh.height, 0xFFFFFFFF, 1.0f, 1.0f);
		tileMesh.vertices = AEGfxMeshEnd();

		// BUILDING
		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 0.0f, 0x00FFFFFF, 0.0f, 0.0f, 0.0f, -buildingMesh.height, 0x00FFFFFF, 0.0f, 1.0f, buildingMesh.width, -buildingMesh.height, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.0f, 0.0f, 0x00FFFFFF, 0.0f, 0.0f, buildingMesh.width, 0.0f, 0x00FFFFFF, 1.0f, 0.0f, buildingMesh.width, -buildingMesh.height, 0xFFFFFFFF, 1.0f, 1.0f);
		buildingMesh.vertices = AEGfxMeshEnd();
	}

	void Renderer::LoadTextures() {
		tileSprite.tex = AEGfxTextureLoad("Assets/Tile.png");
		residential_S_Sprite.tex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	}
}
