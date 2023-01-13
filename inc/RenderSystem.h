#pragma once
namespace RenderSystem{
	// TEMP VEC 4
	template<class T>
	struct Vec4
	{
		T w;
		T x;
		T y;
		T z;
	};

	struct RenderSetting{
		// Default: Opaque + Allow transperancy. (PNG images)
		AEGfxBlendMode blendMode{ AE_GFX_BM_BLEND };
		float transperancy{ 1.0f };
		Vec4<float> tint{ 1.0f,1.0f,1.0f,1.0f };
	};

	// WIP
	struct Sprite {
		MESH_TYPE mesh;
		TEXTURE_TYPE tex;
	};

	enum MESH_TYPE{
		TRIANGLE,
		QUAD,
		TILE,
		BUILDING
	};

	enum TEXTURE_TYPE{
		NONE,
		TILE_TEX,
		BUILDING_TEX
	};

	class Renderer{

	private:
		AEGfxVertexList* triMesh{ 0 };
		AEGfxVertexList* quadMesh{ 0 };
		AEGfxVertexList* tileMesh{ 0 };
		AEGfxVertexList* buildingMesh{ 0 };

		AEGfxTexture* tileTex = AEGfxTextureLoad("Assets/Tile.png");
		AEGfxTexture* buildingTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
		// Identidy transform matrix of rendered mesh.
		AEMtx33 transform{ 1,0,0, 0,1,0, 0,0,1 };

	public:
		Renderer();
		void Draw(const MESH_TYPE mesh, const TEXTURE_TYPE tex, const float x, const float y, const float scale = 1, const float rot = 0);
		void Draw(const MESH_TYPE mesh, const TEXTURE_TYPE tex, RenderSetting settings, const float x, const float y, const float scale = 1, const float rot = 0);

		void DrawMesh(const MESH_TYPE type, const float x, const float y);
		void DrawMesh(const MESH_TYPE type, const float x, const float y, const float scale = 1, const float rot = 0);

	private:
		void InitMesh();
		AEGfxVertexList* GetMesh(MESH_TYPE type);
		AEGfxTexture* GetTex(TEXTURE_TYPE type);
		void UpdateRenderTransformMtx(const float x, const float y, const float scale = 1, const float rot = 0);
		void UpdateRenderSetting(RenderSetting setting = {});
	};
}

