#pragma once
#include <list>
namespace RenderSystem {
	// VEC 4
	// TODO: MOVE THIS SOMEWHERE ELSE
	template<class T>
	struct Vec4
	{
		T w;
		T x;
		T y;
		T z;
	};

	// Overload == operator to compare two Vec4.
	// TODO: MOVE THIS SOMEWHERE ELSE
	template<class T>
	bool operator==(const Vec4<T>& lhs, const Vec4<T>& rhs)
	{
		if (lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z) {
			return true;
		}
		return false;
	}

	struct RenderSetting {
		// Default: Opaque + Allow transperancy. (PNG images)
		static const AEGfxBlendMode BLEND_MODE{ AE_GFX_BM_BLEND };
		static constexpr float TRANSPERANCY = 1.0f;
		static constexpr Vec4<float> TINT{ 1.0f,1.0f,1.0f,1.0f };

		AEGfxBlendMode blendMode = BLEND_MODE;
		float transperancy = TRANSPERANCY;
		Vec4<float> tint = TINT;
		bool isDefault() { return blendMode == BLEND_MODE && transperancy == TRANSPERANCY && tint == TINT; }
	};

	enum MESH_TYPE {
		TRIANGLE,
		QUAD,
	};

	enum SPRITE_TYPE {
		TILE,
		BUILDING
	};

	enum DRAW_PIVOT {
		TOP_LEFT,
		TOP_MID,
		TOP_RIGHT,
		MID_LEFT,
		MID,
		MID_RIGHT,
		BOT_LEFT,
		BOT_MID,
		BOT_RIGHT
	};

	struct Mesh {
		double height, midHeight;
		double width, midWidth;
	};

	struct Sprite {
		SPRITE_TYPE type;
		Mesh mesh;
	};

	struct Batch {
		int x, y;
		Mesh meshInfo;
		RenderSetting setting;
		int layer = 0;
	};

	class Renderer {

	private:
		std::list<Batch> batchList;
		Batch batch;
		AEGfxTexture* loadedTex = nullptr;

		Sprite tileSprite;
		AEGfxVertexList* tileVertices = { 0 };
		AEGfxTexture* tileTex = AEGfxTextureLoad("Assets/Tile.png");

		Sprite buildingSprite;
		AEGfxVertexList* buildingVertices = { 0 };
		AEGfxTexture* buildingTex = AEGfxTextureLoad("Assets/PlanetTexture.png");

		// Identity transform matrix of rendered mesh.
		AEMtx33 transform{
			1,0,0,
			0,1,0,
			0,0,1
		};

	public:
		Renderer();
		void AddBatch(SPRITE_TYPE type, const int& x, const int& y, const int& priority = 0, const DRAW_PIVOT& pivot = MID);
		void Render();

	private:
		void InitMesh();
		Mesh GetMesh(SPRITE_TYPE type);
		AEGfxTexture* GetTex(SPRITE_TYPE type);
		void SortBatchList();
		void UpdateRenderTransformMtx(const int& x, const int& y, const float& scale = 1, const float& rot = 0);
		void UpdateRenderSetting(RenderSetting setting = {});
		void AlignToPivot(int& x, int& y, const Mesh&, const DRAW_PIVOT& pivot);
	};
}

