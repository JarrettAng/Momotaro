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
#include <string>
#include <UIManager.h>

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
		CARD,
	};

	enum SPRITE_BATCH_TYPE {
		TILE_BATCH = 0,
		BUILDING_BATCH,
		NATURE_BATCH,
		CARD_BATCH,
	};

	enum RENDER_PIVOT {
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

	struct Sprite {
		SPRITE_TYPE type;
		int x, y;
		float rot = 0;
		AEVec2 scale = { 1,1 };
		int layer = 0;
		AEGfxTexture* tex;
		RenderSetting setting;

		float height, midHeight;
		float width, midWidth;
	};

	struct SpriteInfo {
		SPRITE_BATCH_TYPE id;
		SPRITE_TYPE type;
		int x, y;
		int layer = 0;
		RENDER_PIVOT pivot = MID;
		RenderSetting setting = {};
	};

	void Initialize();
	void Render();

	AEGfxVertexList* GetRenderMesh();
	void SetRenderMesh(RENDER_PIVOT pivot);

	void AddSpriteBatch(const SPRITE_BATCH_TYPE& id, const SPRITE_TYPE& type, const int& x, const int& y, const int& layer = 0, const RENDER_PIVOT& pivot = MID, RenderSetting setting = {});
	void AddUIBatch(UIManager::UIData data);


}

