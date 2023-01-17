#include<AEEngine.h>
#include<RenderSystem.h>
#include <iostream>
#include <algorithm>
namespace RenderSystem {

	void Renderer::AddBatch(SPRITE_TYPE type, const int& x, const int& y, const int& layer, const DRAW_PIVOT& pivot) {

		batch.x = x;
		batch.y = y;
		batch.layer = layer;

		AlignToPivot(batch.x, batch.y, GetMesh(type), pivot);

		batchList.push_back(batch);
		SortBatchList();

	}

	void Renderer::SortBatchList() {
		batchList.sort([](const Batch& a, const Batch& b) { return a.layer < b.layer; });
	}

	void Renderer::Render() {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(tileTex, 0, 0);
		UpdateRenderSetting();
		// -> Update rot and scale.

		for (auto& sprite : batchList) {
			// Use specified render setting if needed.
			UpdateRenderTransformMtx(sprite.x, sprite.y);
			AEGfxMeshDraw(tileVertices, AE_GFX_MDM_TRIANGLES);
		}
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
			return tileSprite.mesh;
			break;
		case RenderSystem::BUILDING:
			return buildingSprite.mesh;
			break;
		default:
			break;
		}
	}

	/*!***********************************************************************
	\brief
		Draw sprite on screen.
	*************************************************************************/
	//void Renderer::Draw(const MESH_TYPE mesh, const TEXTURE_TYPE tex, const float x, const float y, const float scale, const float rot) {
	//	// Drawing object with texture.
	//	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//	UpdateRenderSetting();

	//	// Select texture to use.
	//	AEGfxTextureSet(GetTex(tex), 0, 0);

	//	// Apply translation, scaling and rotation to global transform mtx.
	//	UpdateRenderTransformMtx(x, y, scale, rot);

	//	// Draw sprite (Mesh w/ texuture).
	//	AEGfxMeshDraw(GetMesh(mesh), AE_GFX_MDM_TRIANGLES);
	//}

	/*!***********************************************************************
	\brief
		Draw sprite on screen. Allow more customization on how to render the sprite using RenderSetting.
	*************************************************************************/
	//void Renderer::Draw(const MESH_TYPE mesh, const TEXTURE_TYPE tex, RenderSetting settings, const float x, const float y, const float scale, const float rot) {
	//	// Drawing object with texture.
	//	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//	UpdateRenderSetting(settings);

	//	// Select texture to use.
	//	AEGfxTextureSet(GetTex(tex), 0, 0);

	//	// Apply translation, scaling and rotation to global transform mtx.
	//	UpdateRenderTransformMtx(x, y, scale, rot);

	//	// Draw mesh.
	//	AEGfxMeshDraw(GetMesh(mesh), AE_GFX_MDM_TRIANGLES);
	//}

	/*!***********************************************************************
	\brief
		Draw mesh on screen.
	*************************************************************************/
	//void Renderer::DrawMesh(const MESH_TYPE type, const float x, const float y) {
	//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//	// Position to render mesh.
	//	AEGfxSetPosition(x, y);

	//	// Draw mesh.
	//	AEGfxMeshDraw(GetMesh(type), AE_GFX_MDM_TRIANGLES);
	//}

	/*!***********************************************************************
	\brief
		Draw mesh on screen. Allow scaling and rotation.
	*************************************************************************/
	//void Renderer::DrawMesh(const MESH_TYPE type, const float x, const float y, const float scale, const float rot) {
	//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	//	// Rotate mesh.
	//	UpdateRenderTransformMtx(x, y, scale, rot);

	//	// Draw mesh.
	//	AEGfxMeshDraw(GetMesh(type), AE_GFX_MDM_TRIANGLES);
	//}

	/*!***********************************************************************
	\brief
		Get mesh from given type.
	*************************************************************************/
	//AEGfxVertexList* Renderer::GetMesh(MESH_TYPE type) {
	//	switch (type)
	//	{
	//	case RenderSystem::TRIANGLE:
	//		return triMesh;
	//		break;
	//	case RenderSystem::QUAD:
	//		return quadMesh;
	//		break;
	//	case RenderSystem::TILE:
	//		return tileMesh;
	//		break;
	//	case RenderSystem::BUILDING:
	//		return buildingMesh;
	//		break;
	//	default:
	//		std::cerr << "Mesh type undefined.";
	//		break;
	//	}
	//}

	/*!***********************************************************************
	\brief
		Get texture from given type.
	*************************************************************************/
	//AEGfxTexture* Renderer::GetTex(TEXTURE_TYPE type) {
	//	switch (type)
	//	{
	//	case RenderSystem::NONE:
	//		return nullptr;
	//		break;
	//	case RenderSystem::TILE_TEX:
	//		return tileTex;
	//		break;
	//	case RenderSystem::BUILDING_TEX:
	//		return buildingTex;
	//		break;
	//	default:
	//		std::cerr << "Textue type undefined.";
	//		break;
	//	}
	//}

	/*!***********************************************************************
	\brief
		Update engine render settings.
	*************************************************************************/
	void Renderer::UpdateRenderSetting(RenderSetting setting) {
		// AE_GFX_BM_BLEND to allow transperency.
		AEGfxSetBlendMode(setting.blendMode);
		// Apply tint.
		AEGfxSetTintColor(setting.tint.w, setting.tint.x, setting.tint.y, setting.tint.z);
		// Apply transparency.
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
	}

	void Renderer::InitMesh() {
		/*!***********************************************************************
		\brief
			// TODO: GET INFO USING FILE I/O
			Initialize mesh width and height.
		*************************************************************************/
		// TILE
		tileSprite.mesh.width = 100;
		tileSprite.mesh.height = 50;
		tileSprite.mesh.midWidth = tileSprite.mesh.width / 2;
		tileSprite.mesh.midHeight = tileSprite.mesh.height / 2;
		// BUILDING
		buildingSprite.mesh.width = 200;
		buildingSprite.mesh.height = 100;
		buildingSprite.mesh.midWidth = buildingSprite.mesh.width / 2;
		buildingSprite.mesh.midHeight = buildingSprite.mesh.height / 2;

		/*!***********************************************************************
		\brief
			Initialize mesh vertices.
		*************************************************************************/
		// TILE
		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, 0.0f, -tileSprite.mesh.height, 0xFFFFFFFF, 0.0f, 1.0f, tileSprite.mesh.width, -tileSprite.mesh.height, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, tileSprite.mesh.width, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f, tileSprite.mesh.width, -tileSprite.mesh.height, 0xFFFFFFFF, 1.0f, 1.0f);
		tileVertices = AEGfxMeshEnd();

		// BUILDING
		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, 0.0f, -buildingSprite.mesh.height, 0xFFFFFFFF, 0.0f, 1.0f, buildingSprite.mesh.width, -buildingSprite.mesh.height, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, buildingSprite.mesh.width, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f, buildingSprite.mesh.width, -buildingSprite.mesh.height, 0xFFFFFFFF, 1.0f, 1.0f);
		buildingVertices = AEGfxMeshEnd();
	}
}




