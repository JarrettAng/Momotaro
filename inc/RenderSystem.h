#pragma once
namespace RenderSystem{

	// TEMP VEC 4
	struct Vec4F{
		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		float a = 1.0f;
	};


	enum MESH_TYPE{
		TRIANGLE,
		QUAD,
		TILE
	};

	class Renderer{

	private:
		AEGfxVertexList* triMesh{ 0 };
		AEGfxVertexList* quadMesh{ 0 };
		AEGfxVertexList* tileMesh{ 0 };
		// Transform matrix to apply to rendered mesh.
		AEMtx33 transform{ 0 };

	public:
		Renderer();
		void Draw(const MESH_TYPE type, const float x, const float y, const Vec4F tint = {});
		void Draw(const MESH_TYPE type, const float x, const float y, const float rot, const Vec4F tint = {});

	private:
		void InitMesh();
		AEGfxVertexList* GetMesh(MESH_TYPE type);
	};
}

