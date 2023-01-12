#include<AEEngine.h>
#include<RenderSystem.h>
namespace _RenderSystem{

	RenderSystem::RenderSystem(){
		AEGfxVertexList* triMesh = 0;
		AEGfxVertexList* quadMesh = 0;
		InitMesh();
	}

	void RenderSystem::DrawTriangle(float x, float y){
		AEGfxSetPosition(x, y);
		AEGfxMeshDraw(triMesh, AE_GFX_MDM_TRIANGLES);
	}

	// TODO: DRAW TRI WITH TINT

	void RenderSystem::InitMesh(){
		AEGfxMeshStart();
		AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f, 0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f, -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
		triMesh = AEGfxMeshEnd();
	}
}




