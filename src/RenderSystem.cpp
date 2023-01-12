#include<AEEngine.h>
#include<RenderSystem.h>
#include <iostream>
namespace RenderSystem{

	void Renderer::Draw(const MESH_TYPE type, const float x, const float y, const Vec4F tint){
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Position to render mesh.
		AEGfxSetPosition(x, y);

		AEGfxSetTintColor(tint.r, tint.g, tint.b, tint.a);
		// Draw mesh.
		AEGfxMeshDraw(GetMesh(type), AE_GFX_MDM_TRIANGLES);
	}

	void Renderer::Draw(const MESH_TYPE type, const float x, const float y, const float rot, const Vec4F tint){
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Position to render mesh.
		AEGfxSetPosition(x, y);
		// Rotate mesh.
		AEMtx33RotDeg(&transform, rot);
		AEGfxSetTintColor(tint.r, tint.g, tint.b, tint.a);
		// Apply scaling to transform.
		AEGfxSetTransform(transform.m);

		// Draw mesh.
		AEGfxMeshDraw(GetMesh(type), AE_GFX_MDM_TRIANGLES);
	}

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
		default:
			std::cerr << "Mesh type undefined.";
			break;
		}
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




