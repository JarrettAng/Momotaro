/*!************************************************************************
\file RenderSystem.h
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\date 18/01/2023
\brief
This header file declares

**************************************************************************/

#pragma once
#include <list>
#include <vector>
#include <AEEngine.h>
#include <MomoMaths.h>

namespace RenderSystem {

	struct RenderSetting {
		// Default: Opaque + Allow transperancy. (PNG images)
		static const AEGfxBlendMode BLEND_MODE{ AE_GFX_BM_BLEND };
		static constexpr Vec4<float> BLEND_COLOR{ 0,0,0,0 };
		static constexpr Vec4<float> TINT{ 1.0f,1.0f,1.0f,1.0f };
		static constexpr float TRANSPERANCY = 1.0f;


		AEGfxBlendMode blendMode = BLEND_MODE;
		Vec4<float> blendColor = BLEND_COLOR;
		Vec4<float> tint = TINT;
		float transperancy = TRANSPERANCY;
		bool isDefault() { return blendMode == BLEND_MODE && blendColor == BLEND_COLOR && transperancy == TRANSPERANCY && tint == TINT; }
	};

	enum SPRITE_TYPE {
		TILE,
		NATURE,
		RESIDENTIAL_S,
		RESIDENTIAL_M,
		RESIDENTIAL_L,
	};

	enum BATCH_TYPE {
		TILE_BATCH = 0,
		BUILDING_BATCH
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
		AEGfxVertexList* vertices = { 0 };
	};

	struct Sprite {
		SPRITE_TYPE type;
		int x, y;
		double rot = 0, scale = 1;
		int layer = 0;
		AEGfxTexture* tex;
		RenderSetting setting;
	};

	struct SpriteInfo {
		BATCH_TYPE id;
		SPRITE_TYPE type;
		int x, y;
		int layer = 0;
		DRAW_PIVOT pivot = MID;
		RenderSetting setting = { };
	};

	//class Renderer {
	// private:
		/*!***********************************************************************
		BATCH ID:
		0 = TILE
		1 = BUILDING
		2 = HAND
		3 = BUILDING_DRAGGED
		4 = UI
		*************************************************************************/
		//std::list<Sprite> tileBatch;
		//std::list<Sprite> buildingBatch;
		//std::vector<std::list<Sprite>> renderBatches = { tileBatch,buildingBatch };

		// Identity transform matrix of rendered mesh.
		//AEMtx33 transform{
		//	1,0,0,
		//	0,1,0,
		//	0,0,1
		//};

	// public:
		// Renderer();
		void Initialize();
		void Render();
		void AddBatch(const BATCH_TYPE& id, const SPRITE_TYPE& type, const int& x, const int& y, const int& layer = 0, const DRAW_PIVOT& pivot = MID, RenderSetting setting = {});

	// private:
		//void InitMesh();
		//void LoadTextures();

		//Mesh GetMesh(SPRITE_TYPE type);
		//// AEGfxTexture* GetTex(SPRITE_TYPE type);
		//Sprite& GetSprite(const  SPRITE_TYPE& type);

		//int GetBatch(const BATCH_TYPE& id);
		//void SortBatchList(const BATCH_TYPE& id);

		//void UpdateRenderSetting(RenderSetting setting = {});
		//void UpdateRenderTransformMtx(const int& x, const int& y, const float& scale = 1, const float& rot = 0);

		//void AlignToPivot(int& x, int& y, const Mesh&, const DRAW_PIVOT& pivot);

	// private:
		// MESH IS LIKE A SKELETON
		//Mesh tileMesh;
		//Mesh buildingMesh;
		//Mesh natureMesh;
		//Mesh cardMesh;

		//// SPRITE IS THE MEAT ON THE SKELETON.
		//// LoadTexture() put skin on the meat.
		//Sprite tileSprite;

		///*!***********************************************************************
		//* NATURE SPRITE
		//*************************************************************************/
		//Sprite nature_Sprite;

		///*!***********************************************************************
		//* BUILDING SPRITE
		//*************************************************************************/
		//Sprite residential_S_Sprite;
		//Sprite residential_M_Sprite;
		//Sprite residential_L_Sprite;

		//Sprite Commercial_S_Sprite;
		//Sprite Commercial_M_Sprite;
		//Sprite Commercial_L_Sprite;

		//Sprite industrial_S_Sprite;
		//Sprite industrial_M_Sprite;
		//Sprite industrial_L_Sprite;
	// };
}

