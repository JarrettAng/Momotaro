#include<AEEngine.h>
#include<RenderSystem.h>
#include <iostream>
namespace RenderSystem{

	/*!***********************************************************************
	\brief
		Draw sprite on screen.
	*************************************************************************/
	void Renderer::Draw(const MESH_TYPE mesh, const TEXTURE_TYPE tex, const float x, const float y, const float scale, const float rot){
		// Drawing object with texture.
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		UpdateRenderSetting();

		// Select texture to use.
		AEGfxTextureSet(GetTex(tex), 0, 0);

		// Apply translation, scaling and rotation to global transform mtx.
		UpdateRenderTransformMtx(x, y, scale, rot);

		// Draw sprite (Mesh w/ texuture).
		AEGfxMeshDraw(GetMesh(mesh), AE_GFX_MDM_TRIANGLES);
	}

	/*!***********************************************************************
	\brief
		Draw sprite on screen. Allow more customization on how to render the sprite using RenderSetting.
	*************************************************************************/
	void Renderer::Draw(const MESH_TYPE mesh, const TEXTURE_TYPE tex, RenderSetting settings, const float x, const float y, const float scale, const float rot){
		// Drawing object with texture.
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		UpdateRenderSetting(settings);

		// Select texture to use.
		AEGfxTextureSet(GetTex(tex), 0, 0);

		// Apply translation, scaling and rotation to global transform mtx.
		UpdateRenderTransformMtx(x, y, scale, rot);

		// Draw mesh.
		AEGfxMeshDraw(GetMesh(mesh), AE_GFX_MDM_TRIANGLES);
	}

	/*!***********************************************************************
	\brief
		Draw mesh on screen.
	*************************************************************************/
	void Renderer::DrawMesh(const MESH_TYPE type, const float x, const float y){
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Position to render mesh.
		AEGfxSetPosition(x, y);

		// Draw mesh.
		AEGfxMeshDraw(GetMesh(type), AE_GFX_MDM_TRIANGLES);
	}

	/*!***********************************************************************
	\brief
		Draw mesh on screen. Allow scaling and rotation.
	*************************************************************************/
	void Renderer::DrawMesh(const MESH_TYPE type, const float x, const float y, const float scale, const float rot){
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		// Rotate mesh.
		UpdateRenderTransformMtx(x, y, scale, rot);

		// Draw mesh.
		AEGfxMeshDraw(GetMesh(type), AE_GFX_MDM_TRIANGLES);
	}

	/*!***********************************************************************
	\brief
		Get mesh from given type.
	*************************************************************************/
	AEGfxVertexList* Renderer::GetMesh(MESH_TYPE type){
		switch (type)
		{
		case RenderSystem::TRIANGLE:
			return triMesh;
			break;
		case RenderSystem::QUAD:
			return quadMesh;
			break;
		case RenderSystem::TILE:
			return tileMesh;
			break;
		case RenderSystem::BUILDING:
			return buildingMesh;
			break;
		default:
			std::cerr << "Mesh type undefined.";
			break;
		}
	}

	/*!***********************************************************************
	\brief
		Get texture from given type.
	*************************************************************************/
	AEGfxTexture* Renderer::GetTex(TEXTURE_TYPE type){
		switch (type)
		{
		case RenderSystem::NONE:
			return nullptr;
			break;
		case RenderSystem::TILE_TEX:
			return tileTex;
			break;
		case RenderSystem::BUILDING_TEX:
			return buildingTex;
			break;
		default:
			std::cerr << "Textue type undefined.";
			break;
		}
	}

	/*!***********************************************************************
	\brief
		Update engine render settings.
	*************************************************************************/
	void Renderer::UpdateRenderSetting(RenderSetting setting){
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
	void Renderer::UpdateRenderTransformMtx(const float x, const float y, const float scale, const float rot){
		// Apply rotation.
		AEMtx33RotDeg(&transform, rot);
		// Apply scaling
		AEMtx33ScaleApply(&transform, &transform, scale, scale);
		// Apply translation.
		AEMtx33TransApply(&transform, &transform, x, y);
		// Apply transform matrix to mesh to be drawn.
		AEGfxSetTransform(transform.m);
	}

	Renderer::Renderer(){
		InitMesh();
	}

	void Renderer::InitMesh(){
		// TRIANGLE
		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 100.0f, 0xFFFFFFFF, 0.0f, 0.0f, 0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 1.0f, 100.0f, 0.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		triMesh = AEGfxMeshEnd();

		// QUAD
		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, 0.0f, 100.0f, 0xFFFFFFFF, 0.0f, 1.0f, 100.0f, 100.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, 100.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f, 100.0f, 100.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		quadMesh = AEGfxMeshEnd();

		// TILE
		AEGfxMeshStart();
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, 0.0f, 100.0f, 0xFFFFFFFF, 0.0f, 1.0f, 200.0f, 100.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f, 200.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f, 200.0f, 100.0f, 0xFFFFFFFF, 1.0f, 1.0f);
		tileMesh = AEGfxMeshEnd();
	}
}




